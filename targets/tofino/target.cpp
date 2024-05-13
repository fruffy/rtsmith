#include "backends/p4tools/modules/p4rtsmith/targets/tofino/target.h"

#include "backends/p4tools/modules/p4rtsmith/targets/tofino/fuzzer.h"
#include "backends/p4tools/modules/p4rtsmith/targets/tofino/program_info.h"
#include "ir/ir.h"

namespace P4Tools::RTSmith::Tna {

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
    const CompilerResult &compilerResult, const IR::Declaration_Instance * /*mainDecl*/) const {
    return new TofinoTnaProgramInfo(compilerResult);
}

TofinoTnaFuzzer &TofinoTnaRtSmithTarget::getFuzzerImpl(const ProgramInfo &programInfo) const {
    return *new TofinoTnaFuzzer(*programInfo.checkedTo<TofinoTnaProgramInfo>());
}

}  // namespace P4Tools::RTSmith::Tna
