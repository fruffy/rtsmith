#ifndef BACKENDS_P4TOOLS_MODULES_P4RTSMITH_CORE_CONFIG_H_
#define BACKENDS_P4TOOLS_MODULES_P4RTSMITH_CORE_CONFIG_H_

#include <toml++/toml.hpp>

#include "backends/p4tools/common/lib/util.h"
#include "backends/p4tools/modules/p4rtsmith/core/program_info.h"

namespace P4::P4Tools::RTSmith {

class FuzzerConfig {
 private:
    /// The maximum number of entries we are trying to generate for a table.
    int maxEntryGenCnt = 5;
    // The maximum attempts we are trying to generate an entry.
    int maxAttempts = 100;
    /// The maximum number of tables.
    int maxTables = 5;
    /// The string representations of tables to skip.
    std::vector<std::string> tablesToSkip;
    /// Threshold for deletion.
    uint64_t thresholdForDeletion = 30;
    /// The maximum number of updates.
    size_t maxUpdateCount = 10;
    /// The maximum time (in microseconds) for the update.
    uint64_t maxUpdateTimeInMicroseconds = 1000;
    /// The minimum time (in microseconds) for the update.
    uint64_t minUpdateTimeInMicroseconds = 10;

 protected:
    /// Setters to modify/override the fuzzer configurations.
    void setMaxEntryGenCnt(const int numEntries);
    void setMaxAttempts(const int numAttempts);
    void setMaxTables(const int numTables);
    void setTablesToSkip(const std::vector<std::string> &tables);
    void setThresholdForDeletion(const uint64_t threshold);
    void setMaxUpdateCount(const size_t count);
    void setMaxUpdateTimeInMicroseconds(const uint64_t micros);
    void setMinUpdateTimeInMicroseconds(const uint64_t micros);

 public:
    // Default constructor.
    FuzzerConfig() = default;

    // Default destructor.
    virtual ~FuzzerConfig() = default;

    /// @brief Override the default fuzzer configurations through the TOML file.
    /// @param path The path to the TOML file.
    void override_fuzzer_configs(const char *path);

    /// Getters to access the fuzzer configurations.
    [[nodiscard]] int getMaxEntryGenCnt() const { return maxEntryGenCnt; }
    [[nodiscard]] int getMaxAttempts() const { return maxAttempts; }
    [[nodiscard]] int getMaxTables() const { return maxTables; }
    [[nodiscard]] const std::vector<std::string> &getTablesToSkip() const { return tablesToSkip; }
    /// Get a random decision to update or delete an entry.
    [[nodiscard]] bool getIsUpdateEntry() const {
        return Utils::getRandInt(100) >= thresholdForDeletion;
    }
    [[nodiscard]] uint64_t getThresholdForDeletion() const { return thresholdForDeletion; }
    [[nodiscard]] size_t getMaxUpdateCount() const { return maxUpdateCount; }
    /// Get a random update count.
    [[nodiscard]] size_t getRandomUpdateCount() const { return Utils::getRandInt(maxUpdateCount); }
    [[nodiscard]] uint64_t getMaxUpdateTimeInMicroseconds() const {
        return maxUpdateTimeInMicroseconds;
    }
    [[nodiscard]] uint64_t getMinUpdateTimeInMicroseconds() const {
        return minUpdateTimeInMicroseconds;
    }
    /// Get a random update time in microseconds.
    [[nodiscard]] uint64_t getRandomUpdateTimeInMicroseconds() const {
        return Utils::getRandInt(minUpdateTimeInMicroseconds, maxUpdateTimeInMicroseconds);
    }
};

}  // namespace P4::P4Tools::RTSmith

#endif /* BACKENDS_P4TOOLS_MODULES_P4RTSMITH_CORE_CONFIG_H_ */
