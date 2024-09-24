#include "backends/p4tools/modules/p4rtsmith/core/config.h"

namespace P4::P4Tools::RTSmith {

void FuzzerConfig::setMaxEntryGenCnt(const int numEntries) {
    if (numEntries <= 0) {
        // NOTE: `::P4::error` is not usable here, as it requires `ProgramInfo`, which results in
        // circular dependencies.
        throw std::invalid_argument(
            "P4RuntimeSmith: The maximum number of entries per table must be a positive integer.");
    }
    maxEntryGenCnt = numEntries;
}

void FuzzerConfig::setAttempts(const int numAttempts) {
    if (numAttempts <= 0) {
        throw std::invalid_argument(
            "P4RuntimeSmith: The number of attempts must be a positive integer.");
    }
    attempts = numAttempts;
}

void FuzzerConfig::setMaxTables(const int numTables) {
    if (numTables <= 0) {
        throw std::invalid_argument(
            "P4RuntimeSmith: The maximum number of tables must be a positive integer.");
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

}  // namespace P4::P4Tools::RTSmith
