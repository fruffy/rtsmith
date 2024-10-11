#include "backends/p4tools/modules/p4rtsmith/targets/tofino/target.h"

#include "backends/p4tools/modules/p4rtsmith/core/control_plane/protobuf_utils.h"
#include "backends/p4tools/modules/p4rtsmith/core/util.h"
#include "backends/p4tools/modules/p4rtsmith/options.h"
#include "backends/p4tools/modules/p4rtsmith/targets/tofino/fuzzer.h"
#include "backends/p4tools/modules/p4rtsmith/targets/tofino/program_info.h"
#include "ir/ir.h"
#include "lib/cstring.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wpedantic"
#include "p4/config/v1/p4info.pb.h"
#pragma GCC diagnostic pop

namespace P4::P4Tools::RTSmith::Tna {

/* =============================================================================================
 *  TofinoTnaRtSmithTarget implementation
 * ============================================================================================= */

TofinoTnaRtSmithTarget::TofinoTnaRtSmithTarget() : RtSmithTarget("tofino", "tna") {}

void TofinoTnaRtSmithTarget::make() {
    static TofinoTnaRtSmithTarget *INSTANCE = nullptr;
    if (INSTANCE == nullptr) {
        INSTANCE = new TofinoTnaRtSmithTarget();
    }
}

MidEnd TofinoTnaRtSmithTarget::mkMidEnd(const CompilerOptions &options) const {
    MidEnd midEnd(options);
    // Currently a no-op because we have all the necessary information from the front-end.
    midEnd.addPasses({});
    return midEnd;
}

const ProgramInfo *TofinoTnaRtSmithTarget::produceProgramInfoImpl(
    const CompilerResult &compilerResult, const RtSmithOptions &rtSmithOptions, const IR::Declaration_Instance * /*mainDecl*/) const {
    std::optional<P4::P4RuntimeAPI> p4runtimeApi;
    auto p4UserInfo = RtSmithOptions::get().userP4Info();
    if (p4UserInfo.has_value()) {
        ASSIGN_OR_RETURN(
            auto p4Info,
            Protobuf::deserializeObjectFromFile<p4::config::v1::P4Info>(p4UserInfo.value()),
            nullptr);
        p4runtimeApi = P4::P4RuntimeAPI(new p4::config::v1::P4Info(p4Info), nullptr);
    } else {
        /// After the front end, get the P4Runtime API for the V1model architecture.
        p4runtimeApi = P4::P4RuntimeSerializer::get()->generateP4Runtime(
            &compilerResult.getProgram(), cstring("tofino"));
        if (errorCount() > 0) {
            return nullptr;
        }
    }
    auto tofinoTnaProgramInfo = new TofinoTnaProgramInfo(compilerResult, p4runtimeApi.value());
    if (rtSmithOptions.fuzzerConfigPath().has_value())
        tofinoTnaProgramInfo->attmptToOverrideFuzzerConfigs(rtSmithOptions.fuzzerConfigPath().value().c_str());
    return tofinoTnaProgramInfo;
}

TofinoTnaFuzzer &TofinoTnaRtSmithTarget::getFuzzerImpl(const ProgramInfo &programInfo) const {
    return *new TofinoTnaFuzzer(*programInfo.checkedTo<TofinoTnaProgramInfo>());
}

}  // namespace P4::P4Tools::RTSmith::Tna
