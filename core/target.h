#ifndef BACKENDS_P4TOOLS_MODULES_P4RTSMITH_CORE_TARGET_H_
#define BACKENDS_P4TOOLS_MODULES_P4RTSMITH_CORE_TARGET_H_

#include <string>

#include "backends/p4tools/common/core/target.h"
#include "backends/p4tools/modules/p4rtsmith/core/fuzzer.h"
#include "backends/p4tools/modules/p4rtsmith/core/program_info.h"
#include "ir/ir.h"

namespace P4Tools::RTSmith {

class RtSmithTarget : public Target {
 public:
    /// @returns the singleton instance for the current target.
    static const RtSmithTarget &get();

    /// Produces a @ProgramInfo for the given P4 program.
    ///
    /// @returns nullptr if the program is not supported by this target.
    static const ProgramInfo *produceProgramInfo(const IR::P4Program *program);

    /// @returns the fuzzer that will produce an initial configuration and a series of random write
    /// requests..
    [[nodiscard]] static P4RuntimeFuzzer &getFuzzer(const ProgramInfo &programInfo);

 protected:
    /// @see @produceProgramInfo.
    const ProgramInfo *produceProgramInfoImpl(const IR::P4Program *program) const;

    /// @see @produceProgramInfo.
    virtual const ProgramInfo *produceProgramInfoImpl(
        const IR::P4Program *program, const IR::Declaration_Instance *mainDecl) const = 0;

    /// @see @getStepper.
    [[nodiscard]] virtual P4RuntimeFuzzer &getFuzzerImpl(const ProgramInfo &programInfo) const = 0;

    explicit RtSmithTarget(std::string deviceName, std::string archName);

 private:
};

}  // namespace P4Tools::RTSmith

#endif /* BACKENDS_P4TOOLS_MODULES_P4RTSMITH_CORE_TARGET_H_ */
