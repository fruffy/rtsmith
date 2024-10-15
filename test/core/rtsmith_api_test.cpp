#include <gtest/gtest.h>

#include <sstream>

#include "backends/p4tools/modules/p4rtsmith/register.h"
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

}  // anonymous namespace

}  // namespace P4::P4Tools::Test
