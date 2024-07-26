#ifndef BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_TOFINO_FUZZER_H_
#define BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_TOFINO_FUZZER_H_

#include "backends/p4tools/modules/p4rtsmith/core/fuzzer.h"
#include "backends/p4tools/modules/p4rtsmith/targets/tofino/program_info.h"

namespace P4Tools::RTSmith::Tna {

class TofinoTnaFuzzer : public BFRuntimeFuzzer {
 private:
    /// @returns the program info associated with the current target.
    [[nodiscard]] const TofinoTnaProgramInfo &getProgramInfo() const override;

 public:
    explicit TofinoTnaFuzzer(const TofinoTnaProgramInfo &programInfo);

    bfrt_proto::TableEntry produceTableEntry(
        const p4::config::v1::Table &table,
        const google::protobuf::RepeatedPtrField<p4::config::v1::Action> &actions) override;

    InitialConfig produceInitialConfig() override;

    UpdateSeries produceUpdateTimeSeries() override;
};

}  // namespace P4Tools::RTSmith::Tna

#endif /* BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_TOFINO_FUZZER_H_ */
