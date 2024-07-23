#include "backends/p4tools/modules/p4rtsmith/options.h"

#include "backends/p4tools/common/compiler/context.h"
#include "backends/p4tools/common/lib/util.h"
#include "backends/p4tools/common/options.h"
#include "backends/p4tools/modules/p4rtsmith/toolname.h"
#include "lib/cstring.h"
#include "lib/exceptions.h"

namespace P4Tools {

RtSmithOptions &RtSmithOptions::get() {
    return P4Tools::CompileContext<RtSmithOptions>::get().options();
}

const std::set<std::string> K_SUPPORTED_CONTROL_PLANES = {"P4RUNTIME", "BFRUNTIME"};

const char *RtSmithOptions::getIncludePath() {
    P4C_UNIMPLEMENTED("getIncludePath not implemented for P4RuntimeSmith.");
}

RtSmithOptions::RtSmithOptions()
    : AbstractP4cToolOptions(RTSmith::TOOL_NAME,
                             "Remove control-plane dead code from a P4 program.") {
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
    registerOption(
        "--generate-p4info", "filePath",
        [this](const char *arg) {
            _p4InfoFilePath = arg;
            std::cout << "_p4InfoFilePath: " << _p4InfoFilePath.value() << std::endl;
            if (_p4InfoFilePath.value().extension() != ".txtpb") {
                ::error("%1% must have a .txtpb extension.", _p4InfoFilePath.value().c_str());
                return false;
            }
            return true;
        },
        "Write the P4Runtime control plane API description (P4Info) to the specified .txtpb file.");
    registerOption(
        "--control-plane", "controlPlaneApi",
        [this](const char *arg) {
            _controlPlaneApi = arg;
            transform(_controlPlaneApi.begin(), _controlPlaneApi.end(), _controlPlaneApi.begin(),
                      ::toupper);
            if (K_SUPPORTED_CONTROL_PLANES.find(_controlPlaneApi) ==
                K_SUPPORTED_CONTROL_PLANES.end()) {
                ::error(
                    "Test back end %1% not implemented for this target. Supported back ends are "
                    "%2%.",
                    _controlPlaneApi, Utils::containerToString(K_SUPPORTED_CONTROL_PLANES));
                return false;
            }
            return true;
        },
        "Specifies the control plane API to use. Defaults to P4Rtuntime.");
}

std::optional<std::filesystem::path> RtSmithOptions::getOutputDir() const { return outputDir_; }

bool RtSmithOptions::printToStdout() const { return printToStdout_; }

std::optional<std::string> RtSmithOptions::getConfigFilePath() const { return configFilePath; }

std::optional<std::filesystem::path> RtSmithOptions::p4InfoFilePath() const { return _p4InfoFilePath; }

std::string_view RtSmithOptions::controlPlaneApi() const { return _controlPlaneApi; }

}  // namespace P4Tools
