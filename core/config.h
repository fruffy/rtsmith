#ifndef BACKENDS_P4TOOLS_MODULES_P4RTSMITH_CORE_CONFIG_H_
#define BACKENDS_P4TOOLS_MODULES_P4RTSMITH_CORE_CONFIG_H_

#include <toml++/toml.hpp>

#include "backends/p4tools/modules/p4rtsmith/core/program_info.h"

namespace P4::P4Tools::RTSmith {

class FuzzerConfig {
 private:
    /// Maximum number of entries per table.
    int maxEntryGenCnt = 5;
    // The maximum attempts we are trying to generate an entry.
    int attempts = 0;
    /// Maximum number of tables.
    int maxTables = 5;
    /// String representations of tables to skip.
    std::vector<std::string> tablesToSkip;
    /// TODO(zzmic): Verify whether this is needed to be non-deterministic (random).
    /// Whether we update or delete the entry.
    bool isUpdateEntry = false;
    /// The number of updates to generate.
    size_t updateCount = 10;
    /// The number of microseconds to wait before sending the next update.
    uint64_t microseconds = 1000;

 protected:
    /// Setters to modify/override the fuzzer configurations.
    void setMaxEntryGenCnt(const int numEntries);
    void setAttempts(const int numAttempts);
    void setMaxTables(const int numTables);
    void setTablesToSkip(const std::vector<std::string> &tables);
    void setIsUpdateEntry(const bool updateEntry);
    void setUpdateCount(const size_t count);
    void setMicroseconds(const uint64_t micros);

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
    [[nodiscard]] int getAttempts() const { return attempts; }
    [[nodiscard]] int getMaxTables() const { return maxTables; }
    [[nodiscard]] const std::vector<std::string> &getTablesToSkip() const { return tablesToSkip; }
    [[nodiscard]] bool getIsUpdateEntry() const { return isUpdateEntry; }
    [[nodiscard]] size_t getUpdateCount() const { return updateCount; }
    [[nodiscard]] uint64_t getMicroseconds() const { return microseconds; }
};

}  // namespace P4::P4Tools::RTSmith

#endif /* BACKENDS_P4TOOLS_MODULES_P4RTSMITH_CORE_CONFIG_H_ */
