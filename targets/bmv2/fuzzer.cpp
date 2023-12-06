#include "backends/p4tools/modules/p4rtsmith/targets/bmv2/fuzzer.h"

namespace P4Tools::RTSmith::V1Model {

Bmv2V1ModelFuzzer::Bmv2V1ModelFuzzer(const Bmv2V1ModelProgramInfo &programInfo)
    : P4RuntimeFuzzer(programInfo) {}

const Bmv2V1ModelProgramInfo &Bmv2V1ModelFuzzer::getProgramInfo() const {
    return *P4RuntimeFuzzer::getProgramInfo().checkedTo<Bmv2V1ModelProgramInfo>();
}

InitialP4RuntimeConfig Bmv2V1ModelFuzzer::produceInitialConfig() { return {}; }

P4RuntimeUpdateSeries Bmv2V1ModelFuzzer::produceUpdateTimeSeries() { return {}; }

}  // namespace P4Tools::RTSmith::V1Model
