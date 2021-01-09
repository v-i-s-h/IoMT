/*
 * jiofi_info.cpp: A HTTP to MQTT bridge for jiofi devices
 */

#include <iostream>
#include <map>

#include "jiofi.hpp"

int main(int argc, char *argv[]) {

    JioFi m2;

    auto r_lte = m2.lte_status();
    std::cout << "=== LTE Status ===\n";
    for(auto k: r_lte) {
        std::cout << k.first << "    "
            << k.second << "\n";
    }
    std::cout << "------------------" << std::endl;

    auto r_lan = m2.lan_info();
    std::cout << "=== LAN Info ===\n";
    for(auto k: r_lan) {
        std::cout << k.first << "    " << k.second << "\n";
    }
    std::cout << "----------------" << std::endl;

    auto r_wan = m2.wan_info();
    std::cout << "=== WAN Info ===\n";
    for(auto k: r_wan) {
        std::cout << k.first << "    " << k.second << "\n";
    }
    std::cout << "----------------" << std::endl;

    auto r_dev = m2.dev_details();
    std::cout << "=== Device Details ===\n";
    for(auto k: r_dev) {
        std::cout << k.first << "    " << k.second << "\n";
    }
    std::cout << "----------------------" << std::endl;

    auto r_sys = m2.sys_perf();
    std::cout << "=== System Performance ===\n";
    for(auto k: r_sys) {
        std::cout << k.first << "    " << k.second << "\n";
    }
    std::cout << "--------------------------" << std::endl;


    return 0;
}
