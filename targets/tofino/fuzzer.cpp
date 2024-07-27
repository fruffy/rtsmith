#include "backends/p4tools/modules/p4rtsmith/targets/tofino/fuzzer.h"

#include <algorithm>

#include "backends/p4tools/common/lib/util.h"
#include "backends/p4tools/modules/p4rtsmith/core/fuzzer.h"
#include "control-plane/p4infoApi.h"

namespace P4Tools::RTSmith::Tna {

TofinoTnaFuzzer::TofinoTnaFuzzer(const TofinoTnaProgramInfo &programInfo)
    : BFRuntimeFuzzer(programInfo) {}

const TofinoTnaProgramInfo &TofinoTnaFuzzer::getProgramInfo() const {
    return *BFRuntimeFuzzer::getProgramInfo().checkedTo<TofinoTnaProgramInfo>();
}

bfrt_proto::TableEntry TofinoTnaFuzzer::produceTableEntry(
    const p4::config::v1::Table &table,
    const google::protobuf::RepeatedPtrField<p4::config::v1::Action> &actions) {
    /// TODO: Special processing for things like selector, profile in Tofino.
    return BFRuntimeFuzzer::produceTableEntry(table, actions);
}

InitialConfig TofinoTnaFuzzer::produceInitialConfig() {
    auto request = std::make_unique<bfrt_proto::WriteRequest>();

    auto p4Info = getProgramInfo().getP4RuntimeApi().p4Info;

    /// TODO: for Tofino, we also need to look at externs instances for
    /// ActionSelector, ActionProfile and so on.
    const auto tables = p4Info->tables();
    const auto actions = p4Info->actions();

    auto tableCnt = tables.size();

    for (auto tableId = 0; tableId < tableCnt; tableId++) {
        /// NOTE: temporary use a coin to decide if generating entries for the table
        if (Utils::getRandInt(0, 1) == 0) {
            continue;
        }
        auto table = tables.Get(tableId);
        /// TODO: remove this `min`. It is for ease of debugging now.
        auto maxEntryGenCnt = std::min(table.size(), (int64_t)2);
        for (auto i = 0; i < maxEntryGenCnt; i++) {
            auto update = request->add_updates();
            /// TODO: add support for other types.
            update->set_type(bfrt_proto::Update_Type::Update_Type_INSERT);
            update->mutable_entity()->mutable_table_entry()->CopyFrom(
                produceTableEntry(table, actions));
        }
    }

    InitialConfig initialConfig;
    initialConfig.push_back(std::move(request));
    return initialConfig;
}

UpdateSeries TofinoTnaFuzzer::produceUpdateTimeSeries() { return {}; }

}  // namespace P4Tools::RTSmith::Tna
