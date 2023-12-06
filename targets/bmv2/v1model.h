#ifndef BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_BMV2_V1MODEL_H_
#define BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_BMV2_V1MODEL_H_

#include "backends/p4tools/common/compiler/compiler_target.h"
#include "backends/p4tools/common/compiler/midend.h"
#include "frontends/common/options.h"

namespace P4Tools::RTSmith::V1Model {

class Bmv2V1ModelCompilerTarget : public CompilerTarget {
 public:
    /// Registers this target.
    static void make();

 private:
    [[nodiscard]] MidEnd mkMidEnd(const CompilerOptions &options) const override;

    Bmv2V1ModelCompilerTarget();
};

}  // namespace P4Tools::RTSmith::V1Model

#endif /* BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_BMV2_V1MODEL_H_ */
