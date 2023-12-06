#ifndef BACKENDS_P4TOOLS_MODULES_P4RTSMITH_CORE_FUZZER_H_
#define BACKENDS_P4TOOLS_MODULES_P4RTSMITH_CORE_FUZZER_H_

#include "backends/p4tools/modules/p4rtsmith/core/program_info.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wpedantic"
#include "p4/v1/p4runtime.pb.h"
#pragma GCC diagnostic pop

namespace P4Tools::RTSmith {

using InitialP4RuntimeConfig = std::vector<p4::v1::WriteRequest>;
using P4RuntimeUpdateSeries = std::vector<std::pair<uint64_t, p4::v1::WriteRequest>>;

class P4RuntimeFuzzer {
 private:
    /// The program info of the target.
    std::reference_wrapper<const ProgramInfo> programInfo;

 protected:
    /// @returns the program info associated with the current target.
    [[nodiscard]] virtual const ProgramInfo &getProgramInfo() const { return programInfo; }

 public:
    explicit P4RuntimeFuzzer(const ProgramInfo &programInfo) : programInfo(programInfo) {}

    P4RuntimeFuzzer(const P4RuntimeFuzzer &) = default;
    P4RuntimeFuzzer(P4RuntimeFuzzer &&) = delete;
    P4RuntimeFuzzer &operator=(const P4RuntimeFuzzer &) = default;
    P4RuntimeFuzzer &operator=(P4RuntimeFuzzer &&) = delete;
    virtual ~P4RuntimeFuzzer() = default;

    virtual InitialP4RuntimeConfig produceInitialConfig() = 0;

    virtual P4RuntimeUpdateSeries produceUpdateTimeSeries() = 0;
};

}  // namespace P4Tools::RTSmith

#endif /* BACKENDS_P4TOOLS_MODULES_P4RTSMITH_CORE_FUZZER_H_ */
