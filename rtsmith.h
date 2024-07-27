#ifndef BACKENDS_P4TOOLS_MODULES_P4RTSMITH_RTSMITH_H_
#define BACKENDS_P4TOOLS_MODULES_P4RTSMITH_RTSMITH_H_

#include "backends/p4tools/common/p4ctool.h"
#include "backends/p4tools/modules/p4rtsmith/core/fuzzer.h"
#include "backends/p4tools/modules/p4rtsmith/options.h"

namespace P4Tools::RTSmith {

struct RtSmithResult {
    InitialConfig config;
    UpdateSeries updateSeries;

    RtSmithResult(InitialConfig &&config, UpdateSeries &&updateSeries)
        : config(std::move(config)), updateSeries(std::move(updateSeries)) {}

    // RtSmithResult(const P4Tools::RTSmith::RtSmithResult&) = delete;

    RtSmithResult(RtSmithResult &other) : RtSmithResult(std::move(other)) {}

    RtSmithResult(RtSmithResult &&other) {
        config = std::move(other.config);
        updateSeries = std::move(other.updateSeries);
    }
};

/// This is main implementation of the P4RuntimeSmith tool.
class RtSmith : public AbstractP4cTool<RtSmithOptions> {
 protected:
    void registerTarget() override;

    int mainImpl(const CompilerResult &compilerResult) override;

 public:
    virtual ~RtSmith() = default;

    static std::optional<RtSmithResult> generateConfig(const std::string &program,
                                                       const RtSmithOptions &rtSmithOptions);

    static std::optional<RtSmithResult> generateConfig(const RtSmithOptions &rtSmithOptions);
};

}  // namespace P4Tools::RTSmith

#endif /* BACKENDS_P4TOOLS_MODULES_P4RTSMITH_RTSMITH_H_ */
