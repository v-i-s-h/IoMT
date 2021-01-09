/*
 * jiofi_info.cpp: A HTTP to MQTT bridge for jiofi devices
 */

#include <iostream>
#include <map>

#include "jiofi.hpp"

int main(int argc, char *argv[]) {

    JioFi m2;

    result_map r = m2.lte_status();
    for(auto k: r) {
        std::cout << k.first << "    "
            << k.second << std::endl;
    }

    return 0;
}
