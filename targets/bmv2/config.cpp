#include "backends/p4tools/modules/p4rtsmith/targets/bmv2/config.h"

namespace P4::P4Tools::RTSmith::V1Model {

void Bmv2V1ModelFuzzerConfig::setMatchKindOpt(const std::string &matchKind) {
    matchKindOpt = matchKind;
}

void Bmv2V1ModelFuzzerConfig::setMatchKindSelector(const std::string &matchKind) {
    matchKindSelector = matchKind;
}

void Bmv2V1ModelFuzzerConfig::setMatchKindRange(const std::string &matchKind) {
    matchKindRange = matchKind;
}

void Bmv2V1ModelFuzzerConfig::setPktInstanceTypeNormal(const uint64_t pktInstanceType) {
    pktInstanceTypeNormal = pktInstanceType;
}

void Bmv2V1ModelFuzzerConfig::setPktInstanceTypeIngressClone(const uint64_t pktInstanceType) {
    pktInstanceTypeIngressClone = pktInstanceType;
}

void Bmv2V1ModelFuzzerConfig::setPktInstanceTypeEgressClone(const uint64_t pktInstanceType) {
    pktInstanceTypeEgressClone = pktInstanceType;
}

void Bmv2V1ModelFuzzerConfig::setPktInstanceTypeCoalesced(const uint64_t pktInstanceType) {
    pktInstanceTypeCoalesced = pktInstanceType;
}

void Bmv2V1ModelFuzzerConfig::setPktInstanceTypeRecirc(const uint64_t pktInstanceType) {
    pktInstanceTypeRecirc = pktInstanceType;
}

void Bmv2V1ModelFuzzerConfig::setPktInstanceTypeReplication(const uint64_t pktInstanceType) {
    pktInstanceTypeReplication = pktInstanceType;
}
void Bmv2V1ModelFuzzerConfig::setPktInstanceTypeResubmit(const uint64_t pktInstanceType) {
    pktInstanceTypeResubmit = pktInstanceType;
}
void Bmv2V1ModelFuzzerConfig::setCloneSessionIdMin(const uint16_t sessionIdMin) {
    cloneSessionIdMin = sessionIdMin;
}
void Bmv2V1ModelFuzzerConfig::setCloneSessionIdMax(const uint16_t sessionIdMax) {
    cloneSessionIdMax = sessionIdMax;
}
void Bmv2V1ModelFuzzerConfig::setCloneType(const std::string cloneType) {
    if (cloneType == "I2E") {
        this->cloneType = V1ModelConstants::CloneType::I2E;
    } else if (cloneType == "E2E") {
        this->cloneType = V1ModelConstants::CloneType::E2E;
    } else {
        ::P4::error("P4RuntimeSmith: Invalid clone type: %1%", cloneType);
    }
}

void Bmv2V1ModelFuzzerConfig::setMeterColor(const std::string meterColor) {
    if (meterColor == "GREEN") {
        this->meterColor = V1ModelConstants::METER_COLOR::GREEN;
    } else if (meterColor == "YELLOW") {
        this->meterColor = V1ModelConstants::METER_COLOR::YELLOW;
    } else if (meterColor == "RED") {
        this->meterColor = V1ModelConstants::METER_COLOR::RED;
    } else {
        ::P4::error("P4RuntimeSmith: Invalid meter color: %1%", meterColor);
    }
}

void Bmv2V1ModelFuzzerConfig::setStfMinPktSize(const int minPktSize) { stfMinPktSize = minPktSize; }

void Bmv2V1ModelFuzzerConfig::setEthHdrSize(const int hdrSize) { ethHdrSize = hdrSize; }

void Bmv2V1ModelFuzzerConfig::setDropPort(const int port) { dropPort = port; }

void Bmv2V1ModelFuzzerConfig::override_fuzzer_configs(const char *path) {
    toml::parse_result tomlConfig;
    try {
        // Note that the parameter fed into the `parse_file` function should be of (or could be
        // converted to) type `std::string_view`.
        tomlConfig = toml::parse_file(path);
    } catch (const toml::parse_error &e) {
        ::P4::error("P4RuntimeSmith: Failed to parse fuzzer configuration file: %1%", e.what());
    }

    // Retrieve the configurations from the TOML file and override the default configurations.
    // TODO(zzmic): Figure out whether this is a desirable engineering practice.
    try {
        const int maxEntriesPerTable = tomlConfig["tables"]["max_entries_per_table"].value_or(0);
        setMaxEntriesPerTable(maxEntriesPerTable);
    } catch (const std::runtime_error &e) {
        /* Ignore. */
    }

    try {
        const int maxTables = tomlConfig["tables"]["max_tables"].value_or(0);
        setMaxTables(maxTables);
    } catch (const std::runtime_error &e) {
        /* Ignore. */
    }

    try {
        std::vector<std::string> tablesToSkip;
        if (const auto *stringRepresentations = tomlConfig["tables"]["tables_to_skip"].as_array()) {
            for (const auto &element : *stringRepresentations) {
                if (const auto *str = element.as_string()) {
                    tablesToSkip.push_back(str->get());
                }
            }
        }
        setTablesToSkip(tablesToSkip);
    } catch (const std::runtime_error &e) {
        /* Ignore. */
    }

    try {
        const std::string matchKindOpt = tomlConfig["match_kind"]["opt"].value_or("");
        setMatchKindOpt(matchKindOpt);
    } catch (const std::runtime_error &e) {
        /* Ignore. */
    }

    try {
        const std::string matchKindSelector = tomlConfig["match_kind"]["selector"].value_or("");
        setMatchKindSelector(matchKindSelector);
    } catch (const std::runtime_error &e) {
        /* Ignore. */
    }

    try {
        const std::string matchKindRange = tomlConfig["match_kind"]["range"].value_or("");
        setMatchKindRange(matchKindRange);
    } catch (const std::runtime_error &e) {
        /* Ignore. */
    }

    try {
        const uint64_t pktInstanceTypeNormal =
            tomlConfig["pkt_instance_type"]["normal"].value_or(0);
        setPktInstanceTypeNormal(pktInstanceTypeNormal);
    } catch (const std::runtime_error &e) {
        /* Ignore. */
    }

    try {
        const uint64_t pktInstanceTypeIngressClone =
            tomlConfig["pkt_instance_type"]["ingress_clone"].value_or(0);
        setPktInstanceTypeIngressClone(pktInstanceTypeIngressClone);
    } catch (const std::runtime_error &e) {
        /* Ignore. */
    }

    try {
        const uint64_t pktInstanceTypeEgressClone =
            tomlConfig["pkt_instance_type"]["egress_clone"].value_or(0);
        setPktInstanceTypeEgressClone(pktInstanceTypeEgressClone);
    } catch (const std::runtime_error &e) {
        /* Ignore. */
    }

    try {
        const uint64_t pktInstanceTypeCoalesced =
            tomlConfig["pkt_instance_type"]["coalesced"].value_or(0);
        setPktInstanceTypeCoalesced(pktInstanceTypeCoalesced);
    } catch (const std::runtime_error &e) {
        /* Ignore. */
    }

    try {
        const uint64_t pktInstanceTypeRecirc =
            tomlConfig["pkt_instance_type"]["recirc"].value_or(0);
        setPktInstanceTypeRecirc(pktInstanceTypeRecirc);
    } catch (const std::runtime_error &e) {
        /* Ignore. */
    }

    try {
        const uint64_t pktInstanceTypeReplication =
            tomlConfig["pkt_instance_type"]["replication"].value_or(0);
        setPktInstanceTypeReplication(pktInstanceTypeReplication);
    } catch (const std::runtime_error &e) {
        /* Ignore. */
    }

    try {
        const uint64_t pktInstanceTypeResubmit =
            tomlConfig["pkt_instance_type"]["resubmit"].value_or(0);
        setPktInstanceTypeResubmit(pktInstanceTypeResubmit);
    } catch (const std::runtime_error &e) {
        /* Ignore. */
    }

    try {
        const uint16_t cloneSessionIdMin = tomlConfig["clone_session_id"]["min"].value_or(0);
        setCloneSessionIdMin(cloneSessionIdMin);
    } catch (const std::runtime_error &e) {
        /* Ignore. */
    }

    try {
        const uint16_t cloneSessionIdMax = tomlConfig["clone_session_id"]["max"].value_or(0);
        setCloneSessionIdMax(cloneSessionIdMax);
    } catch (const std::runtime_error &e) {
        /* Ignore. */
    }

    try {
        const std::string cloneType = tomlConfig["clone_type"].value_or("");
        setCloneType(cloneType);
    } catch (const std::runtime_error &e) {
        /* Ignore. */
    }

    try {
        const std::string meterColor = tomlConfig["meter_color"].value_or("");
        setMeterColor(meterColor);
    } catch (const std::runtime_error &e) {
        /* Ignore. */
    }

    try {
        const int stfMinPktSize = tomlConfig["stf_min_pkt_size"].value_or(0);
        setStfMinPktSize(stfMinPktSize);
    } catch (const std::runtime_error &e) {
        /* Ignore. */
    }

    try {
        const int ethHdrSize = tomlConfig["eth_hdr_size"].value_or(0);
        setEthHdrSize(ethHdrSize);
    } catch (const std::runtime_error &e) {
        /* Ignore. */
    }

    try {
        const int dropPort = tomlConfig["drop_port"].value_or(0);
        setDropPort(dropPort);
    } catch (const std::runtime_error &e) {
        /* Ignore. */
    }
}

}  // namespace P4::P4Tools::RTSmith::V1Model
