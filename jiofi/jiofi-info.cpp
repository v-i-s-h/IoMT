/*
 * jiofi_info.cpp: A HTTP to MQTT bridge for jiofi devices
 */

#include <iostream>
#include <chrono>
#include <thread>
#include <signal.h>
#include <mqtt/async_client.h>

#include "jiofi.hpp"

// Exit clean using CTRL+C
volatile sig_atomic_t stop;

// Callback for handling interrupts
void cb_int_handler(int signum) {
    stop = 1;
}

int main(int argc, char *argv[]) {

    JioFi m2;

    // MQTT messaging
    const std::string broker { "tcp://localhost:1883" };
    const std::string client_id { "jiofi-bridge" };
    const std::string root_topic { "home/devices/jiofi" };

    // create an async client
    mqtt::async_client jiofi_bridge(broker, client_id);

    // install interrupt handler
    signal(SIGINT, cb_int_handler);

    try {

        std::cerr << "Connecting to broker " << broker << " ... ";
        jiofi_bridge.connect()->wait();
        std::cerr << "[DONE]" << std::endl;
        std::string cli_id = jiofi_bridge.get_client_id();
        mqtt::token_ptr tok;

        while(!stop) {
            // mark start of this loop
            auto ts_now = std::chrono::system_clock::now();
            auto ts_next = ts_now + std::chrono::seconds(30);

            // send each of the status messages
            for(auto t: m2.lte_status()) {
                tok = jiofi_bridge.publish(
                    root_topic + "/lte_status/" + t.first, 
                    t.second
                );
            }

            for(auto t: m2.lan_info()) {
                tok = jiofi_bridge.publish(
                    root_topic + "/lan_info/" + t.first,
                    t.second
                );
            }

            for(auto t: m2.wan_info()) {
                tok = jiofi_bridge.publish(
                    root_topic + "/wan_info/" + t.first,
                    t.second
                );
            }

            for(auto t: m2.dev_details()) {
                tok = jiofi_bridge.publish(
                    root_topic + "/dev_details/" + t.first,
                    t.second
                );
            }

            for(auto t: m2.sys_perf()) {
                tok = jiofi_bridge.publish(
                    root_topic + "/sys_perf/" + t.first,
                    t.second
                );
            }

            // wait for sending!
            // tok->wait();

            // wait untill the period ends
            std::this_thread::sleep_until(ts_next);
        }

        std::cerr << "Disconnecting from broker ... ";
        jiofi_bridge.disconnect()->wait();
        std::cerr << "[DONE]" << std::endl;
        
    }
    catch (const mqtt::exception& exc) {
        std::cerr << exc << std::endl;
        return 1;
    }

    return 0;
}
