#ifndef BACKENDS_P4TOOLS_MODULES_P4RTSMITH_CORE_FUZZER_H_
#define BACKENDS_P4TOOLS_MODULES_P4RTSMITH_CORE_FUZZER_H_

#include "backends/p4tools/modules/p4rtsmith/core/program_info.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wpedantic"
#include "backends/p4tools/modules/p4rtsmith/core/control_plane/bfruntime/bfruntime.pb.h"
#include "p4/v1/p4runtime.pb.h"
#pragma GCC diagnostic pop

namespace P4Tools::RTSmith {

enum class FuzzerType { P4Runtime, BFRuntime };

using ProtobufMessagePtr = std::shared_ptr<google::protobuf::Message>;
using InitialConfig = std::vector<ProtobufMessagePtr>;
using UpdateSeries = std::vector<std::pair<uint64_t, ProtobufMessagePtr>>;

class RuntimeFuzzer {
 private:
    /// fuzzer type to distinguish between P4Runtime and BFRuntime fuzzers.
    FuzzerType fuzzerType;

    /// The program info of the target.
    std::reference_wrapper<const ProgramInfo> programInfo;

 protected:
    /// @returns the program info associated with the current target.
    [[nodiscard]] virtual const ProgramInfo &getProgramInfo() const { return programInfo; }

 public:
    explicit RuntimeFuzzer(const FuzzerType fuzzerType, const ProgramInfo &programInfo)
        : fuzzerType(fuzzerType), programInfo(programInfo) {}

    RuntimeFuzzer(const RuntimeFuzzer &) = default;
    RuntimeFuzzer(RuntimeFuzzer &&) = delete;
    RuntimeFuzzer &operator=(const RuntimeFuzzer &) = default;
    RuntimeFuzzer &operator=(RuntimeFuzzer &&) = delete;
    virtual ~RuntimeFuzzer() = default;

    [[nodiscard]] FuzzerType getFuzzerType() const { return fuzzerType; }

    /// @brief Produce an `InitialConfig`, which is a vector of updates.
    /// @return A InitialConfig
    virtual InitialConfig produceInitialConfig() = 0;

    /// @brief Produce an `UpdateSeries`, which is a vector of indexed updates.
    /// @return A InitialConfig
    virtual UpdateSeries produceUpdateTimeSeries() = 0;
};

class P4RuntimeFuzzer : public RuntimeFuzzer {
 public:
    explicit P4RuntimeFuzzer(const ProgramInfo &programInfo)
        : RuntimeFuzzer(FuzzerType::P4Runtime, programInfo) {}

    P4RuntimeFuzzer(const P4RuntimeFuzzer &) = default;
    P4RuntimeFuzzer(P4RuntimeFuzzer &&) = delete;
    P4RuntimeFuzzer &operator=(const P4RuntimeFuzzer &) = default;
    P4RuntimeFuzzer &operator=(P4RuntimeFuzzer &&) = delete;
    virtual ~P4RuntimeFuzzer() = default;

    /// @brief Produce a FieldMatch_Exact with bitwidth
    /// @param bitwidth
    /// @return A FieldMatch_Exact
    virtual p4::v1::FieldMatch_Exact produceFieldMatch_Exact(int bitwidth);

    /// @brief Produce a FieldMatch_LPM with bitwidth
    /// @param bitwidth
    /// @return A FieldMatch_LPM
    virtual p4::v1::FieldMatch_LPM produceFieldMatch_LPM(int bitwidth);

    /// @brief Produce a FieldMatch_Ternary with bitwidth
    /// @param bitwidth
    /// @return A FieldMatch_Ternary
    virtual p4::v1::FieldMatch_Ternary produceFieldMatch_Ternary(int bitwidth);

    /// @brief Produce a FieldMatch_Optional with bitwidth
    /// @param bitwidth
    /// @return A FieldMatch_Optional
    virtual p4::v1::FieldMatch_Optional produceFieldMatch_Optional(int bitwidth);

    /// @brief Produce a param for an action in the table entry
    /// @param param
    /// @return An action param
    virtual p4::v1::Action_Param produceActionParam(const p4::config::v1::Action_Param &param);

    /// @brief Produce a random action selected for a table entry
    /// @param action_refs action reference options where we will randomly pick one from.
    /// @param actions actions that containing `action_refs` for finding the action.
    /// @return An `Action`
    virtual p4::v1::Action produceTableAction(
        const google::protobuf::RepeatedPtrField<p4::config::v1::ActionRef> &action_refs,
        const google::protobuf::RepeatedPtrField<p4::config::v1::Action> &actions);

    /// @brief Produce priority for an entry given match fields
    /// @param matchFields
    /// @return A 32-bit integer
    virtual uint32_t producePriority(
        const google::protobuf::RepeatedPtrField<p4::config::v1::MatchField> &matchFields);

    /// @brief Produce match field given match type
    /// @param match
    /// @return A `FieldMatch`
    virtual p4::v1::FieldMatch produceMatchField(p4::config::v1::MatchField &match);

    /// @brief Produce a `TableEntry` with id, match fields, priority and action
    /// @param table
    /// @param actions
    /// @return A `TableEntry`
    virtual p4::v1::TableEntry produceTableEntry(
        const p4::config::v1::Table &table,
        const google::protobuf::RepeatedPtrField<p4::config::v1::Action> &actions);
};

class BFRuntimeFuzzer : public RuntimeFuzzer {
 public:
    explicit BFRuntimeFuzzer(const ProgramInfo &programInfo)
        : RuntimeFuzzer(FuzzerType::BFRuntime, programInfo) {}

    BFRuntimeFuzzer(const BFRuntimeFuzzer &) = default;
    BFRuntimeFuzzer(BFRuntimeFuzzer &&) = delete;
    BFRuntimeFuzzer &operator=(const BFRuntimeFuzzer &) = default;
    BFRuntimeFuzzer &operator=(BFRuntimeFuzzer &&) = delete;
    virtual ~BFRuntimeFuzzer() = default;

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
    virtual bfrt_proto::TableEntry produceTableEntry(
        const p4::config::v1::Table &table,
        const google::protobuf::RepeatedPtrField<p4::config::v1::Action> &actions);
};

}  // namespace P4Tools::RTSmith

#endif /* BACKENDS_P4TOOLS_MODULES_P4RTSMITH_CORE_FUZZER_H_ */
