#include <toml++/toml.hpp>

#include "backends/p4tools/modules/p4rtsmith/test/core/rtsmith_test.h"

namespace P4::P4Tools::Test {

namespace {

using namespace P4::literals;

class TOMLFuzzerConfigurationTest : public RtSmithTest {};

// Test of overriding fuzzer configurations via configurations specified in a TOML file.
TEST_F(TOMLFuzzerConfigurationTest, OverrideFuzzerConfigurationsViaTOMLFile) {
    auto source = generateTestProgram(R"(
    // Drop the packet.
    action acl_drop() {
        mark_to_drop(sm);
    }

    table drop_table {
        key = {
            hdr.eth_hdr.dst_addr : ternary @name("dst_eth");
        }
        actions = {
            acl_drop();
            @defaultonly NoAction();
        }
    }

    apply {
        if (hdr.eth_hdr.isValid()) {
            drop_table.apply();
        }
    })");
    auto autoContext = SetUp("bmv2", "v1model");
    auto &rtSmithOptions = RTSmith::RtSmithOptions::get();
    rtSmithOptions.target = "bmv2"_cs;
    rtSmithOptions.arch = "v1model"_cs;

    const char *configFilePath = "test/configuration.toml";
    rtSmithOptions.setFuzzerConfigPath(configFilePath);
    // Check if the fuzzer configuration path is set (and is set correctly).
    ASSERT_TRUE(rtSmithOptions.fuzzerConfigPath().has_value());
    ASSERT_EQ(rtSmithOptions.fuzzerConfigPath().value(), configFilePath);

    auto rtSmithResultOpt = P4::P4Tools::RTSmith::RtSmith::generateConfig(source, rtSmithOptions);
    // Check if the `RtSmithResult` object is generated successfully.
    ASSERT_TRUE(rtSmithResultOpt.has_value());

    auto compilerResult =
        P4::P4Tools::RTSmith::RtSmith::generateCompilerResult(source, rtSmithOptions);
    // Check if the `compilerResult` object is generated successfully.
    ASSERT_TRUE(compilerResult.has_value());

    const auto *programInfo = P4::P4Tools::RTSmith::RtSmithTarget::produceProgramInfo(
        compilerResult.value(), rtSmithOptions);
    // Check if the `programInfo` object is generated successfully.
    ASSERT_TRUE(programInfo != nullptr);

    // Parse the fuzzer configurations from the TOML file.
    // Note: The configuration overridings would fail (during the `generateCompilerResult` function
    // call) if any of the configurations are missing or the provided value is less than or equal to
    // 0 (for `int`, `size_t`, and `uint64_t` types).
    toml::parse_result tomlConfig;
    try {
        tomlConfig = toml::parse_file(configFilePath);
    } catch (const toml::parse_error &e) {
        error("P4RuntimeSmith: Failed to parse fuzzer configuration file: %1%", e.what());
    }
    int maxEntryGenCntConfig = tomlConfig["maxEntryGenCnt"].value_or(0);
    int maxAttemptsConfig = tomlConfig["maxAttempts"].value_or(0);
    int maxTablesConfig = tomlConfig["maxTables"].value_or(0);
    std::vector<std::string> tablesToSkipConfig;
    if (const auto *stringRepresentations = tomlConfig["tablesToSkip"].as_array()) {
        for (const auto &stringRepresentation : *stringRepresentations) {
            if (const auto *str = stringRepresentation.as_string()) {
                tablesToSkipConfig.push_back(str->get());
            }
        }
    }
    uint64_t thresholdForDeletionConfig = tomlConfig["thresholdForDeletion"].value_or(0);
    size_t maxUpdateCountConfig = tomlConfig["maxUpdateCount"].value_or(0);
    uint64_t maxUpdateTimeInMicrosecondsConfig =
        tomlConfig["maxUpdateTimeInMicroseconds"].value_or(0);
    uint64_t minUpdateTimeInMicrosecondsConfig =
        tomlConfig["minUpdateTimeInMicroseconds"].value_or(0);

    // Check if the fuzzer configurations are overridden correctly.
    ASSERT_EQ(programInfo->getFuzzerConfig().getMaxEntryGenCnt(), maxEntryGenCntConfig);
    ASSERT_EQ(programInfo->getFuzzerConfig().getMaxAttempts(), maxAttemptsConfig);
    ASSERT_EQ(programInfo->getFuzzerConfig().getMaxTables(), maxTablesConfig);
    for (size_t i = 0; i < tablesToSkipConfig.size(); i++) {
        ASSERT_EQ(programInfo->getFuzzerConfig().getTablesToSkip()[i], tablesToSkipConfig[i]);
    }
    ASSERT_EQ(programInfo->getFuzzerConfig().getThresholdForDeletion(), thresholdForDeletionConfig);
    ASSERT_EQ(programInfo->getFuzzerConfig().getMaxUpdateCount(), maxUpdateCountConfig);
    ASSERT_EQ(programInfo->getFuzzerConfig().getMaxUpdateTimeInMicroseconds(),
              maxUpdateTimeInMicrosecondsConfig);
    ASSERT_EQ(programInfo->getFuzzerConfig().getMinUpdateTimeInMicroseconds(),
              minUpdateTimeInMicrosecondsConfig);
}

// Test of overriding fuzzer configurations via the string representation of the configurations of
// format TOML.
TEST_F(TOMLFuzzerConfigurationTest, OverrideFuzzerConfigurationsViaTOMLString) {
    auto source = generateTestProgram(R"(
    // Drop the packet.
    action acl_drop() {
        mark_to_drop(sm);
    }

    table drop_table {
        key = {
            hdr.eth_hdr.dst_addr : ternary @name("dst_eth");
        }
        actions = {
            acl_drop();
            @defaultonly NoAction();
        }
    }

    apply {
        if (hdr.eth_hdr.isValid()) {
            drop_table.apply();
        }
    })");
    auto autoContext = SetUp("bmv2", "v1model");
    auto &rtSmithOptions = RTSmith::RtSmithOptions::get();
    rtSmithOptions.target = "bmv2"_cs;
    rtSmithOptions.arch = "v1model"_cs;

    const char *configInString = R"(
    maxEntryGenCnt = 10
    maxAttempts = 200
    maxTables = 10
    tablesToSkip = ["table1", "table2"]
    thresholdForDeletion = 50
    maxUpdateCount = 20
    maxUpdateTimeInMicroseconds = 100001
    minUpdateTimeInMicroseconds = 50001
    )";
    rtSmithOptions.setFuzzerConfigString(configInString);
    // Check if the fuzzer configuration string is set (and is set correctly).
    ASSERT_TRUE(rtSmithOptions.fuzzerConfigString().has_value());
    ASSERT_EQ(rtSmithOptions.fuzzerConfigString().value(), configInString);

    auto rtSmithResultOpt = P4::P4Tools::RTSmith::RtSmith::generateConfig(source, rtSmithOptions);
    // Check if the `RtSmithResult` object is generated successfully.
    ASSERT_TRUE(rtSmithResultOpt.has_value());

    auto compilerResult =
        P4::P4Tools::RTSmith::RtSmith::generateCompilerResult(source, rtSmithOptions);
    // Check if the `compilerResult` object is generated successfully.
    ASSERT_TRUE(compilerResult.has_value());

    const auto *programInfo = P4::P4Tools::RTSmith::RtSmithTarget::produceProgramInfo(
        compilerResult.value(), rtSmithOptions);
    // Check if the `programInfo` object is generated successfully.
    ASSERT_TRUE(programInfo != nullptr);

    // Parse the fuzzer configurations from the string.
    // Note: The configuration overridings would fail (during the `generateCompilerResult` function
    // call) if any of the configurations are missing or the provided value is less than or equal to
    // 0 (for `int`, `size_t`, and `uint64_t` types).
    toml::parse_result tomlConfig;
    try {
        tomlConfig = toml::parse(configInString);
    } catch (const toml::parse_error &e) {
        error("P4RuntimeSmith: Failed to parse fuzzer configuration string: %1%", e.what());
    }
    int maxEntryGenCntConfig = tomlConfig["maxEntryGenCnt"].value_or(0);
    int maxAttemptsConfig = tomlConfig["maxAttempts"].value_or(0);
    int maxTablesConfig = tomlConfig["maxTables"].value_or(0);
    std::vector<std::string> tablesToSkipConfig;
    if (const auto *stringRepresentations = tomlConfig["tablesToSkip"].as_array()) {
        for (const auto &stringRepresentation : *stringRepresentations) {
            if (const auto *str = stringRepresentation.as_string()) {
                tablesToSkipConfig.push_back(str->get());
            }
        }
    }
    uint64_t thresholdForDeletionConfig = tomlConfig["thresholdForDeletion"].value_or(0);
    size_t maxUpdateCountConfig = tomlConfig["maxUpdateCount"].value_or(0);
    uint64_t maxUpdateTimeInMicrosecondsConfig =
        tomlConfig["maxUpdateTimeInMicroseconds"].value_or(0);
    uint64_t minUpdateTimeInMicrosecondsConfig =
        tomlConfig["minUpdateTimeInMicroseconds"].value_or(0);

    // Check if the fuzzer configurations are overridden correctly.
    ASSERT_EQ(programInfo->getFuzzerConfig().getMaxEntryGenCnt(), maxEntryGenCntConfig);
    ASSERT_EQ(programInfo->getFuzzerConfig().getMaxAttempts(), maxAttemptsConfig);
    ASSERT_EQ(programInfo->getFuzzerConfig().getMaxTables(), maxTablesConfig);
    for (size_t i = 0; i < tablesToSkipConfig.size(); i++) {
        ASSERT_EQ(programInfo->getFuzzerConfig().getTablesToSkip()[i], tablesToSkipConfig[i]);
    }
    ASSERT_EQ(programInfo->getFuzzerConfig().getThresholdForDeletion(), thresholdForDeletionConfig);
    ASSERT_EQ(programInfo->getFuzzerConfig().getMaxUpdateCount(), maxUpdateCountConfig);
    ASSERT_EQ(programInfo->getFuzzerConfig().getMaxUpdateTimeInMicroseconds(),
              maxUpdateTimeInMicrosecondsConfig);
    ASSERT_EQ(programInfo->getFuzzerConfig().getMinUpdateTimeInMicroseconds(),
              minUpdateTimeInMicrosecondsConfig);
}

}  // anonymous namespace

}  // namespace P4::P4Tools::Test
