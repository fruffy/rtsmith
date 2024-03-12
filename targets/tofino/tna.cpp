#include "backends/p4tools/modules/p4rtsmith/targets/tofino/tna.h"

#include <string>

namespace P4Tools::RTSmith::Tna {

TofinoTnaCompilerTarget::TofinoTnaCompilerTarget() : CompilerTarget("tofino", "tna") {}

void TofinoTnaCompilerTarget::make() {
    static TofinoTnaCompilerTarget *INSTANCE = nullptr;
    if (INSTANCE == nullptr) {
        INSTANCE = new TofinoTnaCompilerTarget();
    }
}

MidEnd TofinoTnaCompilerTarget::mkMidEnd(const CompilerOptions &options) const {
    MidEnd midEnd(options);
    // Currently a no-op because we have all the necessary information from the front-end.
    midEnd.addPasses({});
    return midEnd;
}

}  // namespace P4Tools::RTSmith::Tna
