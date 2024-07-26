#ifndef BACKENDS_P4TOOLS_MODULES_P4RTSMITH_OPTIONS_H_
#define BACKENDS_P4TOOLS_MODULES_P4RTSMITH_OPTIONS_H_

#include <filesystem>
#include <optional>

#include "backends/p4tools/common/options.h"

namespace P4Tools {

/// Encapsulates and processes command-line options for P4RtSmith.
class RtSmithOptions : public AbstractP4cToolOptions {
 public:
    RtSmithOptions();

    /// @returns the singleton instance of this class.
    static RtSmithOptions &get();

    const char *getIncludePath();

    /// @returns true when the --print-to-stdout option has been set.
    [[nodiscard]] bool printToStdout() const;

    /// @returns the path set with --output-dir.
    [[nodiscard]] std::filesystem::path outputDir() const;

    /// @returns the path set with --generate-config.
    [[nodiscard]] std::optional<std::string> configName() const;

    /// @returns the path set with --generate-p4info.
    [[nodiscard]] std::filesystem::path p4InfoFilePath() const;

    /// @returns the control plane API to use.
    [[nodiscard]] std::string_view controlPlaneApi() const;

 protected:
    // Write the generated config to the specified file.
    std::optional<std::string> _configName = std::nullopt;

    /// The path to the output file of the config file.
    std::filesystem::path _outputDir;

    /// Whether to write the generated config to a file or to stdout.
    bool _printToStdout = false;

    // Write the P4Runtime control plane API description to the specified file.
    std::filesystem::path _p4InfoFilePath;

    // The control plane API to use. Defaults to P4Runtime.
    std::string _controlPlaneApi = "P4RUNTIME";
};

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_P4RTSMITH_OPTIONS_H_ */
