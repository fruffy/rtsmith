#include "backends/p4tools/modules/p4rtsmith/options.h"

#include "backends/p4tools/common/options.h"
#include "lib/cstring.h"
#include "lib/exceptions.h"

namespace P4Tools {

RtSmithOptions &RtSmithOptions::get() {
    static RtSmithOptions INSTANCE;
    return INSTANCE;
}

const char *RtSmithOptions::getIncludePath() {
    P4C_UNIMPLEMENTED("getIncludePath not implemented for P4RuntimeSmith.");
}

RtSmithOptions::RtSmithOptions()
    : AbstractP4cToolOptions("Remove control-plane dead code from a P4 program.") {
    registerOption(
        "--print-to-stdout", nullptr,
        [this](const char *) {
            printToStdout_ = true;
            return true;
        },
        "Whether to write the generated config to a file or to stdout.");

    registerOption(
        "--output-dir", "outputDir",
        [this](const char *arg) {
            outputDir_ = std::filesystem::path(arg);
            return true;
        },
        "The path to the output file of the config file(s).");

    registerOption(
        "--generate-config", "filePath",
        [this](const char *arg) {
            configFilePath = arg;
            if (configFilePath.value().extension() != ".txtpb") {
                ::error("%1% must have a .txtpb extension.", configFilePath.value().c_str());
                return false;
            }
            return true;
        },
        "Write the generated config to the specified .txtpb file.");
}

std::optional<std::filesystem::path> RtSmithOptions::getOutputDir() const { return outputDir_; }

bool RtSmithOptions::printToStdout() const { return printToStdout_; }

std::optional<std::string> RtSmithOptions::getConfigFilePath() const { return configFilePath; }

}  // namespace P4Tools
