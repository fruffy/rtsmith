#include "backends/p4tools/modules/p4rtsmith/targets/bmv2/fuzzer.h"

#include "backends/p4tools/common/lib/util.h"
#include "backends/p4tools/modules/p4rtsmith/core/fuzzer.h"

namespace P4::P4Tools::RTSmith::V1Model {

Bmv2V1ModelFuzzer::Bmv2V1ModelFuzzer(const Bmv2V1ModelProgramInfo &programInfo)
    : P4RuntimeFuzzer(programInfo) {}

const Bmv2V1ModelProgramInfo &Bmv2V1ModelFuzzer::getProgramInfo() const {
    return *P4RuntimeFuzzer::getProgramInfo().checkedTo<Bmv2V1ModelProgramInfo>();
}

InitialConfig Bmv2V1ModelFuzzer::produceInitialConfig() {
    InitialConfig initialConfig;
    initialConfig.push_back(produceWriteRequest(true));
    return initialConfig;
}

UpdateSeries Bmv2V1ModelFuzzer::produceUpdateTimeSeries() {
    UpdateSeries updateSeries;
    size_t updateCount = rtSmithOptions.getFuzzerConfig().getRandomUpdateCount();
    for (size_t idx = 0; idx < updateCount; ++idx) {
        auto microseconds = rtSmithOptions.getFuzzerConfig().getRandomUpdateTimeInMicroseconds();
        updateSeries.emplace_back(microseconds, produceWriteRequest(false));
    }

    return updateSeries;
}

}  // namespace P4::P4Tools::RTSmith::V1Model
