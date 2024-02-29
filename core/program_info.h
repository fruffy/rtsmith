#ifndef BACKENDS_P4TOOLS_MODULES_P4RTSMITH_CORE_PROGRAM_INFO_H_
#define BACKENDS_P4TOOLS_MODULES_P4RTSMITH_CORE_PROGRAM_INFO_H_

#include <functional>

#include "backends/p4tools/common/compiler/compiler_target.h"
#include "control-plane/p4RuntimeSerializer.h"
#include "ir/ir.h"
#include "lib/castable.h"

namespace P4Tools::RTSmith {

/// Stores target-specific information about a P4 program.
class ProgramInfo : public ICastable {
 private:
    /// The P4 program from which this object is derived.
    std::reference_wrapper<const CompilerResult> compilerResult;

    P4::P4RuntimeAPI p4runtimeApi;

 protected:
    explicit ProgramInfo(const CompilerResult &compilerResult, P4::P4RuntimeAPI p4runtimeApi);

 public:
    ProgramInfo(const ProgramInfo &) = default;

    ProgramInfo(ProgramInfo &&) = default;

    ProgramInfo &operator=(const ProgramInfo &) = default;

    ProgramInfo &operator=(ProgramInfo &&) = default;

    ~ProgramInfo() override = default;

    /// @returns the P4 program associated with this program info.
    [[nodiscard]] const IR::P4Program *getProgram() const;

    /// @returns the P4RuntimeAPI associated with this program.
    [[nodiscard]] const P4::P4RuntimeAPI &getP4RuntimeApi() const;
};

}  // namespace P4Tools::RTSmith

#endif /* BACKENDS_P4TOOLS_MODULES_P4RTSMITH_CORE_PROGRAM_INFO_H_ */
