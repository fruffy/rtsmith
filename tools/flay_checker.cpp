#include <cstdlib>

#include "backends/p4tools/common/compiler/context.h"
#include "backends/p4tools/common/lib/logging.h"
#include "backends/p4tools/modules/flay/core/lib/return_macros.h"
#include "backends/p4tools/modules/flay/flay.h"
#include "backends/p4tools/modules/flay/register.h"
#include "backends/p4tools/modules/p4rtsmith/options.h"
#include "backends/p4tools/modules/p4rtsmith/register.h"
#include "backends/p4tools/modules/p4rtsmith/rtsmith.h"
#include "frontends/common/parser_options.h"
#include "lib/compile_context.h"
#include "lib/error.h"
#include "lib/options.h"

namespace P4Tools {

namespace {

class FlayCheckerOptions : public RtSmithOptions {
    /// Write a performance report.
    bool _writePerformanceReport = false;

 public:
    FlayCheckerOptions() {
        registerOption(
            "--file", "inputFile",
            [this](const char *arg) {
                file = arg;
                if (!std::filesystem::exists(file)) {
                    ::error("The input P4 program '%s' does not exist.", file.c_str());
                    return false;
                }
                return true;
            },
            "The input file to process.");
        registerOption(
            "--enable-info-logging", nullptr,
            [](const char *) {
                enableInformationLogging();
                return true;
            },
            "Print verbose messages.");
        registerOption(
            "--write-performance-report", nullptr,
            [this](const char *) {
                enablePerformanceLogging();
                _writePerformanceReport = true;
                return true;
            },
            "Write a performance report for the file. The report will be written to either the "
            "location of the reference file or the location of the folder.");
    }

    ~FlayCheckerOptions() override = default;

    // Process options; return list of remaining options.
    // Returns EXIT_FAILURE if an error occurred.
    int processOptions(int argc, char *const argv[]) {
        auto *unprocessedOptions = process(argc, argv);
        if (unprocessedOptions == nullptr) {
            return EXIT_FAILURE;
        }
        if (unprocessedOptions != nullptr && !unprocessedOptions->empty()) {
            for (const auto &option : *unprocessedOptions) {
                ::error("Unprocessed input: %s", option);
            }
            return EXIT_FAILURE;
        }
        if (file.empty()) {
            ::error("No input file specified.");
            return EXIT_FAILURE;
        }
        if (_outputDir.empty()) {
            _outputDir = std::tmpnam(nullptr);
            printInfo("Using temporary directory: %s", _outputDir.c_str());
        }
        // If the environment variable P4FLAY_INFO or RTSMITH_INFO is set, enable information
        // logging.
        if (std::getenv("P4FLAY_INFO") != nullptr || std::getenv("RTSMITH_INFO") != nullptr) {
            enableInformationLogging();
        }
        return EXIT_SUCCESS;
    }

    [[nodiscard]] const std::filesystem::path &getInputFile() const { return file; }

    [[nodiscard]] const RtSmithOptions &toRtSmithOptions() const { return *this; }

    [[nodiscard]] bool writePerformanceReport() const { return _writePerformanceReport; }
};

}  // namespace

int run(const FlayCheckerOptions &options, const RtSmithOptions &rtSmithOptions) {
    printInfo("Generating RtSmith configuration for program...");
    ASSIGN_OR_RETURN(auto configResult, RTSmith::RtSmith::generateConfig(rtSmithOptions),
                     EXIT_FAILURE);

    printInfo("RtSmith configuration complete.");
    printInfo("Starting Flay optimization...");
    {
        auto *flayContext = new CompileContext<FlayOptions>();
        AutoCompileContext autoContext(flayContext);
        FlayOptions &flayOptions = flayContext->options();
        flayOptions.target = options.target;
        flayOptions.arch = options.arch;
        flayOptions.file = options.file;
        flayOptions.setConfigurationUpdatePattern(rtSmithOptions.outputDir() / "*update_*.txtpb");
        ASSIGN_OR_RETURN(auto flayServiceStatistics, Flay::Flay::optimizeProgram(flayOptions),
                         EXIT_FAILURE);
    }
    printInfo("Flay optimization complete.");

    return EXIT_SUCCESS;
}

}  // namespace P4Tools

int main(int argc, char *argv[]) {
    P4Tools::Flay::registerFlayTargets();
    P4Tools::RTSmith::registerRtSmithTargets();

    // Set up the options.
    auto *compileContext = new P4Tools::CompileContext<P4Tools::FlayCheckerOptions>();
    AutoCompileContext autoContext(new P4CContextWithOptions<P4Tools::FlayCheckerOptions>());
    // Process command-line options.
    if (compileContext->options().processOptions(argc, argv) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    auto *rtSmithContext = new P4Tools::CompileContext<P4Tools::RtSmithOptions>(*compileContext);
    AutoCompileContext autoContext2(rtSmithContext);
    // Run the reference checker.
    auto result = P4Tools::run(compileContext->options(), rtSmithContext->options());
    if (result == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    return ::errorCount() == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
