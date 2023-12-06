#include "backends/p4tools/modules/p4rtsmith/options.h"

#include "backends/p4tools/common/options.h"
#include "lib/cstring.h"
#include "lib/exceptions.h"

namespace P4Tools {

RtSmithOptions &RtSmithOptions::get() {
    static RtSmithOptions INSTANCE;
    return INSTANCE;
}

const char *RtSmithOptions::getIncludePath() {
    P4C_UNIMPLEMENTED("getIncludePath not implemented for P4RuntimeSmith.");
}

RtSmithOptions::RtSmithOptions()
    : AbstractP4cToolOptions("Remove control-plane dead code from a P4 program.") {}

}  // namespace P4Tools
