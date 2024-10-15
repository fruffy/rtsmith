#include "backends/p4tools/modules/p4rtsmith/core/config.h"

#include "lib/error.h"

namespace P4::P4Tools::RTSmith {

void FuzzerConfig::setMaxEntryGenCnt(const int numEntries) {
    if (numEntries < 0) {
        error(
            "P4RuntimeSmith: The maximum number of entries to generate must be a non-negative "
            "integer.");
    }
    maxEntryGenCnt = numEntries;
}

void FuzzerConfig::setMaxAttempts(const int numAttempts) {
    if (numAttempts <= 0) {
        error("P4RuntimeSmith: The number of attempts must be a positive integer.");
    }
    maxAttempts = numAttempts;
}

void FuzzerConfig::setMaxTables(const int numTables) {
    if (numTables < 0) {
        error("P4RuntimeSmith: The maximum number of tables must be a non-negative integer.");
    }
    maxTables = numTables;
}

void FuzzerConfig::setThresholdForDeletion(const uint64_t threshold) {
    thresholdForDeletion = threshold;
}

void FuzzerConfig::setTablesToSkip(const std::vector<std::string> &tables) {
    tablesToSkip = tables;
}

void FuzzerConfig::setMaxUpdateCount(const size_t count) { maxUpdateCount = count; }

void FuzzerConfig::setMaxUpdateTimeInMicroseconds(const uint64_t micros) {
    if (micros <= 0) {
        error("P4RuntimeSmith: The maximum wait time must be a positive integer.");
    }
    maxUpdateTimeInMicroseconds = micros;
}
void FuzzerConfig::setMinUpdateTimeInMicroseconds(const uint64_t micros) {
    if (micros <= 0) {
        error("P4RuntimeSmith: The minimum wait time must be a positive integer.");
    }
    minUpdateTimeInMicroseconds = micros;
}

void FuzzerConfig::overrideFuzzerConfigsViaFile(const char *path) {
    toml::parse_result tomlConfig;
    try {
        // Note that the parameter fed into the `parse_file` function should be of (or could be
        // converted to) type `std::string_view`.
        tomlConfig = toml::parse_file(path);
    } catch (const toml::parse_error &e) {
        error("P4RuntimeSmith: Failed to parse fuzzer configuration file: %1%", e.what());
    }

    // For the following blocks, retrieve the configurations from the TOML file and override the
    // default configurations if they comply with the constraints.
    int maxEntryGenCntConfig = tomlConfig["maxEntryGenCnt"].value_or(0);
    if (maxEntryGenCntConfig <= 0) {
        error(
            "P4RuntimeSmith: The maximum number of entries to generate must be provided and must "
            "be a non-negative integer.");
    } else {
        setMaxEntryGenCnt(maxEntryGenCntConfig);
    }

    int maxAttemptsConfig = tomlConfig["maxAttempts"].value_or(0);
    if (maxEntryGenCntConfig <= 0) {
        error(
            "P4RuntimeSmith: The maximum number of entries to generate must be a non-negative "
            "integer.");
    } else {
        setMaxAttempts(maxAttemptsConfig);
    }

    int maxTablesConfig = tomlConfig["maxTables"].value_or(0);
    if (maxTablesConfig <= 0) {
        error(
            "P4RuntimeSmith: The maximum number of tables must be provided and must be a "
            "non-negative integer.");
    } else {
        setMaxTables(maxTablesConfig);
    }

    std::vector<std::string> tablesToSkipConfig;
    if (const auto *stringRepresentations = tomlConfig["tablesToSkip"].as_array()) {
        for (const auto &stringRepresentation : *stringRepresentations) {
            if (const auto *str = stringRepresentation.as_string()) {
                tablesToSkipConfig.push_back(str->get());
            }
        }
        setTablesToSkip(tablesToSkipConfig);
    } else {
        error("P4RuntimeSmith: The tables to skip must be provided.");
    }

    uint64_t thresholdForDeletionConfig = tomlConfig["thresholdForDeletion"].value_or(0);
    if (thresholdForDeletionConfig <= 0) {
        error(
            "P4RuntimeSmith: The threshold for deletion must be provided and must be a positive "
            "integer.");
    } else {
        setThresholdForDeletion(thresholdForDeletionConfig);
    }

    size_t maxUpdateCountConfig = tomlConfig["maxUpdateCount"].value_or(0);
    if (maxUpdateCountConfig <= 0) {
        error(
            "P4RuntimeSmith: The maximum number of updates must be provided and must be a positive "
            "integer.");
    } else {
        setMaxUpdateCount(maxUpdateCountConfig);
    }

    uint64_t maxUpdateTimeInMicrosecondsConfig =
        tomlConfig["maxUpdateTimeInMicroseconds"].value_or(0);
    uint64_t minUpdateTimeInMicrosecondsConfig =
        tomlConfig["minUpdateTimeInMicroseconds"].value_or(0);
    if (maxUpdateTimeInMicrosecondsConfig <= 0) {
        error(
            "P4RuntimeSmith: The maximum wait time must be provided and must be a positive "
            "integer.");
    } else {
        setMaxUpdateTimeInMicroseconds(maxUpdateTimeInMicrosecondsConfig);
    }
    if (minUpdateTimeInMicrosecondsConfig <= 0) {
        error(
            "P4RuntimeSmith: The minimum wait time must be provided and must be a positive "
            "integer.");
    } else {
        setMinUpdateTimeInMicroseconds(minUpdateTimeInMicrosecondsConfig);
    }
}

void FuzzerConfig::overrideFuzzerConfigsViaString(const char *configInString) {
    toml::parse_result tomlConfig;
    try {
        // Note that the parameter fed into the `parse` function should be of (or could be
        // converted to) type `std::string_view`.
        tomlConfig = toml::parse(configInString);
    } catch (const toml::parse_error &e) {
        error("P4RuntimeSmith: Failed to parse fuzzer configuration string: %1%", e.what());
    }

    // For the following blocks, retrieve the configurations from the string and override the
    // default configurations if they comply with the constraints.
    int maxEntryGenCntConfig = tomlConfig["maxEntryGenCnt"].value_or(0);
    if (maxEntryGenCntConfig <= 0) {
        error(
            "P4RuntimeSmith: The maximum number of entries to generate must be provided and must "
            "be a non-negative integer.");
    } else {
        setMaxEntryGenCnt(maxEntryGenCntConfig);
    }

    int maxAttemptsConfig = tomlConfig["maxAttempts"].value_or(0);
    if (maxEntryGenCntConfig <= 0) {
        error(
            "P4RuntimeSmith: The maximum number of entries to generate must be a non-negative "
            "integer.");
    } else {
        setMaxAttempts(maxAttemptsConfig);
    }

    int maxTablesConfig = tomlConfig["maxTables"].value_or(0);
    if (maxTablesConfig <= 0) {
        error(
            "P4RuntimeSmith: The maximum number of tables must be provided and must be a "
            "non-negative integer.");
    } else {
        setMaxTables(maxTablesConfig);
    }

    std::vector<std::string> tablesToSkipConfig;
    if (const auto *stringRepresentations = tomlConfig["tablesToSkip"].as_array()) {
        for (const auto &stringRepresentation : *stringRepresentations) {
            if (const auto *str = stringRepresentation.as_string()) {
                tablesToSkipConfig.push_back(str->get());
            }
        }
        setTablesToSkip(tablesToSkipConfig);
    } else {
        error("P4RuntimeSmith: The tables to skip must be provided.");
    }

    uint64_t thresholdForDeletionConfig = tomlConfig["thresholdForDeletion"].value_or(0);
    if (thresholdForDeletionConfig <= 0) {
        error(
            "P4RuntimeSmith: The threshold for deletion must be provided and must be a positive "
            "integer.");
    } else {
        setThresholdForDeletion(thresholdForDeletionConfig);
    }

    size_t maxUpdateCountConfig = tomlConfig["maxUpdateCount"].value_or(0);
    if (maxUpdateCountConfig <= 0) {
        error(
            "P4RuntimeSmith: The maximum number of updates must be provided and must be a positive "
            "integer.");
    } else {
        setMaxUpdateCount(maxUpdateCountConfig);
    }

    uint64_t maxUpdateTimeInMicrosecondsConfig =
        tomlConfig["maxUpdateTimeInMicroseconds"].value_or(0);
    uint64_t minUpdateTimeInMicrosecondsConfig =
        tomlConfig["minUpdateTimeInMicroseconds"].value_or(0);
    if (maxUpdateTimeInMicrosecondsConfig <= 0) {
        error(
            "P4RuntimeSmith: The maximum wait time must be provided and must be a positive "
            "integer.");
    } else {
        setMaxUpdateTimeInMicroseconds(maxUpdateTimeInMicrosecondsConfig);
    }
    if (minUpdateTimeInMicrosecondsConfig <= 0) {
        error(
            "P4RuntimeSmith: The minimum wait time must be provided and must be a positive "
            "integer.");
    } else {
        setMinUpdateTimeInMicroseconds(minUpdateTimeInMicrosecondsConfig);
    }
}

}  // namespace P4::P4Tools::RTSmith
