#include "backends/p4tools/modules/p4rtsmith/targets/bmv2/fuzzer.h"
#include "backends/p4tools/common/lib/util.h"
#include "control-plane/bytestrings.h"

namespace P4Tools::RTSmith::V1Model {

Bmv2V1ModelFuzzer::Bmv2V1ModelFuzzer(const Bmv2V1ModelProgramInfo &programInfo)
    : P4RuntimeFuzzer(programInfo) {}

const Bmv2V1ModelProgramInfo &Bmv2V1ModelFuzzer::getProgramInfo() const {
    return *P4RuntimeFuzzer::getProgramInfo().checkedTo<Bmv2V1ModelProgramInfo>();
}

void addTernary(p4::v1::TableEntry *protoEntry, int fieldId, int bitwidth) {
    auto value = Utils::getRandConstantForWidth(bitwidth)->value;
    std::optional<std::string> valueStr = P4::ControlPlaneAPI::stringReprConstant(value, bitwidth);
    std::optional<std::string> maskStr = P4::ControlPlaneAPI::stringReprConstant(0, bitwidth);

    auto protoMatch = protoEntry->add_match();
    protoMatch->set_field_id(fieldId);
    auto protoTernary = protoMatch->mutable_ternary();
    protoTernary->set_value(*valueStr);
    protoTernary->set_mask(*maskStr);
}

void addActions(p4::v1::TableEntry *tableEntry, const p4::config::v1::Table *table, google::protobuf::RepeatedPtrField<p4::config::v1::Action> *actions) {
    const auto &action_refs = table->action_refs();
    auto action_index = Utils::getRandInt(action_refs.size()-1);

    auto protoAction = tableEntry->mutable_action()->mutable_action();
    auto action_ref_id = action_refs[action_index].id();
    protoAction->set_action_id(action_ref_id);

    for (auto &action : *actions) {
        auto action_id = action.preamble().id();
        if (action_id == action_ref_id) {
            auto params = action.params();

            if (!params.empty()) {
                int parameterId = 1;
                for (auto &param : params) {
                    auto protoParam = protoAction->add_params();
                    auto paramBitWidth = param.bitwidth();
                    auto value = Utils::getRandConstantForWidth(paramBitWidth)->value;
                    std::optional<std::string> valueStr = P4::ControlPlaneAPI::stringReprConstant(value, paramBitWidth);

                    protoParam->set_param_id(parameterId++); 
                    protoParam->set_value(*valueStr);
                }
            }
        }
    }
}

void addMatchFields(p4::v1::TableEntry *tableEntry, const p4::config::v1::Table *table) {
    const auto &matchFields = table->match_fields();
    int fieldId = 1;
    bool setPriority = false;

    for (auto &match : matchFields) {
        auto matchType = match.match_type();
        auto bitwidth = match.bitwidth();
        if (matchType == p4::config::v1::MatchField::EXACT) {
            // TODO: addExact(tableEntry, fieldId++, k, keyWidth, typeMap);
        } else if (matchType == p4::config::v1::MatchField::LPM) {
            // TODO: addLpm(tableEntry, fieldId++, k, keyWidth, typeMap);
        } else if (matchType == p4::config::v1::MatchField::TERNARY) {
            addTernary(tableEntry, fieldId++, bitwidth);
            setPriority = true;
        } else if (matchType == p4::config::v1::MatchField::RANGE) {
            // TODO: addRange(tableEntry, fieldId++, k, keyWidth, typeMap);
            setPriority = true;
        } else if (matchType == p4::config::v1::MatchField::OPTIONAL) {
            // TODO: addOptional(tableEntry, fieldId++, k, keyWidth, typeMap);
            setPriority = true;
        }
    }

    if (setPriority) {
        auto value = Utils::getRandConstantForWidth(32)->value;
        auto priority = static_cast<int>(value);
        tableEntry->set_priority(priority);
    }
}

InitialP4RuntimeConfig Bmv2V1ModelFuzzer::produceInitialConfig() { 
    p4::v1::WriteRequest request;

    auto p4Info = getProgramInfo().getP4RuntimeApi().p4Info;

    auto tables = p4Info->tables();
    auto actions = p4Info->actions();

    for (auto &table : tables) {
        p4::v1::TableEntry tableEntry;

        const auto &pre_t = table.preamble();
        tableEntry.set_table_id(pre_t.id());

        addMatchFields(&tableEntry, &table);
        addActions(&tableEntry, &table, &actions);

        *request.add_updates()->mutable_entity()->mutable_table_entry() = tableEntry;
    }

    // printInfo("Request:\n%1%", request.DebugString());
    std::vector<p4::v1::WriteRequest> requests { request };
    return requests; 
}

P4RuntimeUpdateSeries Bmv2V1ModelFuzzer::produceUpdateTimeSeries() { return {}; }

}  // namespace P4Tools::RTSmith::V1Model
