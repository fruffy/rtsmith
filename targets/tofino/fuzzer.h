#ifndef BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_TOFINO_FUZZER_H_
#define BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_TOFINO_FUZZER_H_

#include "backends/p4tools/modules/p4rtsmith/core/fuzzer.h"
#include "backends/p4tools/modules/p4rtsmith/targets/tofino/program_info.h"

namespace P4Tools::RTSmith::Tna {

class TofinoTnaFuzzer : public P4RuntimeFuzzer {
 private:
    /// @returns the program info associated with the current target.
    [[nodiscard]] const TofinoTnaProgramInfo &getProgramInfo() const override;

 public:
    explicit TofinoTnaFuzzer(const TofinoTnaProgramInfo &programInfo);

    InitialP4RuntimeConfig produceInitialConfig() override;

    P4RuntimeUpdateSeries produceUpdateTimeSeries() override;
};

}  // namespace P4Tools::RTSmith::Tna

#endif /* BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_TOFINO_FUZZER_H_ */
