
#include <stdlib.h>

#include <gtest/gtest.h>

#include <sstream>

#include "backends/p4tools/common/compiler/compiler_target.h"
#include "backends/p4tools/common/compiler/context.h"
#include "backends/p4tools/common/core/target.h"
#include "backends/p4tools/common/lib/logging.h"
#include "backends/p4tools/common/options.h"
#include "backends/p4tools/modules/p4rtsmith/register.h"
#include "backends/p4tools/modules/p4rtsmith/rtsmith.h"
#include "backends/p4tools/modules/p4rtsmith/toolname.h"
#include "test/gtest/helpers.h"

namespace Test {

namespace {

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

CompilerOptions generateDefaultApiTestCompilerOptions() {
    auto compilerOptions = CompilerOptions();
    // TODO: Remove the cstring copy.
    compilerOptions.target = cstring("bmv2");
    compilerOptions.arch = cstring("v1model");
    return compilerOptions;
}

P4Tools::RtSmithOptions &generateDefaultApiTestRtSmithOptions() {
    auto &rtsmithOptions = P4Tools::RtSmithOptions::get();

    return rtsmithOptions;
}

/// Helper methods to build configurations for Optimization Tests.
class P4RuntimeApiTest : public ::testing::Test {};

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

    // Initialize the target and the context.
    P4Tools::RTSmith::registerRtSmithTargets();
    auto context = P4Tools::Target::initializeTarget(P4Tools::RTSmith::TOOL_NAME,
                                                     {"", "--target", "bmv2", "--arch", "v1model"});
    AutoCompileContext autoContext(context.value());

    auto compilerOptions = generateDefaultApiTestCompilerOptions();
    auto &rtsmithOptions = generateDefaultApiTestRtSmithOptions();

    auto rtSmithResultOpt =
        P4Tools::RTSmith::RtSmith::generateConfig(source, compilerOptions, rtsmithOptions);
    ASSERT_TRUE(rtSmithResultOpt.has_value());
}

}  // anonymous namespace

}  // namespace Test
