#include "backends/p4tools/modules/p4rtsmith/core/program_info.h"

namespace P4Tools::RTSmith {

ProgramInfo::ProgramInfo(const IR::P4Program *program, P4::P4RuntimeAPI p4runtimeApi)
    : program(program), p4runtimeApi(p4runtimeApi) {}

/* =============================================================================================
 *  Getters
 * ============================================================================================= */

const IR::P4Program *ProgramInfo::getProgram() const { return program; }

const P4::P4RuntimeAPI &ProgramInfo::getP4RuntimeApi() const { return p4runtimeApi; }

}  // namespace P4Tools::RTSmith
