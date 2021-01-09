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
        std::string lan_info();
        std::string wan_info();
        std::string dev_details();
        std::string sys_perf();
        std::string generic_info();
};

result_map JioFi::lte_status() {
    const std::string cmd_url = "st_lte.w.xml";
    const std::string url = deviceurl + cmd_url;

    // return info_json;
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

/*
std::string JioFi::lan_info() {
    const std::string cmd_url = "st_lan.w.xml";
    const std::string url = deviceurl + cmd_url;

    std::string info_xml = fetch_data(url);
    std::string info_json = xml2json(info_xml.c_str());

    return info_json;
}

std::string JioFi::wan_info() {
    const std::string cmd_url = "st_wan.w.xml";
    const std::string url = deviceurl + cmd_url;

    std::string info_xml = fetch_data(url);
    std::string info_json = xml2json(info_xml.c_str());

    return info_json;
}

std::string JioFi::dev_details() {
    const std::string cmd_url = "st_dev.w.xml";
    const std::string url = deviceurl + cmd_url;

    std::string info_xml = fetch_data(url);
    std::string info_json = xml2json(info_xml.c_str());

    return info_json;
}

std::string JioFi::sys_perf() {
    const std::string cmd_url = "st_per.w.xml";
    const std::string url = deviceurl + cmd_url;

    std::string info_xml = fetch_data(url);
    std::string info_json = xml2json(info_xml.c_str());

    return info_json;
}

std::string JioFi::generic_info() {
    const std::string cmd_url = "mark_title.w.xml";
    const std::string url = deviceurl + cmd_url;

    std::string info_xml = fetch_data(url);
    std::string info_json = xml2json(info_xml.c_str());

    return info_json;
}
*/




