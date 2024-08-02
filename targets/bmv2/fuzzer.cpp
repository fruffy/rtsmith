#include "backends/p4tools/modules/p4rtsmith/targets/bmv2/fuzzer.h"

#include "backends/p4tools/common/lib/util.h"
#include "backends/p4tools/modules/p4rtsmith/core/fuzzer.h"
#include "control-plane/p4infoApi.h"

namespace P4Tools::RTSmith::V1Model {

Bmv2V1ModelFuzzer::Bmv2V1ModelFuzzer(const Bmv2V1ModelProgramInfo &programInfo)
    : P4RuntimeFuzzer(programInfo) {}

const Bmv2V1ModelProgramInfo &Bmv2V1ModelFuzzer::getProgramInfo() const {
    return *P4RuntimeFuzzer::getProgramInfo().checkedTo<Bmv2V1ModelProgramInfo>();
}

InitialConfig Bmv2V1ModelFuzzer::produceInitialConfig() {
    auto request = std::make_unique<p4::v1::WriteRequest>();

    auto p4Info = getProgramInfo().getP4RuntimeApi().p4Info;

    const auto tables = p4Info->tables();
    const auto actions = p4Info->actions();

    auto tableCnt = tables.size();

    for (auto tableId = 0; tableId < tableCnt; tableId++) {
        /// NOTE: temporary use a coin to decide if generating entries for the table
        if (Utils::getRandInt(0, 1) == 0) {
            continue;
        }
        auto table = tables.Get(tableId);
        if (table.match_fields_size() == 0 || table.is_const_table()) {
            continue;
        }
        /// TODO: Remove this when Flay supports RANGE.
        if (tableHasFieldType(table, p4::config::v1::MatchField::RANGE)) {
            continue;
        }
        /// TODO: remove this `min`. It is for ease of debugging now.
        auto maxEntryGenCnt = std::min(table.size(), (int64_t)4);
        std::set<std::string> matchFields;
        for (auto i = 0; i < maxEntryGenCnt; i++) {
            auto entry = produceTableEntry(table, actions);
            std::string matchFieldString;
            for (const auto &match : entry.match()) {
                match.AppendPartialToString(&matchFieldString);
            }
            if (matchFields.find(matchFieldString) == matchFields.end()) {
                /// Only insert unique entries
                auto update = request->add_updates();
                update->set_type(p4::v1::Update_Type::Update_Type_INSERT);
                matchFields.insert(std::move(matchFieldString));
                update->mutable_entity()->mutable_table_entry()->CopyFrom(entry);
            }
        }
    }

    InitialConfig initialConfig;
    initialConfig.push_back(std::move(request));
    return initialConfig;
}

UpdateSeries Bmv2V1ModelFuzzer::produceUpdateTimeSeries() { return {}; }

}  // namespace P4Tools::RTSmith::V1Model
