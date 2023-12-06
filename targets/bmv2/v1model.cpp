#include "backends/p4tools/modules/p4rtsmith/targets/bmv2/v1model.h"

#include <string>

namespace P4Tools::RTSmith::V1Model {

Bmv2V1ModelCompilerTarget::Bmv2V1ModelCompilerTarget() : CompilerTarget("bmv2", "v1model") {}

void Bmv2V1ModelCompilerTarget::make() {
    static Bmv2V1ModelCompilerTarget *INSTANCE = nullptr;
    if (INSTANCE == nullptr) {
        INSTANCE = new Bmv2V1ModelCompilerTarget();
    }
}

MidEnd Bmv2V1ModelCompilerTarget::mkMidEnd(const CompilerOptions &options) const {
    MidEnd midEnd(options);
    // Currently a no-op because we have all the necessary information from the front-end.
    midEnd.addPasses({});
    return midEnd;
}

}  // namespace P4Tools::RTSmith::V1Model
