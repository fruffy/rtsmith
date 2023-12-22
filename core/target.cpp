#include "backends/p4tools/modules/p4rtsmith/core/target.h"

#include <string>
#include <utility>

#include "backends/p4tools/common/core/target.h"
#include "backends/p4tools/modules/p4rtsmith/core/program_info.h"
#include "ir/declaration.h"
#include "ir/ir.h"
#include "ir/node.h"
#include "lib/enumerator.h"
#include "lib/exceptions.h"

namespace P4Tools::RTSmith {

RtSmithTarget::RtSmithTarget(std::string deviceName, std::string archName)
    : Target("rtsmith", std::move(deviceName), std::move(archName)) {}

const ProgramInfo *RtSmithTarget::produceProgramInfoImpl(const IR::P4Program *program) const {
    // Check that the program has at least one main declaration.
    const auto mainCount = program->getDeclsByName(IR::P4Program::main)->count();
    BUG_CHECK(mainCount > 0, "Program doesn't have a main declaration.");

    // Resolve the program's main declaration instance and delegate to the version of
    // produceProgramInfoImpl that takes the main declaration.
    const auto *mainIDecl = program->getDeclsByName(IR::P4Program::main)->single();
    BUG_CHECK(mainIDecl, "Program's main declaration not found: %1%", program->main);

    const auto *mainNode = mainIDecl->getNode();
    const auto *mainDecl = mainIDecl->to<IR::Declaration_Instance>();
    BUG_CHECK(mainDecl, "%1%: Program's main declaration is a %2%, not a Declaration_Instance",
              mainNode, mainNode->node_type_name());

    return produceProgramInfoImpl(program, mainDecl);
}

P4RuntimeFuzzer &RtSmithTarget::getFuzzer(const ProgramInfo &programInfo) {
    return get().getFuzzerImpl(programInfo);
}

const RtSmithTarget &RtSmithTarget::get() { return Target::get<RtSmithTarget>("rtsmith"); }

const ProgramInfo *RtSmithTarget::produceProgramInfo(const IR::P4Program *program) {
    return get().produceProgramInfoImpl(program);
}

}  // namespace P4Tools::RTSmith
