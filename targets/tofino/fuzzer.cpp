#include "backends/p4tools/modules/p4rtsmith/targets/tofino/fuzzer.h"

#include "backends/p4tools/common/lib/util.h"
#include "backends/p4tools/modules/p4rtsmith/core/fuzzer.h"
#include "control-plane/p4infoApi.h"

namespace P4Tools::RTSmith::Tna {

TofinoTnaFuzzer::TofinoTnaFuzzer(const TofinoTnaProgramInfo &programInfo)
    : P4RuntimeFuzzer(programInfo) {}

const TofinoTnaProgramInfo &TofinoTnaFuzzer::getProgramInfo() const {
    return *P4RuntimeFuzzer::getProgramInfo().checkedTo<TofinoTnaProgramInfo>();
}

InitialP4RuntimeConfig TofinoTnaFuzzer::produceInitialConfig() {
    p4::v1::WriteRequest request;

    auto p4Info = getProgramInfo().getP4RuntimeApi().p4Info;

    const auto tables = p4Info->tables();
    const auto actions = p4Info->actions();

    auto tableCnt = tables.size();
    auto tableGenCnt = Utils::getRandInt(tableCnt);

    for (auto i = 0; (uint64_t)i < tableGenCnt; i++) {
        auto tableId = Utils::getRandInt(tableCnt - 1);
        auto table = tables.Get(tableId);

        p4::v1::Update update;
        update.set_type(p4::v1::Update_Type::Update_Type_INSERT);

        auto tableEntry = produceTableEntry(table, actions);
        *update.mutable_entity()->mutable_table_entry() = tableEntry;
        *request.add_updates() = update;
    }

    std::vector<p4::v1::WriteRequest> requests{request};
    return requests;
}

P4RuntimeUpdateSeries TofinoTnaFuzzer::produceUpdateTimeSeries() { return {}; }

}  // namespace P4Tools::RTSmith::Tna
