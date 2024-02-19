#include "backends/p4tools/modules/p4rtsmith/targets/bmv2/fuzzer.h"

#include "backends/p4tools/common/lib/util.h"
#include "control-plane/bytestrings.h"
#include "control-plane/p4infoApi.h"

namespace P4Tools::RTSmith::V1Model {

Bmv2V1ModelFuzzer::Bmv2V1ModelFuzzer(const Bmv2V1ModelProgramInfo &programInfo)
    : P4RuntimeFuzzer(programInfo) {}

const Bmv2V1ModelProgramInfo &Bmv2V1ModelFuzzer::getProgramInfo() const {
    return *P4RuntimeFuzzer::getProgramInfo().checkedTo<Bmv2V1ModelProgramInfo>();
}

std::optional<p4::v1::FieldMatch> produceTernaryMatch(int fieldId, int bitwidth) {
    p4::v1::FieldMatch protoMatch;

    auto value = Utils::getRandConstantForWidth(bitwidth)->value;
    std::optional<std::string> valueStr = P4::ControlPlaneAPI::stringReprConstant(value, bitwidth);
    std::optional<std::string> maskStr = P4::ControlPlaneAPI::stringReprConstant(0, bitwidth);
    if (valueStr == std::nullopt || maskStr == std::nullopt) return std::nullopt;

    protoMatch.set_field_id(fieldId);
    auto protoTernary = protoMatch.mutable_ternary();
    protoTernary->set_value(*valueStr);
    protoTernary->set_mask(*maskStr);

    return protoMatch;
}

std::optional<p4::v1::FieldMatch> produceExactMatch(int fieldId, int bitwidth) {
    p4::v1::FieldMatch protoMatch;

    auto value = Utils::getRandConstantForWidth(bitwidth)->value;
    std::optional<std::string> valueStr = P4::ControlPlaneAPI::stringReprConstant(value, bitwidth);
    if (valueStr == std::nullopt) return std::nullopt;

    protoMatch.set_field_id(fieldId);
    auto protoExact = protoMatch.mutable_exact();
    protoExact->set_value(*valueStr);

    return protoMatch;
}

p4::v1::Action_Param produceActionParam(p4::config::v1::Action_Param &param, int parameterId) {
    p4::v1::Action_Param protoParam;

    auto paramBitWidth = param.bitwidth();
    auto value = Utils::getRandConstantForWidth(paramBitWidth)->value;
    std::optional<std::string> valueStr =
        P4::ControlPlaneAPI::stringReprConstant(value, paramBitWidth);
    protoParam.set_param_id(parameterId);
    protoParam.set_value(*valueStr);

    return protoParam;
}

p4::v1::Action produceTableAction(
    const google::protobuf::RepeatedPtrField<p4::config::v1::ActionRef> &action_refs,
    const google::protobuf::RepeatedPtrField<p4::config::v1::Action> &actions) {
    p4::v1::Action protoAction;

    auto action_index = Utils::getRandInt(action_refs.size() - 1);
    auto action_ref_id = action_refs[action_index].id();

    auto action =
        P4::ControlPlaneAPI::findP4InfoObject(actions.begin(), actions.end(), action_ref_id);
    auto action_id = action->preamble().id();
    auto params = action->params();

    protoAction.set_action_id(action_id);
    for (auto i = 0; i < params.size(); i++) {
        auto param = params[i];
        auto protoParam = produceActionParam(param, ++i);
        *protoAction.add_params() = protoParam;
    }

    return protoAction;
}

uint32_t producePriority(
    const google::protobuf::RepeatedPtrField<p4::config::v1::MatchField> &matchFields) {
    for (auto i = 0; i < matchFields.size(); i++) {
        auto match = matchFields[i];
        auto matchType = match.match_type();
        if (matchType == p4::config::v1::MatchField::TERNARY ||
            matchType == p4::config::v1::MatchField::RANGE ||
            matchType == p4::config::v1::MatchField::OPTIONAL) {
            auto value = Utils::getRandConstantForWidth(32)->value;
            auto priority = static_cast<int>(value);
            return priority;
        }
    }

    return 0;
}

std::optional<p4::v1::FieldMatch> produceMatchField(p4::config::v1::MatchField &match,
                                                    int fieldId) {
    auto matchType = match.match_type();
    auto bitwidth = match.bitwidth();

    if (matchType == p4::config::v1::MatchField::EXACT) {
        return produceExactMatch(fieldId, bitwidth);
    } else if (matchType == p4::config::v1::MatchField::LPM) {
        // TODO: addLpm(tableEntry, fieldId++, k, keyWidth, typeMap);
    } else if (matchType == p4::config::v1::MatchField::TERNARY) {
        return produceTernaryMatch(fieldId, bitwidth);
    } else if (matchType == p4::config::v1::MatchField::RANGE) {
        // TODO: addRange(tableEntry, fieldId++, k, keyWidth, typeMap);
    } else if (matchType == p4::config::v1::MatchField::OPTIONAL) {
        // TODO: addOptional(tableEntry, fieldId++, k, keyWidth, typeMap);
    }

    return std::nullopt;
}

p4::v1::TableEntry produceTableEntry(
    const p4::config::v1::Table &table,
    const google::protobuf::RepeatedPtrField<p4::config::v1::Action> &actions) {
    p4::v1::TableEntry protoEntry;

    // set table id
    const auto &pre_t = table.preamble();
    protoEntry.set_table_id(pre_t.id());

    // add matches
    const auto &matchFields = table.match_fields();
    for (auto i = 0; i < matchFields.size(); i++) {
        auto match = matchFields[i];
        auto protoMatch = produceMatchField(match, ++i);
        *protoEntry.add_match() = *protoMatch;
    }

    // set priority
    auto priority = producePriority(matchFields);
    protoEntry.set_priority(priority);

    // add action
    const auto &action_refs = table.action_refs();
    auto protoAction = produceTableAction(action_refs, actions);
    *protoEntry.mutable_action()->mutable_action() = protoAction;

    return protoEntry;
}

InitialP4RuntimeConfig Bmv2V1ModelFuzzer::produceInitialConfig() {
    p4::v1::WriteRequest request;

    auto p4Info = getProgramInfo().getP4RuntimeApi().p4Info;

    const auto tables = p4Info->tables();
    const auto actions = p4Info->actions();

    for (auto &table : tables) {
        auto tableEntry = produceTableEntry(table, actions);
        *request.add_updates()->mutable_entity()->mutable_table_entry() = tableEntry;
    }

    // printInfo("Request:\n%1%", request.DebugString());
    std::vector<p4::v1::WriteRequest> requests{request};
    return requests;
}

P4RuntimeUpdateSeries Bmv2V1ModelFuzzer::produceUpdateTimeSeries() { return {}; }

}  // namespace P4Tools::RTSmith::V1Model
