#ifndef BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_BMV2_PROGRAM_INFO_H_
#define BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_BMV2_PROGRAM_INFO_H_

#include "backends/p4tools/modules/p4rtsmith/core/program_info.h"
#include "ir/ir.h"
#include "ir/node.h"

namespace P4Tools::RTSmith::V1Model {

class Bmv2V1ModelProgramInfo : public ProgramInfo {
 public:
    explicit Bmv2V1ModelProgramInfo(const IR::P4Program *program);

    DECLARE_TYPEINFO(Bmv2V1ModelProgramInfo);
};

}  // namespace P4Tools::RTSmith::V1Model

#endif /* BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_BMV2_PROGRAM_INFO_H_ */
