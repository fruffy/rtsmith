#include "backends/p4tools/modules/p4rtsmith/core/fuzzer.h"

#include "backends/p4tools/common/lib/util.h"
#include "control-plane/bytestrings.h"
#include "control-plane/p4infoApi.h"

namespace P4Tools::RTSmith {

p4::v1::FieldMatch_Exact P4RuntimeFuzzer::produceFieldMatch_Exact(int bitwidth) {
    p4::v1::FieldMatch_Exact protoExact;
    protoExact.set_value(produceBytes(bitwidth));
    return protoExact;
}

p4::v1::FieldMatch_LPM P4RuntimeFuzzer::produceFieldMatch_LPM(int bitwidth) {
    p4::v1::FieldMatch_LPM protoLPM;
    protoLPM.set_value(produceBytes(bitwidth));
    protoLPM.set_prefix_len(Utils::getRandInt(0, bitwidth));
    return protoLPM;
}

p4::v1::FieldMatch_Ternary P4RuntimeFuzzer::produceFieldMatch_Ternary(int bitwidth) {
    p4::v1::FieldMatch_Ternary protoTernary;
    protoTernary.set_value(produceBytes(bitwidth));
    /// TODO: use random mask
    protoTernary.set_mask(produceBytes(bitwidth, /*value=*/0));
    return protoTernary;
}

p4::v1::FieldMatch_Range P4RuntimeFuzzer::produceFieldMatch_Range(int bitwidth) {
    p4::v1::FieldMatch_Range protoRange;
    const auto &highValue = Utils::getRandConstantForWidth(bitwidth)->value;
    auto low = produceBytes(bitwidth, /*min=*/0, /*max=*/highValue);
    auto high = checkBigIntToString(highValue, bitwidth);
    protoRange.set_low(low);
    protoRange.set_high(high);
    return protoRange;
}

p4::v1::FieldMatch_Optional P4RuntimeFuzzer::produceFieldMatch_Optional(int bitwidth) {
    p4::v1::FieldMatch_Optional protoOptional;
    protoOptional.set_value(produceBytes(bitwidth));
    return protoOptional;
}

p4::v1::Action_Param P4RuntimeFuzzer::produceActionParam(
    const p4::config::v1::Action_Param &param) {
    p4::v1::Action_Param protoParam;
    protoParam.set_param_id(param.id());
    protoParam.set_value(produceBytes(param.bitwidth()));
    return protoParam;
}

p4::v1::Action P4RuntimeFuzzer::produceTableAction(
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
    for (auto &param : action->params()) {
        protoAction.add_params()->CopyFrom(produceActionParam(param));
    }

    return protoAction;
}

uint32_t P4RuntimeFuzzer::producePriority(
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

p4::v1::FieldMatch P4RuntimeFuzzer::produceMatchField(p4::config::v1::MatchField &match) {
    p4::v1::FieldMatch protoMatch;
    protoMatch.set_field_id(match.id());

    auto matchType = match.match_type();
    auto bitwidth = match.bitwidth();

    switch (matchType) {
        case p4::config::v1::MatchField::EXACT:
            protoMatch.mutable_exact()->CopyFrom(produceFieldMatch_Exact(bitwidth));
            break;
        case p4::config::v1::MatchField::LPM:
            protoMatch.mutable_lpm()->CopyFrom(produceFieldMatch_LPM(bitwidth));
            break;
        case p4::config::v1::MatchField::TERNARY:
            protoMatch.mutable_ternary()->CopyFrom(produceFieldMatch_Ternary(bitwidth));
            break;
        case p4::config::v1::MatchField::RANGE:
            protoMatch.mutable_range()->CopyFrom(produceFieldMatch_Range(bitwidth));
            break;
        case p4::config::v1::MatchField::OPTIONAL:
            protoMatch.mutable_optional()->CopyFrom(produceFieldMatch_Optional(bitwidth));
            break;
        default:
            P4C_UNIMPLEMENTED("Match type %1% not supported for P4RuntimeFuzzer yet",
                              p4::config::v1::MatchField::MatchType_Name(matchType));
    }
    return protoMatch;
}

p4::v1::TableEntry P4RuntimeFuzzer::produceTableEntry(
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
        /// TODO: add this back once Flay supports OPTIONAL.
        if (match.match_type() == p4::config::v1::MatchField::OPTIONAL) {
            continue;
        }
        protoEntry.add_match()->CopyFrom(produceMatchField(match));
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

/// Some Helper functions below

std::string RuntimeFuzzer::checkBigIntToString(const big_int &value, int bitwidth) {
    std::optional<std::string> valueStr = P4::ControlPlaneAPI::stringReprConstant(value, bitwidth);
    BUG_CHECK(valueStr.has_value(), "Failed to check %1% to string, maybe value < 0?", value.str());
    return valueStr.value();
}

std::string RuntimeFuzzer::produceBytes(int bitwidth) {
    auto value = Utils::getRandConstantForWidth(bitwidth)->value;
    return checkBigIntToString(value, bitwidth);
}

std::string RuntimeFuzzer::produceBytes(int bitwidth, const big_int &value) {
    return checkBigIntToString(value, bitwidth);
}

std::string RuntimeFuzzer::produceBytes(int bitwidth, const big_int &min, const big_int &max) {
    auto value = Utils::getRandBigInt(min, max);
    return checkBigIntToString(value, bitwidth);
}

bool RuntimeFuzzer::tableHasFieldType(const p4::config::v1::Table &table,
                                      const p4::config::v1::MatchField::MatchType type) {
    for (const auto &match : table.match_fields()) {
        if (match.match_type() == type) {
            return true;
        }
    }
    return false;
}

}  // namespace P4Tools::RTSmith
