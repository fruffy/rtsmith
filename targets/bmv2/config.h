#ifndef BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_BMV2_CONFIG_H_
#define BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_BMV2_CONFIG_H_

#include "backends/p4tools/common/compiler/compiler_target.h"
#include "backends/p4tools/modules/p4rtsmith/core/config.h"
#include "backends/p4tools/modules/p4rtsmith/core/program_info.h"
#include "backends/p4tools/modules/p4rtsmith/core/target.h"
#include "backends/p4tools/modules/p4rtsmith/targets/bmv2/constants.h"
#include "backends/p4tools/modules/p4rtsmith/targets/bmv2/fuzzer.h"

namespace P4::P4Tools::RTSmith::V1Model {

class Bmv2V1ModelFuzzerConfig : public FuzzerConfig {
 private:
    /// Initialize the default values of the configuration parameters to their corresponding
    /// constants,
    // TODO(zzmic): Verify whether I should use C strings or C++ strings.
    /// Match bits exactly or not at all.
    std::string matchKindOpt = V1ModelConstants::MATCH_KIND_OPT;
    /// A match that is used as an argument for the selector.
    std::string matchKindSelector = V1ModelConstants::MATCH_KIND_SELECTOR;
    /// Entries that can match a range.
    std::string matchKindRange = V1ModelConstants::MATCH_KIND_RANGE;
    /// These definitions are derived from the numerical values of the enum
    /// named "PktInstanceType" in the p4lang/behavioral-model source file
    /// targets/simple_switch/simple_switch.h
    /// https://github.com/p4lang/behavioral-model/blob/main/targets/simple_switch/simple_switch.h#L146
    uint64_t pktInstanceTypeNormal = V1ModelConstants::PKT_INSTANCE_TYPE_NORMAL;
    uint64_t pktInstanceTypeIngressClone = V1ModelConstants::PKT_INSTANCE_TYPE_INGRESS_CLONE;
    uint64_t pktInstanceTypeEgressClone = V1ModelConstants::PKT_INSTANCE_TYPE_EGRESS_CLONE;
    uint64_t pktInstanceTypeCoalesced = V1ModelConstants::PKT_INSTANCE_TYPE_COALESCED;
    uint64_t pktInstanceTypeRecirc = V1ModelConstants::PKT_INSTANCE_TYPE_RECIRC;
    uint64_t pktInstanceTypeReplication = V1ModelConstants::PKT_INSTANCE_TYPE_REPLICATION;
    uint64_t pktInstanceTypeResubmit = V1ModelConstants::PKT_INSTANCE_TYPE_RESUBMIT;
    /// The session IDs for clone are limited to a specific range.
    /// Details: https://github.com/p4lang/PI/pull/588
    uint16_t cloneSessionIdMin = V1ModelConstants::CLONE_SESSION_ID_MIN;
    uint16_t cloneSessionIdMax = V1ModelConstants::CLONE_SESSION_ID_MAX;
    /// Clone type is derived from v1model.p4
    V1ModelConstants::CloneType cloneType = V1ModelConstants::I2E;
    /// Meter colors are defined in v1model.p4
    V1ModelConstants::METER_COLOR meterColor = V1ModelConstants::GREEN;

    /// Other useful constants
    int stfMinPktSize = V1ModelConstants::STF_MIN_PKT_SIZE;
    int ethHdrSize = V1ModelConstants::ETH_HDR_SIZE;
    int dropPort = V1ModelConstants::DROP_PORT;

    /// Additional setters to modify/override the fuzzer configurations.
    void setMatchKindOpt(const std::string &matchKind);
    void setMatchKindSelector(const std::string &matchKind);
    void setMatchKindRange(const std::string &matchKind);
    void setPktInstanceTypeNormal(const uint64_t pktInstanceType);
    void setPktInstanceTypeIngressClone(const uint64_t pktInstanceType);
    void setPktInstanceTypeEgressClone(const uint64_t pktInstanceType);
    void setPktInstanceTypeCoalesced(const uint64_t pktInstanceType);
    void setPktInstanceTypeRecirc(const uint64_t pktInstanceType);
    void setPktInstanceTypeReplication(const uint64_t pktInstanceType);
    void setPktInstanceTypeResubmit(const uint64_t pktInstanceType);
    void setCloneSessionIdMin(const uint16_t sessionIdMin);
    void setCloneSessionIdMax(const uint16_t sessionIdMax);
    void setCloneType(const std::string cloneType);
    void setMeterColor(const std::string meterColor);
    void setStfMinPktSize(const int minPktSize);
    void setEthHdrSize(const int hdrSize);
    void setDropPort(const int port);

 public:
    /// Default constructor w.r.t. the Bmv2 V1Model.
    Bmv2V1ModelFuzzerConfig() = default;

    /// @brief Override the default fuzzer configurations through the TOML file.
    /// @param path The path to the TOML file.
    void override_fuzzer_configs(const char *path);

    /// Additional getters to access the fuzzer configurations.
    [[nodiscard]] std::string getMatchKindOpt() const { return matchKindOpt; }
    [[nodiscard]] std::string getMatchKindSelector() const { return matchKindSelector; }
    [[nodiscard]] std::string getMatchKindRange() const { return matchKindRange; }
    [[nodiscard]] uint64_t getPktInstanceTypeNormal() const { return pktInstanceTypeNormal; }
    [[nodiscard]] uint64_t getPktInstanceTypeIngressClone() const {
        return pktInstanceTypeIngressClone;
    }
    [[nodiscard]] uint64_t getPktInstanceTypeEgressClone() const {
        return pktInstanceTypeEgressClone;
    }
    [[nodiscard]] uint64_t getPktInstanceTypeCoalesced() const { return pktInstanceTypeCoalesced; }
    [[nodiscard]] uint64_t getPktInstanceTypeRecirc() const { return pktInstanceTypeRecirc; }
    [[nodiscard]] uint64_t getPktInstanceTypeReplication() const {
        return pktInstanceTypeReplication;
    }
    [[nodiscard]] uint64_t getPktInstanceTypeResubmit() const { return pktInstanceTypeResubmit; }
    [[nodiscard]] uint16_t getCloneSessionIdMin() const { return cloneSessionIdMin; }
    [[nodiscard]] uint16_t getCloneSessionIdMax() const { return cloneSessionIdMax; }
    [[nodiscard]] std::string getCloneType() const {
        if (cloneType == V1ModelConstants::I2E) {
            return "I2E";
        } else if (cloneType == V1ModelConstants::E2E) {
            return "E2E";
        } else {
            return "UNKNOWN";
        }
    }
    [[nodiscard]] std::string getMeterColor() const {
        if (meterColor == V1ModelConstants::GREEN) {
            return "GREEN";
        } else if (meterColor == V1ModelConstants::YELLOW) {
            return "YELLOW";
        } else if (meterColor == V1ModelConstants::RED) {
            return "RED";
        } else {
            return "UNKNOWN";
        }
    }
    [[nodiscard]] int getStfMinPktSize() const { return stfMinPktSize; }
    [[nodiscard]] int getEthHdrSize() const { return ethHdrSize; }
    [[nodiscard]] int getDropPort() const { return dropPort; }
};

}  // namespace P4::P4Tools::RTSmith::V1Model

#endif /* BACKENDS_P4TOOLS_MODULES_P4RTSMITH_TARGETS_BMV2_CONFIG_H_ */
