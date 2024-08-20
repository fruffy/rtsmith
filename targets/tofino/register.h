#ifndef BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_TOFINO_REGISTER_H_
#define BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_TOFINO_REGISTER_H_

#include "backends/p4tools/modules/p4rtsmith/targets/tofino/target.h"

namespace P4::P4Tools::RTSmith {

/// Register the Tna RtSmith target with the P4RuntimeSmith framework.
inline void tofino_registerRtSmithTarget() { Tna::TofinoTnaRtSmithTarget::make(); }

}  // namespace P4::P4Tools::RTSmith

#endif /* BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_TOFINO_REGISTER_H_ */
