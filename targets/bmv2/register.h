#ifndef BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_BMV2_REGISTER_H_
#define BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_BMV2_REGISTER_H_

#include "backends/p4tools/modules/p4rtsmith/targets/bmv2/target.h"
#include "backends/p4tools/modules/p4rtsmith/targets/bmv2/v1model.h"

namespace P4Tools::RTSmith {

/// Register the V1Model compiler target with the tools framework.
inline void bmv2_registerCompilerTarget() { V1Model::Bmv2V1ModelCompilerTarget::make(); }

/// Register the V1Model RtSmith target with the P4RuntimeSmith framework.
inline void bmv2_registerRtSmithTarget() { V1Model::Bmv2V1ModelRtSmithTarget::make(); }

}  // namespace P4Tools::RTSmith

#endif /* BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_BMV2_REGISTER_H_ */
