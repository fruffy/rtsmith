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

InitialP4RuntimeConfig Bmv2V1ModelFuzzer::produceInitialConfig() {
    p4::v1::WriteRequest request;

    auto p4Info = getProgramInfo().getP4RuntimeApi().p4Info;

    const auto tables = p4Info->tables();
    const auto actions = p4Info->actions();

    auto tableCnt = tables.size();

    for (auto tableId = 0; tableId < tableCnt; tableId++) {
        // NOTE: temporary use a coin to decide if generating entries for the table
        if (Utils::getRandInt(0, 1) == 0) {
            continue;
        }
        auto table = tables.Get(tableId);
        auto maxEntryGenCnt = table.size();
        for (auto i = 0; i < maxEntryGenCnt; i++) {
            p4::v1::Update update;
            update.set_type(p4::v1::Update_Type::Update_Type_INSERT);

            auto tableEntry = produceTableEntry(table, actions);
            *update.mutable_entity()->mutable_table_entry() = tableEntry;
            *request.add_updates() = update;
        }
    }

    std::vector<p4::v1::WriteRequest> requests{request};
    return requests;
}

P4RuntimeUpdateSeries Bmv2V1ModelFuzzer::produceUpdateTimeSeries() { return {}; }

}  // namespace P4Tools::RTSmith::V1Model
