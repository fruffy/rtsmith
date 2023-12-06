#ifndef BACKENDS_P4TOOLS_MODULES_P4RTSMITH_OPTIONS_H_
#define BACKENDS_P4TOOLS_MODULES_P4RTSMITH_OPTIONS_H_

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

 private:
    RtSmithOptions();
};

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_P4RTSMITH_OPTIONS_H_ */
