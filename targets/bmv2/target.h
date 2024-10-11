#ifndef BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_BMV2_TARGET_H_
#define BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_BMV2_TARGET_H_

#include "backends/p4tools/modules/p4rtsmith/core/program_info.h"
#include "backends/p4tools/modules/p4rtsmith/core/target.h"
#include "backends/p4tools/modules/p4rtsmith/targets/bmv2/fuzzer.h"
#include "ir/ir.h"

namespace P4::P4Tools::RTSmith::V1Model {

class Bmv2V1ModelRtSmithTarget : public RtSmithTarget {
 private:
    Bmv2V1ModelRtSmithTarget();

 public:
    /// Registers this target.
    static void make();

 protected:
    ProgramInfo *produceProgramInfoImpl(const CompilerResult &compilerResult,
                                        const IR::Declaration_Instance *mainDecl) const override;

    [[nodiscard]] Bmv2V1ModelFuzzer &getFuzzerImpl(const ProgramInfo &programInfo) const override;

    [[nodiscard]] MidEnd mkMidEnd(const CompilerOptions &options) const override;
};

}  // namespace P4::P4Tools::RTSmith::V1Model

#endif /* BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_BMV2_TARGET_H_ */
