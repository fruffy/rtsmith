#include <gtest/gtest.h>

#include <sstream>

#include <toml++/toml.hpp>

#include "backends/p4tools/modules/p4rtsmith/core/target.h"
#include "backends/p4tools/modules/p4rtsmith/register.h"
#include "backends/p4tools/modules/p4rtsmith/register.h.in"
#include "backends/p4tools/modules/p4rtsmith/rtsmith.h"
#include "backends/p4tools/modules/p4rtsmith/toolname.h"
#include "test/gtest/helpers.h"

namespace P4::P4Tools::Test {

namespace {

using namespace P4::literals;

class RtSmithTest : public testing::Test {
 public:
    [[nodiscard]] static std::optional<std::unique_ptr<AutoCompileContext>> SetUp(
        std::string_view target, std::string_view archName) {
        P4::P4Tools::RTSmith::registerRtSmithTargets();
        /// Set up the appropriate compile context for RtSmith tests.
        /// TODO: Remove this once options are not initialized statically anymore.
        auto ctxOpt = P4::P4Tools::RTSmith::RtSmithTarget::initializeTarget(
            P4::P4Tools::RTSmith::TOOL_NAME, target, archName);

        if (!ctxOpt.has_value()) {
            return std::nullopt;
        }
        return std::make_unique<AutoCompileContext>(ctxOpt.value());
    }
};

std::string generateTestProgram(const char *ingressBlock) {
    std::stringstream templateString;
    templateString << R"p4(
header ethernet_t {
    bit<48> dst_addr;
    bit<48> src_addr;
    bit<16> ether_type;
}
struct Headers {
  ethernet_t eth_hdr;
}
struct Metadata {  }
parser parse(packet_in pkt, out Headers hdr, inout Metadata m, inout standard_metadata_t sm) {
  state start {
      pkt.extract(hdr.eth_hdr);
      transition accept;
  }
}
control ingress(inout Headers hdr, inout Metadata meta, inout standard_metadata_t sm) {)p4"
                   << ingressBlock << R"p4(
}
control egress(inout Headers hdr, inout Metadata meta, inout standard_metadata_t sm) {
  apply {}
}
control deparse(packet_out pkt, in Headers hdr) {
  apply {
    pkt.emit(hdr.eth_hdr);
  }
}
control verifyChecksum(inout Headers hdr, inout Metadata meta) {
  apply {}
}
control computeChecksum(inout Headers hdr, inout Metadata meta) {
  apply {}
}
V1Switch(parse(), verifyChecksum(), ingress(), egress(), computeChecksum(), deparse()) main;
)p4";
    return P4_SOURCE(P4Headers::V1MODEL, templateString.str().c_str());
}

/// Helper methods to build configurations for Optimization Tests.
class P4RuntimeApiTest : public RtSmithTest {};

// Tests for the optimization of various expressions.
TEST_F(P4RuntimeApiTest, GeneratesATestViaTheApi) {
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
    auto rtSmithResultOpt = P4::P4Tools::RTSmith::RtSmith::generateConfig(source, rtSmithOptions);
    ASSERT_TRUE(rtSmithResultOpt.has_value());
}

// Tests for the optimization of various expressions with user-specified fuzzer configurations (by
// ingesting a TOML file).
TEST_F(P4RuntimeApiTest, GeneratesATestViaTheApiWithFuzzerConfig) {
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
    rtSmithOptions.setFuzzerConfigPath("configuration.toml");
    // Check if the fuzzer configuration path is set (and is set correctly).
    ASSERT_TRUE(rtSmithOptions.fuzzerConfigPath().has_value());
    ASSERT_EQ(rtSmithOptions.fuzzerConfigPath().value(), "configuration.toml");
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
    toml::parse_result tomlConfig;
    try {
        tomlConfig = toml::parse_file("configuration.toml");
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

}  // anonymous namespace

}  // namespace P4::P4Tools::Test
