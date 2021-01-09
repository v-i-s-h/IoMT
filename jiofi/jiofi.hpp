/*
 * Header for interacting with JioFi devices through HTTP
 * Supported devices
    - JioFi M2 (Pesagus M2)
 */

#include <iostream>
#include <map>
#include <vector>
#include <rapidxml/rapidxml.hpp>

#include "nw_utils.hpp"

using result_map = std::map<std::string, std::string>;

/*
 * parse_csv: Parse CSV string and returns as list
 */
std::vector<std::string> parse_csv(const std::string datastring) {
    std::vector<std::string> parse_result;
    
    auto string_left = datastring;
    auto delim_pos = string_left.find(",");

    while(delim_pos != std::string::npos) {
        parse_result.push_back(string_left.substr(0, delim_pos));
        string_left = string_left.substr(delim_pos + 1);
        delim_pos = string_left.find(",");
    }
    parse_result.push_back(string_left);

    return parse_result;
}


/*
 *  JioFi; Class for interacting with JioFi devices
 */
class JioFi {
    private:
        std::string deviceurl;
        
    public:
        JioFi(std::string url="http://jiofi.local.html/"):
                deviceurl(url) {}

        result_map lte_status();
        result_map lan_info();
        result_map wan_info();
        result_map dev_details();
        result_map sys_perf();
};

result_map JioFi::lte_status() {
    const std::string cmd_url = "st_lte.w.xml";
    const std::string url = deviceurl + cmd_url;

    // fetch data in XML form
    std::string info_xml = fetch_data(url);

    result_map r;

    rapidxml::xml_document<> xml_data;
    xml_data.parse<0>(&info_xml[0]);

    rapidxml::xml_node<> *rootnode = xml_data.first_node();
    if(rootnode) {
        /*
         * Extract fields
         */
        rapidxml::xml_node<> *node;
        std::string raw_info;
        std::vector<std::string> values;

        // CSPN Info
        node = rootnode->first_node("cspn");
        if(node) {
            r["cspn"] = node->value();
        }

        // Attach Info
        node = rootnode->first_node("attach");
        if(node) {
            r["attach"] = node->value();
        }

        // Time info
        node = rootnode->first_node("time");
        if(node) {
            r["time"] = node->value();
        }

        // APN info
        node = rootnode->first_node("apn");
        if(node) {
            r["apn"] = node->value();
        }

        // Frequency Info
        // Extracted as (UNKN, UNKN, UNKN, UNKN, BandWidth, EARFCN, BandID, OpMode)
        node = rootnode->first_node("freq_info");
        if(node) {
            raw_info = node->value();
            values = parse_csv(raw_info);
            r["freq_info_unkn0"] = values[0];
            r["freq_info_unkn1"] = values[1];
            r["freq_info_unkn2"] = values[2];
            r["freq_info_unkn3"] = values[3];
            r["bandwidth"] = values[4];
            r["earfcn"] = values[5];
            r["band"] = values[6];
            r["mode"] = values[7];
        }
        
        // CellInfo
        // Extracted as (PLMN, Global_Cell_ID, Physical_Cell_ID, ECGI)
        node = rootnode->first_node("cell_node");
        if(node) {
            raw_info = node->value();
            values = parse_csv(raw_info);
            r["plmn"] = values[0];
            r["global_cell_id"] = values[1];
            r["phy_cell_id"] = values[2];
            r["ecgi"] = values[3];
        }

        // RSSI Info
        // Extracted as (RSRP, RSRQ, SINR, UNKN)
        node = rootnode->first_node("rssi_info");
        if(node) {
            raw_info = node->value();
            values = parse_csv(raw_info);
            r["rsrp"] = values[0];
            r["rsrq"] = values[1];
            r["sinr"] = values[2];
            r["rssi_info_unkn0"] = values[3];
        }
    }

    return r;

}

result_map JioFi::lan_info() {
    const std::string cmd_url = "st_lan.w.xml";
    const std::string url = deviceurl + cmd_url;

    std::string info_xml = fetch_data(url);

    result_map r;

    rapidxml::xml_document<> xml_data;
    xml_data.parse<0>(&info_xml[0]);

    rapidxml::xml_node<> *rootnode = xml_data.first_node();
    if(rootnode) {
        /*
         * Extract fields
         */
        rapidxml::xml_node<> *node;

        // SSID Info
        node = rootnode->first_node("ssid");
        if(node) {
            r["ssid"] = node->value();
        }

        // DHCP Status
        node = rootnode->first_node("dhcp");
        if(node) {
            r["dhcp"] = node->value();
        }

        // Subnetmask
        node = rootnode->first_node("submask");
        if(node) {
            r["submask"] = node->value();
        }

        // Gateway
        node = rootnode->first_node("gate");
        if(node) {
            r["gate"] = node->value();
        }

        // MAC Address
        node = rootnode->first_node("macaddr");
        if(node) {
            r["macaddr"] = node->value();
        }

        // User count
        node = rootnode->first_node("user_cnt");
        if(node) {
            r["user_cnt"] = node->value();
        }

        // Channel Selction
        node = rootnode->first_node("ch");
        if(node) {
            r["ch"] = node->value();
        }

        // Channel ID
        node = rootnode->first_node("channel");
        if(node) {
            r["channel"] = node->value();
        }

        // WiFi status
        node = rootnode->first_node("wifi");
        if(node) {
            r["wifi"] = node->value();
        }

        // Bandwidth
        node = rootnode->first_node("band_width");
        if(node) { 
            r["band_width"] = node->value();
        }

        // Security
        node = rootnode->first_node("enc_type");
        if(node) {
            r["enc_type"] = node->value();
        }

        // TODO: User list

    }

    return r;
}

result_map JioFi::wan_info() {
    const std::string cmd_url = "st_wan.w.xml";
    const std::string url = deviceurl + cmd_url;

    // fetch data in XML form
    std::string info_xml = fetch_data(url);

    result_map r;

    rapidxml::xml_document<> xml_data;
    xml_data.parse<0>(&info_xml[0]);

    rapidxml::xml_node<> *rootnode = xml_data.first_node();
    if(rootnode) {
        /*
         * Extract fields
         */
        rapidxml::xml_node<> *node;

        // Total Uplink
        node = rootnode->first_node("t_tx");
        if(node) {
            r["t_tx"] = node->value();
        }

        // Total downlink
        node = rootnode->first_node("t_rx");
        if(node) {
            r["t_rx"] = node->value();
        }

        // Packet loss per session
        node = rootnode->first_node("plps");
        if(node) {
            r["plps"] = node->value();
        }
    }

    return r;
}

result_map JioFi::dev_details() {
    const std::string cmd_url = "st_dev.w.xml";
    const std::string url = deviceurl + cmd_url;

    // fetch data in XML form
    std::string info_xml = fetch_data(url);

    result_map r;

    rapidxml::xml_document<> xml_data;
    xml_data.parse<0>(&info_xml[0]);

    rapidxml::xml_node<> *rootnode = xml_data.first_node();
    if(rootnode) {
        /*
         * Extract fields
         */
        rapidxml::xml_node<> *node;

        // Battery status (dsc)
        node = rootnode->first_node("dsc");
        if(node) {
            r["dsc"] = node->value();
        }

        // Battery percentage
        node = rootnode->first_node("batt_per");
        if(node) {
            r["batt_per"] = node->value();
        }

        // Charging status
        node = rootnode->first_node("batt_st");
        if(node) {
            r["batt_st"] = node->value();
        }
    }

    return r;
}

result_map JioFi::sys_perf() {
    const std::string cmd_url = "st_per.w.xml";
    const std::string url = deviceurl + cmd_url;

    // fetch data in XML form
    std::string info_xml = fetch_data(url);

    result_map r;

    rapidxml::xml_document<> xml_data;
    xml_data.parse<0>(&info_xml[0]);

    rapidxml::xml_node<> *rootnode = xml_data.first_node();
    if(rootnode) {
        /*
         * Extract fields
         */
        rapidxml::xml_node<> *node;
        std::string raw_info;
        std::vector<std::string> values;

        // CPU utilization
        node = rootnode->first_node("cpu");
        if(node) {
            raw_info = node->value();
            values = parse_csv(raw_info);
            r["cpu_usg_min"] = values[0];
            r["cpu_usg_max"] = values[1];
        }

        // Memory usage
        node = rootnode->first_node("mem");
        if(node) {
            raw_info = node->value();
            values = parse_csv(raw_info);
            r["mem_usg_min"] = values[0];
            r["mem_usg_max"] = values[1];
        }

        // Current Uplink speed
        node = rootnode->first_node("curr_tx");
        if(node) {
            r["curr_tx"] = node->value();
        }

        // Max Uplink speed in this session
        node = rootnode->first_node("max_tx");
        if(node) {
            r["max_tx"] = node->value();
        }

        // min Uplink speed in this session
        node = rootnode->first_node("min_tx");
        if(node) {
            r["min_tx"] = node->value();
        }

        // Current downlink speed
        node = rootnode->first_node("curr_rx");
        if(node) {
            r["curr_rx"] = node->value();
        }

        // Max downlink speed
        node = rootnode->first_node("max_rx");
        if(node) {
            r["max_rx"] = node->value();
        }

        // Min downlink speed
        node = rootnode->first_node("min_rx");
        if(node) {
            r["min_rx"] = node->value();
        }
    }

    return r;
}
