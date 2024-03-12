#ifndef BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_TOFINO_TARGET_H_
#define BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_TOFINO_TARGET_H_

#include "backends/p4tools/modules/p4rtsmith/core/program_info.h"
#include "backends/p4tools/modules/p4rtsmith/core/target.h"
#include "backends/p4tools/modules/p4rtsmith/targets/tofino/fuzzer.h"
#include "ir/ir.h"

namespace P4Tools::RTSmith::Tna {

class TofinoTnaRtSmithTarget : public RtSmithTarget {
 private:
    TofinoTnaRtSmithTarget();

 public:
    /// Registers this target.
    static void make();

 protected:
    const ProgramInfo *produceProgramInfoImpl(
        const CompilerResult &compilerResult,
        const IR::Declaration_Instance *mainDecl) const override;

    [[nodiscard]] TofinoTnaFuzzer &getFuzzerImpl(const ProgramInfo &programInfo) const override;
};

}  // namespace P4Tools::RTSmith::Tna

#endif /* BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_TOFINO_TARGET_H_ */
