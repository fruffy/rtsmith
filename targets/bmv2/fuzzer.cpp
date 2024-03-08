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
    auto tableGenCnt = Utils::getRandInt(tableCnt);

    for (auto i = 0; i < tableGenCnt; i++) {
        auto tableId = Utils::getRandInt(tableCnt - 1);
        auto table = tables.Get(tableId);
        auto maxEntryGenCnt = table.size();

        p4::v1::Update update;
        update.set_type(p4::v1::Update_Type::Update_Type_INSERT);

        auto tableEntry = produceTableEntry(table, actions, maxEntryGenCnt);
        *update.mutable_entity()->mutable_table_entry() = tableEntry;
        *request.add_updates() = update;
    }

    // printInfo("Request:\n%1%", request.DebugString());
    std::vector<p4::v1::WriteRequest> requests{request};
    return requests;
}

P4RuntimeUpdateSeries Bmv2V1ModelFuzzer::produceUpdateTimeSeries() { return {}; }

}  // namespace P4Tools::RTSmith::V1Model
