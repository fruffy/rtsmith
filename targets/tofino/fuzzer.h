#ifndef BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_TOFINO_FUZZER_H_
#define BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_TOFINO_FUZZER_H_

#include "backends/p4tools/modules/p4rtsmith/core/fuzzer.h"
#include "backends/p4tools/modules/p4rtsmith/targets/tofino/program_info.h"

namespace P4Tools::RTSmith::Tna {

class TofinoTnaFuzzer : public RuntimeFuzzer {
 private:
    /// @returns the program info associated with the current target.
    [[nodiscard]] const TofinoTnaProgramInfo &getProgramInfo() const override;

 public:
    explicit TofinoTnaFuzzer(const TofinoTnaProgramInfo &programInfo);

    /// @brief Produce a `produceKeyField_Exact` with bitwidth.
    /// @param bitwidth
    /// @return A `produceKeyField_Exact`.
    virtual bfrt_proto::KeyField_Exact produceKeyField_Exact(int bitwidth);

    /// @brief Produce a `produceKeyField_LPM` with bitwidth.
    /// @param bitwidth
    /// @return A `produceKeyField_LPM`.
    virtual bfrt_proto::KeyField_LPM produceKeyField_LPM(int bitwidth);

    /// @brief Produce a `produceKeyField_Ternary` with bitwidth.
    /// @param bitwidth
    /// @return A `produceKeyField_Ternary`.
    virtual bfrt_proto::KeyField_Ternary produceKeyField_Ternary(int bitwidth);

    /// @brief Produce a `KeyField_Optional` with bitwidth.
    /// @param bitwidth
    /// @return A `KeyField_Optional`.
    virtual bfrt_proto::KeyField_Optional produceKeyField_Optional(int bitwidth);

    /// @brief Produce a `DataField` for an action in the table entry.
    /// @param param
    /// @return A `DataField`.
    virtual bfrt_proto::DataField produceDataField(const p4::config::v1::Action_Param &param);

    /// @brief Produce a `TableData` for an action in the table entry.
    /// @param action_refs action reference options where we will randomly pick one from.
    /// @param actions actions that containing `action_refs` for finding the action.
    /// @return A `TableData`.
    virtual bfrt_proto::TableData produceTableData(
        const google::protobuf::RepeatedPtrField<p4::config::v1::ActionRef> &action_refs,
        const google::protobuf::RepeatedPtrField<p4::config::v1::Action> &actions);

    /// @brief Produce a random `KeyField`.
    /// @param match The match field info.
    /// @return A `KeyField`
    virtual bfrt_proto::KeyField produceKeyField(const p4::config::v1::MatchField &match);

    /// @brief Produce a `TableEntry` for `table` with a randomly selected action.
    /// @param table
    /// @param actions
    /// @return A `TableEntry`.
    bfrt_proto::TableEntry produceTableEntry(
        const p4::config::v1::Table &table,
        const google::protobuf::RepeatedPtrField<p4::config::v1::Action> &actions);

    InitialConfig produceInitialConfig() override;

    UpdateSeries produceUpdateTimeSeries() override;
};

}  // namespace P4Tools::RTSmith::Tna

#endif /* BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_TOFINO_FUZZER_H_ */
