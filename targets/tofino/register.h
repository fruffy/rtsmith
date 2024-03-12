#ifndef BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_TOFINO_REGISTER_H_
#define BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_TOFINO_REGISTER_H_

#include "backends/p4tools/modules/p4rtsmith/targets/tofino/target.h"
#include "backends/p4tools/modules/p4rtsmith/targets/tofino/tna.h"

namespace P4Tools::RTSmith {

/// Register the Tna compiler target with the tools framework.
inline void tofino_registerCompilerTarget() { Tna::TofinoTnaCompilerTarget::make(); }

/// Register the Tna RtSmith target with the P4RuntimeSmith framework.
inline void tofino_registerRtSmithTarget() { Tna::TofinoTnaRtSmithTarget::make(); }

}  // namespace P4Tools::RTSmith

#endif /* BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_TOFINO_REGISTER_H_ */
