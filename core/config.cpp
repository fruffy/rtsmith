#include "backends/p4tools/modules/p4rtsmith/core/config.h"

namespace P4::P4Tools::RTSmith {

void FuzzerConfig::setMaxEntryGenCnt(const int numEntries) {
    if (numEntries <= 0) {
        ::P4::error(
            "P4RuntimeSmith: The maximum number of entries per table must be a positive integer.");
    }
    maxEntryGenCnt = numEntries;
}

void FuzzerConfig::setAttempts(const int numAttempts) {
    if (numAttempts <= 0) {
        ::P4::error("P4RuntimeSmith: The number of attempts must be a positive integer.");
    }
    attempts = numAttempts;
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

void FuzzerConfig::setIsUpdateEntry(const bool updateEntry) { isUpdateEntry = updateEntry; }

void FuzzerConfig::setUpdateCount(const size_t count) {
    if (count <= 0) {
        throw std::invalid_argument(
            "P4RuntimeSmith: The number of updates must be a positive integer.");
    }
    updateCount = count;
}

void FuzzerConfig::setMicroseconds(const uint64_t micros) {
    if (micros <= 0) {
        throw std::invalid_argument(
            "P4RuntimeSmith: The number of microseconds must be a positive integer.");
    }
    microseconds = micros;
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
    try {
        const int maxEntryGenCnt = tomlConfig["tables"]["max_entry_generation_count"].value_or(5);
        setMaxEntryGenCnt(maxEntryGenCnt);
    } catch (const std::runtime_error &e) {
        /* Ignore. */
    }

    try {
        const int attempts = tomlConfig["tables"]["max_attempts"].value_or(50);
        setAttempts(attempts);
    } catch (const std::runtime_error &e) {
        /* Ignore. */
    }

    try {
        const int maxTables = tomlConfig["tables"]["max_tables"].value_or(5);
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

    try {
        const bool isUpdateEntry = tomlConfig["tables"]["is_update_entry"].value_or(false);
        setIsUpdateEntry(isUpdateEntry);
    } catch (const std::runtime_error &e) {
        /* Ignore. */
    }

    try {
        const size_t updateCount = tomlConfig["tables"]["update_count"].value_or(10);
        setUpdateCount(updateCount);
    } catch (const std::runtime_error &e) {
        /* Ignore. */
    }

    try {
        const uint64_t microseconds = tomlConfig["tables"]["microseconds"].value_or(1000);
        setMicroseconds(microseconds);
    } catch (const std::runtime_error &e) {
        /* Ignore. */
    }
}

}  // namespace P4::P4Tools::RTSmith
