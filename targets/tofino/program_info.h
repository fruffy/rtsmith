#ifndef BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_TOFINO_PROGRAM_INFO_H_
#define BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_TOFINO_PROGRAM_INFO_H_

#include "backends/p4tools/common/compiler/compiler_target.h"
#include "backends/p4tools/modules/p4rtsmith/core/program_info.h"
#include "ir/ir.h"
#include "ir/node.h"

namespace P4Tools::RTSmith::Tna {

class TofinoTnaProgramInfo : public ProgramInfo {
 public:
    explicit TofinoTnaProgramInfo(const CompilerResult &compilerResult);

    DECLARE_TYPEINFO(TofinoTnaProgramInfo);
};

}  // namespace P4Tools::RTSmith::Tna

#endif /* BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_TOFINO_PROGRAM_INFO_H_ */
