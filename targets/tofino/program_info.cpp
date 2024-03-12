#include "backends/p4tools/modules/p4rtsmith/targets/tofino/program_info.h"

#include "backends/p4tools/modules/p4rtsmith/core/program_info.h"
#include "control-plane/p4RuntimeSerializer.h"
#include "ir/ir.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wpedantic"
#include "p4/v1/p4runtime.pb.h"
#pragma GCC diagnostic pop

namespace P4Tools::RTSmith::Tna
{

    TofinoTnaProgramInfo::TofinoTnaProgramInfo(const CompilerResult &compilerResult)
        : ProgramInfo(compilerResult, P4::P4RuntimeSerializer::get()->generateP4Runtime(
                                          &compilerResult.getProgram(), "tna")) {}

} // namespace P4Tools::RTSmith::Tna
