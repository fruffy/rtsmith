#include "backends/p4tools/modules/p4rtsmith/options.h"

#include <random>

#include "backends/p4tools/common/compiler/context.h"
#include "backends/p4tools/common/lib/logging.h"
#include "backends/p4tools/common/lib/util.h"
#include "backends/p4tools/common/options.h"
#include "backends/p4tools/modules/p4rtsmith/toolname.h"
#include "lib/error.h"

namespace P4::P4Tools::RTSmith {

RtSmithOptions &RtSmithOptions::get() {
    return P4Tools::CompileContext<RtSmithOptions>::get().options();
}

const std::set<std::string> K_SUPPORTED_CONTROL_PLANES = {"P4RUNTIME", "BFRUNTIME"};

RtSmithOptions::RtSmithOptions()
    : AbstractP4cToolOptions(RTSmith::TOOL_NAME,
                             "Remove control-plane dead code from a P4 program.") {
    registerOption(
        "--print-to-stdout", nullptr,
        [this](const char *) {
            _printToStdout = true;
            return true;
        },
        "Whether to write the generated config to a file or to stdout.");
    registerOption(
        "--output-dir", "outputDir",
        [this](const char *arg) {
            _outputDir = std::filesystem::path(arg);
            return true;
        },
        "The path where config file(s) are being emitted.");
    registerOption(
        "--config-name", "configName",
        [this](const char *arg) {
            _configName = arg;
            return true;
        },
        "The base name of the config files. Optional.");
    registerOption(
        "--user-p4info", "filePath",
        [this](const char *arg) {
            _userP4Info = arg;
            if (!std::filesystem::exists(_userP4Info.value())) {
                error("%1% does not exist. Please provide a valid file path.",
                      _userP4Info.value().c_str());
                return false;
            }
            return true;
        },
        "Use user-provided P4Runtime control plane API description (P4Info).");
    registerOption(
        "--generate-p4info", "filePath",
        [this](const char *arg) {
            _p4InfoFilePath = arg;
            if (_p4InfoFilePath.value().extension() != ".txtpb") {
                error("%1% must have a .txtpb extension.", _p4InfoFilePath.value().c_str());
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
                error(
                    "Test back end %1% not implemented for this target. Supported back ends are "
                    "%2%.",
                    _controlPlaneApi, Utils::containerToString(K_SUPPORTED_CONTROL_PLANES));
                return false;
            }
            return true;
        },
        "Specifies the control plane API to use. Defaults to P4Rtuntime.");
    registerOption(
        "--random-seed", nullptr,
        [this](const char *) {
            if (seed.has_value()) {
                error("Seed has already been set to %1%.", *seed);
                return false;
            }
            // No seed provided, we generate our own.
            std::random_device r;
            seed = r();
            Utils::setRandomSeed(*seed);
            printInfo("Using randomly generated seed %1%", *seed);
            return true;
        },
        "Use a random seed.");
    registerOption(
        "--classbench [CONFIG_TYPE]", nullptr,
        [this](const char *arg) {
            // Check if the configuration type is specified.
            if (arg == nullptr) {
                ::P4::error("No configuration type specified. Please specify one.");
                return false;
            }
            // Check if the configuration type is valid.
            if (std::string(arg) != "<config-type-placeholder-1>" &&
                std::string(arg) != "<config-type-placeholder-2>" &&
                std::string(arg) != "<config-type-placeholder-3>") {
                ::P4::error("Invalid configuration type %1% for --classbench.", arg);
                return false;
            }
            _configType = std::string(arg);
            printInfo("Using configuration type %1%.", _configType);
        },
        "Specifies the configuration type to generation the desired configuration.");
}

std::filesystem::path RtSmithOptions::outputDir() const { return _outputDir; }

bool RtSmithOptions::validateOptions() const {
    if (_userP4Info.has_value() && _p4InfoFilePath.has_value()) {
        error("Both --user-p4info and --generate-p4info are specified. Please specify only one.");
        return false;
    }
    if (!seed.has_value()) {
        warning("No seed is set. Will always choose 0 for random values.");
    }
    return true;
}

bool RtSmithOptions::printToStdout() const { return _printToStdout; }

std::optional<std::string> RtSmithOptions::configName() const { return _configName; }

std::optional<std::filesystem::path> RtSmithOptions::userP4Info() const { return _userP4Info; }

std::optional<std::filesystem::path> RtSmithOptions::p4InfoFilePath() const {
    return _p4InfoFilePath;
}

std::string_view RtSmithOptions::controlPlaneApi() const { return _controlPlaneApi; }

}  // namespace P4::P4Tools::RTSmith
