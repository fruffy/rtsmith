#ifndef BACKENDS_P4TOOLS_MODULES_P4RTSMITH_RTSMITH_H_
#define BACKENDS_P4TOOLS_MODULES_P4RTSMITH_RTSMITH_H_

#include "backends/p4tools/common/p4ctool.h"
#include "backends/p4tools/modules/p4rtsmith/options.h"
#include "ir/ir.h"

namespace P4Tools::RTSmith {

/// This is main implementation of the P4RuntimeSmith tool.
class RtSmith : public AbstractP4cTool<RtSmithOptions> {
 protected:
    void registerTarget() override;

    int mainImpl(const IR::P4Program *program) override;

 public:
    virtual ~RtSmith() = default;
};

}  // namespace P4Tools::RTSmith

#endif /* BACKENDS_P4TOOLS_MODULES_P4RTSMITH_RTSMITH_H_ */
