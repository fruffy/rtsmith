#include "backends/p4tools/modules/p4rtsmith/core/config.h"

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

    // Retrieve the configurations from the TOML file and override the default configurations.
    // TODO(zzmic): Figure out whether this is a desirable engineering practice.
    try {
        const int maxEntriesPerTable = tomlConfig["tables"]["max_entries_per_table"].value_or(0);
        setMaxEntriesPerTable(maxEntriesPerTable);
    } catch (const std::runtime_error &e) {
        /* Ignore. */
    }

    try {
        const int maxTables = tomlConfig["tables"]["max_tables"].value_or(0);
        setMaxTables(maxTables);
    } catch (const std::runtime_error &e) {
        /* Ignore. */
    }

    try {
        std::vector<std::string> tablesToSkip;
        if (const auto *stringRepresentations = tomlConfig["tables"]["tables_to_skip"].as_array()) {
            for (const auto &element : *stringRepresentations) {
                if (const auto *str = element.as_string()) {
                    tablesToSkip.push_back(str->get());
                }
            }
        }
        setTablesToSkip(tablesToSkip);
    } catch (const std::runtime_error &e) {
        /* Ignore. */
    }
}

}  // namespace P4::P4Tools::RTSmith
