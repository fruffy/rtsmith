#ifndef BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_TOFINO_TNA_H_
#define BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_TOFINO_TNA_H_

#include "backends/p4tools/common/compiler/compiler_target.h"
#include "backends/p4tools/common/compiler/midend.h"
#include "frontends/common/options.h"

namespace P4Tools::RTSmith::Tna {

class TofinoTnaCompilerTarget : public CompilerTarget {
 public:
    /// Registers this target.
    static void make();

 private:
    [[nodiscard]] MidEnd mkMidEnd(const CompilerOptions &options) const override;

    TofinoTnaCompilerTarget();
};

}  // namespace P4Tools::RTSmith::Tna

#endif /* BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_TOFINO_TNA_H_ */
