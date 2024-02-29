#include "backends/p4tools/modules/p4rtsmith/rtsmith.h"

#include <google/protobuf/text_format.h>

#include <cstdlib>

#include "backends/p4tools/common/lib/logging.h"
#include "backends/p4tools/modules/p4rtsmith/core/target.h"
#include "backends/p4tools/modules/p4rtsmith/register.h"
#include "frontends/common/parseInput.h"
#include "lib/error.h"

namespace P4Tools::RTSmith {

void RtSmith::registerTarget() {
    // Register all available compiler targets.
    // These are discovered by CMAKE, which fills out the register.h.in file.
    registerCompilerTargets();
}

int RtSmith::mainImpl(const CompilerResult &compilerResult) {
    // Register all available P4RuntimeSmith targets.
    // These are discovered by CMAKE, which fills out the register.h.in file.
    registerRtSmithTargets();

    enableInformationLogging();

    auto seed = Utils::getCurrentSeed();
    if (seed) {
        printFeature("test_info", 4, "============ Program seed %1% =============\n", *seed);
    }

    const auto *programInfo = RtSmithTarget::produceProgramInfo(&compilerResult.getProgram());
    if (programInfo == nullptr) {
        ::error("Program not supported by target device and architecture.");
        return EXIT_FAILURE;
    }
    if (::errorCount() > 0) {
        ::error("P4RuntimeSmith: Encountered errors during preprocessing. Exiting");
        return EXIT_FAILURE;
    }

    auto p4RuntimeApi = programInfo->getP4RuntimeApi();
    printInfo("Inferred API:\n%1%", p4RuntimeApi.p4Info->DebugString());

    auto &fuzzer = RtSmithTarget::getFuzzer(*programInfo);

    auto initialConfig = fuzzer.produceInitialConfig();
    printInfo("Generated initial configuration:");
    for (const auto &writeRequest : initialConfig) {
        printInfo("%1%", writeRequest.DebugString());
    }

    auto timeSeriesUpdates = fuzzer.produceUpdateTimeSeries();
    printInfo("Time series updates:");
    for (const auto &[time, writeRequest] : timeSeriesUpdates) {
        printInfo("Time %1%:\n%2%", writeRequest.DebugString());
    }

    return ::errorCount() == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

}  // namespace P4Tools::RTSmith
