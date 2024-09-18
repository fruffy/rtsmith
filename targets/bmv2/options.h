#ifndef BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_BMV2_OPTIONS_H_
#define BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_BMV2_OPTIONS_H_

#include <filesystem>
#include <optional>

#include "backends/p4tools/modules/p4rtsmith/options.h"
#include "backends/p4tools/modules/p4rtsmith/targets/bmv2/config.h"

namespace P4::P4Tools::RTSmith::V1Model {

/// Encapsulates and processes command-line options for P4RtSmith.
class Bmv2V1ModelRtSmithOptions : public RtSmithOptions {
 public:
    Bmv2V1ModelRtSmithOptions();

    /// @returns the singleton instance of this class.
    static Bmv2V1ModelRtSmithOptions &get();

 protected:
    Bmv2V1ModelFuzzerConfig _bmv2V1ModelFuzzerConfig = Bmv2V1ModelFuzzerConfig();
};

}  // namespace P4::P4Tools::RTSmith::V1Model

#endif /* BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_BMV2_OPTIONS_H_ */
