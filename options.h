#ifndef BACKENDS_P4TOOLS_MODULES_P4RTSMITH_OPTIONS_H_
#define BACKENDS_P4TOOLS_MODULES_P4RTSMITH_OPTIONS_H_

#include <filesystem>
#include <optional>

#include "backends/p4tools/common/options.h"

namespace P4Tools {

/// Encapsulates and processes command-line options for P4RtSmith.
class RtSmithOptions : public AbstractP4cToolOptions {
 public:
    RtSmithOptions(const RtSmithOptions &) = delete;

    RtSmithOptions(RtSmithOptions &&) = delete;

    RtSmithOptions &operator=(const RtSmithOptions &) = delete;

    RtSmithOptions &operator=(RtSmithOptions &&) = delete;

    virtual ~RtSmithOptions() = default;

    /// @returns the singleton instance of this class.
    static RtSmithOptions &get();

    const char *getIncludePath() override;

    /// @returns true when the --print-to-stdout option has been set.
    [[nodiscard]] bool printToStdout() const;

    /// @returns the path set with --output-dir.
    [[nodiscard]] std::optional<std::filesystem::path> getOutputDir() const;

    /// @returns the path set with --generate-config.
    [[nodiscard]] std::optional<std::string> getConfigFilePath() const;

 private:
    RtSmithOptions();

    // Write the generated config to the specified file.
    std::optional<std::filesystem::path> configFilePath = std::nullopt;

    /// The path to the output file of the config file.
    std::optional<std::filesystem::path> outputDir_ = std::nullopt;

    /// Whether to write the generated config to a file or to stdout.
    bool printToStdout_ = false;
};

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_P4RTSMITH_OPTIONS_H_ */
