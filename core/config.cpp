#include "backends/p4tools/modules/rtsmith/core/config.h"

#include "lib/error.h"

namespace P4::P4Tools::RtSmith {

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
    auto maxEntryGenCntNode = tomlConfig["maxEntryGenCnt"];
    // Check if the node exists. If not, do nothing and leave the default configuration as is.
    if (!!maxEntryGenCntNode) {
        // Check if the node represents an integer, meaning whether the configuration provided is
        // valid.
        if (maxEntryGenCntNode.type() == toml::node_type::integer) {
            int maxEntryGenCntConfig = maxEntryGenCntNode.value<int>().value();
            setMaxEntryGenCnt(maxEntryGenCntConfig);
        } else {
            error("P4RuntimeSmith: The maximum number of entries to generate must be an integer.");
        }
    }

    auto maxAttemptsNode = tomlConfig["maxAttempts"];
    if (!!maxAttemptsNode) {
        if (maxAttemptsNode.type() == toml::node_type::integer) {
            int maxAttemptsConfig = maxAttemptsNode.value<int>().value();
            setMaxAttempts(maxAttemptsConfig);
        } else {
            error("P4RuntimeSmith: The maximum number of attempts must be an integer.");
        }
    }

    auto maxTablesNode = tomlConfig["maxTables"];
    if (!!maxTablesNode) {
        if (maxTablesNode.type() == toml::node_type::integer) {
            int maxTablesConfig = maxTablesNode.value<int>().value();
            setMaxTables(maxTablesConfig);
        } else {
            error("P4RuntimeSmith: The maximum number of tables must be an integer.");
        }
    }

    auto tablesToSkipNode = tomlConfig["tablesToSkip"];
    if (!!tablesToSkipNode) {
        if (tablesToSkipNode.type() == toml::node_type::array) {
            std::vector<std::string> tablesToSkipConfig;
            const auto *expectedStringRepresentations = tablesToSkipNode.as_array();
            for (const auto &expectedStringRepresentation : *expectedStringRepresentations) {
                if (const auto *str = expectedStringRepresentation.as_string()) {
                    tablesToSkipConfig.push_back(str->get());
                } else {
                    error("P4RuntimeSmith: The tables to skip must be strings.");
                }
            }
            setTablesToSkip(tablesToSkipConfig);
        } else {
            error("P4RuntimeSmith: The tables to skip must be an array.");
        }
    }

    auto thresholdForDeletionNode = tomlConfig["thresholdForDeletion"];
    if (!!thresholdForDeletionNode) {
        if (thresholdForDeletionNode.type() == toml::node_type::integer) {
            uint64_t thresholdForDeletionConfig =
                thresholdForDeletionNode.value<uint64_t>().value();
            setThresholdForDeletion(thresholdForDeletionConfig);
        } else {
            error("P4RuntimeSmith: The threshold for deletion must be an integer.");
        }
    }

    auto maxUpdateCountNode = tomlConfig["maxUpdateCount"];
    if (!!maxUpdateCountNode) {
        if (maxUpdateCountNode.type() == toml::node_type::integer) {
            size_t maxUpdateCountConfig = maxUpdateCountNode.value<size_t>().value();
            setMaxUpdateCount(maxUpdateCountConfig);
        } else {
            error("P4RuntimeSmith: The maximum number of updates must be an integer.");
        }
    }

    auto maxUpdateTimeInMicrosecondsNode = tomlConfig["maxUpdateTimeInMicroseconds"];
    auto minUpdateTimeInMicrosecondsNode = tomlConfig["minUpdateTimeInMicroseconds"];
    if (!!maxUpdateTimeInMicrosecondsNode) {
        if (maxUpdateTimeInMicrosecondsNode.type() == toml::node_type::integer) {
            uint64_t maxUpdateTimeInMicrosecondsConfig =
                maxUpdateTimeInMicrosecondsNode.value<uint64_t>().value();
            setMaxUpdateTimeInMicroseconds(maxUpdateTimeInMicrosecondsConfig);
        } else {
            error("P4RuntimeSmith: The maximum wait time must be an integer.");
        }
    }
    if (!!minUpdateTimeInMicrosecondsNode) {
        if (minUpdateTimeInMicrosecondsNode.type() == toml::node_type::integer) {
            uint64_t minUpdateTimeInMicrosecondsConfig =
                minUpdateTimeInMicrosecondsNode.value<uint64_t>().value();
            setMinUpdateTimeInMicroseconds(minUpdateTimeInMicrosecondsConfig);
        } else {
            error("P4RuntimeSmith: The minimum wait time must be an integer.");
        }
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

    // For the following blocks, retrieve the configurations from the TOML file and override the
    // default configurations if they comply with the constraints.
    auto maxEntryGenCntNode = tomlConfig["maxEntryGenCnt"];
    // Check if the node exists. If not, do nothing and leave the default configuration as is.
    if (!!maxEntryGenCntNode) {
        // Check if the node represents an integer, meaning whether the configuration provided is
        // valid.
        if (maxEntryGenCntNode.type() == toml::node_type::integer) {
            int maxEntryGenCntConfig = maxEntryGenCntNode.value<int>().value();
            setMaxEntryGenCnt(maxEntryGenCntConfig);
        } else {
            error("P4RuntimeSmith: The maximum number of entries to generate must be an integer.");
        }
    }

    auto maxAttemptsNode = tomlConfig["maxAttempts"];
    if (!!maxAttemptsNode) {
        if (maxAttemptsNode.type() == toml::node_type::integer) {
            int maxAttemptsConfig = maxAttemptsNode.value<int>().value();
            setMaxAttempts(maxAttemptsConfig);
        } else {
            error("P4RuntimeSmith: The maximum number of attempts must be an integer.");
        }
    }

    auto maxTablesNode = tomlConfig["maxTables"];
    if (!!maxTablesNode) {
        if (maxTablesNode.type() == toml::node_type::integer) {
            int maxTablesConfig = maxTablesNode.value<int>().value();
            setMaxTables(maxTablesConfig);
        } else {
            error("P4RuntimeSmith: The maximum number of tables must be an integer.");
        }
    }

    auto tablesToSkipNode = tomlConfig["tablesToSkip"];
    if (!!tablesToSkipNode) {
        if (tablesToSkipNode.type() == toml::node_type::array) {
            std::vector<std::string> tablesToSkipConfig;
            const auto *expectedStringRepresentations = tablesToSkipNode.as_array();
            for (const auto &expectedStringRepresentation : *expectedStringRepresentations) {
                if (const auto *str = expectedStringRepresentation.as_string()) {
                    tablesToSkipConfig.push_back(str->get());
                } else {
                    error("P4RuntimeSmith: The tables to skip must be strings.");
                }
            }
            setTablesToSkip(tablesToSkipConfig);
        } else {
            error("P4RuntimeSmith: The tables to skip must be an array.");
        }
    }

    auto thresholdForDeletionNode = tomlConfig["thresholdForDeletion"];
    if (!!thresholdForDeletionNode) {
        if (thresholdForDeletionNode.type() == toml::node_type::integer) {
            uint64_t thresholdForDeletionConfig =
                thresholdForDeletionNode.value<uint64_t>().value();
            setThresholdForDeletion(thresholdForDeletionConfig);
        } else {
            error("P4RuntimeSmith: The threshold for deletion must be an integer.");
        }
    }

    auto maxUpdateCountNode = tomlConfig["maxUpdateCount"];
    if (!!maxUpdateCountNode) {
        if (maxUpdateCountNode.type() == toml::node_type::integer) {
            size_t maxUpdateCountConfig = maxUpdateCountNode.value<size_t>().value();
            setMaxUpdateCount(maxUpdateCountConfig);
        } else {
            error("P4RuntimeSmith: The maximum number of updates must be an integer.");
        }
    }

    auto maxUpdateTimeInMicrosecondsNode = tomlConfig["maxUpdateTimeInMicroseconds"];
    auto minUpdateTimeInMicrosecondsNode = tomlConfig["minUpdateTimeInMicroseconds"];
    if (!!maxUpdateTimeInMicrosecondsNode) {
        if (maxUpdateTimeInMicrosecondsNode.type() == toml::node_type::integer) {
            uint64_t maxUpdateTimeInMicrosecondsConfig =
                maxUpdateTimeInMicrosecondsNode.value<uint64_t>().value();
            setMaxUpdateTimeInMicroseconds(maxUpdateTimeInMicrosecondsConfig);
        } else {
            error("P4RuntimeSmith: The maximum wait time must be an integer.");
        }
    }
    if (!!minUpdateTimeInMicrosecondsNode) {
        if (minUpdateTimeInMicrosecondsNode.type() == toml::node_type::integer) {
            uint64_t minUpdateTimeInMicrosecondsConfig =
                minUpdateTimeInMicrosecondsNode.value<uint64_t>().value();
            setMinUpdateTimeInMicroseconds(minUpdateTimeInMicrosecondsConfig);
        } else {
            error("P4RuntimeSmith: The minimum wait time must be an integer.");
        }
    }
}

}  // namespace P4::P4Tools::RtSmith
