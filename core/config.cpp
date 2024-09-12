#include "backends/p4tools/modules/p4rtsmith/core/config.h"

#include "backends/p4tools/common/lib/util.h"
#include "control-plane/bytestrings.h"
#include "control-plane/p4infoApi.h"

namespace P4::P4Tools::RTSmith {

void FuzzerConfig::setMaxEntriesPerTable(const int numEntries) {
    if (numEntries <= 0) {
        ::P4::error(
            "P4RuntimeSmith: The maximum number of entries per table must be a positive integer.");
    }
    maxEntriesPerTable = numEntries;
}

void FuzzerConfig::setMaxTables(const int numTables) {
    if (numTables <= 0) {
        ::P4::error("P4RuntimeSmith: The maximum number of tables must be a positive integer.");
    }
    maxTables = numTables;
}

void FuzzerConfig::setTablesToSkip(const std::vector<std::string> &tables) {
    tablesToSkip = tables;
}

void FuzzerConfig::override_fuzzer_configs(const char *path) {
    toml::parse_result tomlConfig;
    try {
        // Note that the parameter fed into the `parse_file` function should be of (or could be
        // converted to) type `std::string_view`.
        tomlConfig = toml::parse_file(path);
    } catch (const toml::parse_error &e) {
        ::P4::error("P4RuntimeSmith: Failed to parse fuzzer configuration file: %1%", e.what());
    }

    // Retrieve the configurations from the TOML file.
    // TODO(zzmic): Figure out whether exception raising is needed if the keys are not found.
    const int maxEntriesPerTable = tomlConfig["tables"]["max_entries_per_table"].value_or(0);
    const int maxTables = tomlConfig["tables"]["max_tables"].value_or(0);
    std::vector<std::string> tablesToSkip;
    if (const auto *stringRepresentations = tomlConfig["tables"]["tables_to_skip"].as_array()) {
        for (const auto &element : *stringRepresentations) {
            if (const auto *str = element.as_string()) {
                tablesToSkip.push_back(str->get());
            }
        }
    }

    // Override the default configurations.
    setMaxEntriesPerTable(maxEntriesPerTable);
    setMaxTables(maxTables);
    setTablesToSkip(tablesToSkip);
}

}  // namespace P4::P4Tools::RTSmith
