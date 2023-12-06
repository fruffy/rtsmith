#ifndef BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_BMV2_FUZZER_H_
#define BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_BMV2_FUZZER_H_

#include "backends/p4tools/modules/p4rtsmith/core/fuzzer.h"
#include "backends/p4tools/modules/p4rtsmith/targets/bmv2/program_info.h"

namespace P4Tools::RTSmith::V1Model {

class Bmv2V1ModelFuzzer : public P4RuntimeFuzzer {
 private:
    /// @returns the program info associated with the current target.
    [[nodiscard]] const Bmv2V1ModelProgramInfo &getProgramInfo() const override;

 public:
    explicit Bmv2V1ModelFuzzer(const Bmv2V1ModelProgramInfo &programInfo);

    InitialP4RuntimeConfig produceInitialConfig() override;

    P4RuntimeUpdateSeries produceUpdateTimeSeries() override;
};

}  // namespace P4Tools::RTSmith::V1Model

#endif /* BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_BMV2_FUZZER_H_ */
