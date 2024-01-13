#include "backends/p4tools/modules/p4rtsmith/targets/bmv2/target.h"

#include "backends/p4tools/modules/p4rtsmith/targets/bmv2/fuzzer.h"
#include "backends/p4tools/modules/p4rtsmith/targets/bmv2/program_info.h"
#include "ir/ir.h"

namespace P4Tools::RTSmith::V1Model {

/* =============================================================================================
 *  Bmv2V1ModelRtSmithTarget implementation
 * ============================================================================================= */

Bmv2V1ModelRtSmithTarget::Bmv2V1ModelRtSmithTarget() : RtSmithTarget("bmv2", "v1model") {}

void Bmv2V1ModelRtSmithTarget::make() {
    static Bmv2V1ModelRtSmithTarget *INSTANCE = nullptr;
    if (INSTANCE == nullptr) {
        INSTANCE = new Bmv2V1ModelRtSmithTarget();
    }
}

const ProgramInfo *Bmv2V1ModelRtSmithTarget::produceProgramInfoImpl(
    const IR::P4Program *program, const IR::Declaration_Instance * /*mainDecl*/) const {
    return new Bmv2V1ModelProgramInfo(program);
}

Bmv2V1ModelFuzzer &Bmv2V1ModelRtSmithTarget::getFuzzerImpl(const ProgramInfo &programInfo) const {
    return *new Bmv2V1ModelFuzzer(*programInfo.checkedTo<Bmv2V1ModelProgramInfo>());
}

}  // namespace P4Tools::RTSmith::V1Model
