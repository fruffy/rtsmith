#ifndef BACKENDS_P4TOOLS_MODULES_P4RTSMITH_CORE_CONFIG_H_
#define BACKENDS_P4TOOLS_MODULES_P4RTSMITH_CORE_CONFIG_H_

#include <stdexcept>
#include <vector>

namespace P4::P4Tools::RTSmith {

class FuzzerConfig {
 private:
    /// The maximum number of entries we are trying to generate for a table.
    int maxEntryGenCnt = 5;
    // The maximum attempts we are trying to generate an entry.
    int attempts = 0;
    /// The maximum number of tables.
    int maxTables = 5;
    /// The string representations of tables to skip.
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
