#include "backends/p4tools/modules/p4rtsmith/rtsmith.h"

#include <google/protobuf/text_format.h>

#include <cstdlib>

#include "backends/p4tools/common/compiler/context.h"
#include "backends/p4tools/common/lib/logging.h"
#include "backends/p4tools/modules/p4rtsmith/core/target.h"
#include "backends/p4tools/modules/p4rtsmith/core/util.h"
#include "backends/p4tools/modules/p4rtsmith/register.h"
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

    const auto *programInfo = RtSmithTarget::produceProgramInfo(compilerResult);
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

std::optional<RtSmithResult> generateConfigImpl(
    std::optional<std::reference_wrapper<const std::string>> program,
    const CompilerOptions &compilerOptions, const RtSmithOptions & /*rtSmithOptions*/) {
    // Register supported compiler targets.
    registerCompilerTargets();

    // Register supported P4RTSmith targets.
    registerRtSmithTargets();

    P4Tools::Target::init(compilerOptions.target.c_str(), compilerOptions.arch.c_str());

    // Set up the compilation context.
    auto *compileContext = new CompileContext<CompilerOptions>();
    compileContext->options() = compilerOptions;
    AutoCompileContext autoContext(compileContext);

    CompilerResultOrError compilerResult;
    if (program.has_value()) {
        // Run the compiler to get an IR and invoke the tool.
        ASSIGN_OR_RETURN(compilerResult,
                         P4Tools::CompilerTarget::runCompiler(program.value().get()), std::nullopt);
    } else {
        RETURN_IF_FALSE_WITH_MESSAGE(!compilerOptions.file.isNullOrEmpty(), std::nullopt,
                                     ::error("Expected a file input."));
        // Run the compiler to get an IR and invoke the tool.
        ASSIGN_OR_RETURN(compilerResult, P4Tools::CompilerTarget::runCompiler(), std::nullopt);
    }

    const auto *programInfo = RtSmithTarget::produceProgramInfo(compilerResult.value());
    if (programInfo == nullptr || ::errorCount() > 0) {
        ::error("P4RTSmith encountered errors during preprocessing.");
        return std::nullopt;
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
    return {{initialConfig, timeSeriesUpdates}};
}

std::optional<RtSmithResult> RtSmith::generateConfig(const std::string &program,
                                                     const CompilerOptions &compilerOptions,
                                                     const RtSmithOptions &rtSmithOptions) {
    try {
        return generateConfigImpl(program, compilerOptions, rtSmithOptions);
    } catch (const std::exception &e) {
        std::cerr << "Internal error: " << e.what() << "\n";
        return std::nullopt;
    } catch (...) {
        return std::nullopt;
    }
    return std::nullopt;
}

std::optional<RtSmithResult> RtSmith::generateConfig(const CompilerOptions &compilerOptions,
                                                     const RtSmithOptions &rtSmithOptions) {
    try {
        return generateConfigImpl(std::nullopt, compilerOptions, rtSmithOptions);
    } catch (const std::exception &e) {
        std::cerr << "Internal error: " << e.what() << "\n";
        return std::nullopt;
    } catch (...) {
        return std::nullopt;
    }
    return std::nullopt;
}

}  // namespace P4Tools::RTSmith
