#ifndef BACKENDS_P4TOOLS_MODULES_P4RTSMITH_CORE_CONFIG_H_
#define BACKENDS_P4TOOLS_MODULES_P4RTSMITH_CORE_CONFIG_H_

#include <toml++/toml.hpp>

#include "backends/p4tools/modules/p4rtsmith/core/program_info.h"

namespace P4::P4Tools::RTSmith {

class FuzzerConfig {
 private:
    /// Maximum number of entries per table.
    int maxEntriesPerTable = 10;

    /// Maximum number of tables.
    // TODO(zzmic): Verfiy whether this is the desired default value.
    int maxTables = 5;

    /// String representations of tables to skip.
    // TODO(zzmic): Verify whether this is needed.
    std::vector<std::string> tablesToSkip;

    // Helpers to modify/override the fuzzer configurations.
    void setMaxEntriesPerTable(const int numEntries);
    void setMaxTables(const int numTables);
    void setTablesToSkip(const std::vector<std::string> &tables);

 public:
    // Default constructor.
    FuzzerConfig() = default;

    /// @brief Override the default fuzzer configurations through the TOML file.
    // @param path The path to the TOML file.
    void override_fuzzer_configs(const char *path);

    // Getters to access the fuzzer configurations.
    [[nodiscard]] int getMaxEntriesPerTable() const { return maxEntriesPerTable; }
    [[nodiscard]] int getMaxTables() const { return maxTables; }
    [[nodiscard]] const std::vector<std::string> &getTablesToSkip() const { return tablesToSkip; }
};

}  // namespace P4::P4Tools::RTSmith

#endif /* BACKENDS_P4TOOLS_MODULES_P4RTSMITH_CORE_CONFIG_H_ */
