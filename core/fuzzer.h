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
    const FuzzerType fuzzerType;

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

    virtual InitialConfig produceInitialConfig() = 0;

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

    virtual p4::v1::FieldMatch_Exact produceFieldMatch_Exact(int bitwidth);

    virtual p4::v1::FieldMatch_LPM produceFieldMatch_LPM(int bitwidth);

    virtual p4::v1::FieldMatch_Ternary produceFieldMatch_Ternary(int bitwidth);

    virtual p4::v1::FieldMatch_Optional produceFieldMatch_Optional(int bitwidth);

    virtual p4::v1::Action_Param produceActionParam(const p4::config::v1::Action_Param &param,
                                                    int parameterId);

    virtual p4::v1::Action produceTableAction(
        const google::protobuf::RepeatedPtrField<p4::config::v1::ActionRef> &action_refs,
        const google::protobuf::RepeatedPtrField<p4::config::v1::Action> &actions);

    virtual uint32_t producePriority(
        const google::protobuf::RepeatedPtrField<p4::config::v1::MatchField> &matchFields);

    virtual p4::v1::FieldMatch produceMatchField(p4::config::v1::MatchField &match, int fieldId);
    virtual p4::v1::TableEntry produceTableEntry(
        const p4::config::v1::Table &table,
        const google::protobuf::RepeatedPtrField<p4::config::v1::Action> &actions);

    virtual InitialConfig produceInitialConfig() = 0;

    virtual UpdateSeries produceUpdateTimeSeries() = 0;
};

class BFRuntimeInitialConfig : public InitialConfig {
 public:
    BFRuntimeInitialConfig() = default;
    std::vector<bfrt_proto::WriteRequest> requests;
};

class BFRuntimeUpdateSeries : public UpdateSeries {
    std::vector<std::pair<uint64_t, bfrt_proto::WriteRequest>> updates;
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

    virtual bfrt_proto::KeyField_Exact produceKeyField_Exact(int bitwidth);

    virtual bfrt_proto::KeyField_LPM produceKeyField_LPM(int bitwidth);

    virtual bfrt_proto::KeyField_Ternary produceKeyField_Ternary(int bitwidth);

    virtual bfrt_proto::KeyField_Optional produceKeyField_Optional(int bitwidth);

    virtual bfrt_proto::DataField produceDataField(const p4::config::v1::Action_Param &param,
                                                   int parameterId);

    virtual bfrt_proto::TableData produceTableData(
        const google::protobuf::RepeatedPtrField<p4::config::v1::ActionRef> &action_refs,
        const google::protobuf::RepeatedPtrField<p4::config::v1::Action> &actions);

    virtual bfrt_proto::KeyField produceKeyField(const p4::config::v1::MatchField &match,
                                                 int fieldId);

    virtual bfrt_proto::TableEntry produceTableEntry(
        const p4::config::v1::Table &table,
        const google::protobuf::RepeatedPtrField<p4::config::v1::Action> &actions);

    virtual InitialConfig produceInitialConfig() = 0;

    virtual UpdateSeries produceUpdateTimeSeries() = 0;
};

}  // namespace P4Tools::RTSmith

#endif /* BACKENDS_P4TOOLS_MODULES_P4RTSMITH_CORE_FUZZER_H_ */
