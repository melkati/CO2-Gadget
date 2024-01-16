#ifndef CO2_Gadget_Improv_h
#define CO2_Gadget_Improv_h

// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                              SETUP IMPROV FUNCTIONALITY                           *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on

#define MAX_ATTEMPTS_WIFI_CONNECTION 20

uint8_t x_bufferImprov[16];
uint8_t x_positionImprov = 0;

#include "improv.h"

bool connectWifi(std::string ssid, std::string password) {
    uint8_t count = 0;

    WiFi.begin(ssid.c_str(), password.c_str());

    while (WiFi.status() != WL_CONNECTED) {
        // blink_led(500, 1);

        if (count > MAX_ATTEMPTS_WIFI_CONNECTION) {
            WiFi.disconnect();
            return false;
        }
        count++;
    }

    return true;
}

std::vector<std::string> getLocalUrl() {
    return {
        // URL where user can finish onboarding or use device
        // Recommended to use website hosted by device
        String("http://" + WiFi.localIP().toString()).c_str()};
}

void onErrorCallback(improv::Error err) {
    //   blink_led(2000, 3);
}


void set_state(improv::State state) {
    std::vector<uint8_t> data = {'I', 'M', 'P', 'R', 'O', 'V'};
    data.resize(11);
    data[6] = improv::IMPROV_SERIAL_VERSION;
    data[7] = improv::TYPE_CURRENT_STATE;
    data[8] = 1;
    data[9] = state;

    uint8_t checksum = 0x00;
    for (uint8_t d : data)
        checksum += d;
    data[10] = checksum;

    Serial.write(data.data(), data.size());
}

void send_response(std::vector<uint8_t> &response) {
    std::vector<uint8_t> data = {'I', 'M', 'P', 'R', 'O', 'V'};
    data.resize(9);
    data[6] = improv::IMPROV_SERIAL_VERSION;
    data[7] = improv::TYPE_RPC_RESPONSE;
    data[8] = response.size();
    data.insert(data.end(), response.begin(), response.end());

    uint8_t checksum = 0x00;
    for (uint8_t d : data)
        checksum += d;
    data.push_back(checksum);

    Serial.write(data.data(), data.size());
}

void set_error(improv::Error error) {
    std::vector<uint8_t> data = {'I', 'M', 'P', 'R', 'O', 'V'};
    data.resize(11);
    data[6] = improv::IMPROV_SERIAL_VERSION;
    data[7] = improv::TYPE_ERROR_STATE;
    data[8] = 1;
    data[9] = error;

    uint8_t checksum = 0x00;
    for (uint8_t d : data)
        checksum += d;
    data[10] = checksum;

    Serial.write(data.data(), data.size());
}

void getAvailableWifiNetworks() {
    int networkNum = WiFi.scanNetworks();

    for (int id = 0; id < networkNum; ++id) {
        std::vector<uint8_t> data = improv::build_rpc_response(
            improv::GET_WIFI_NETWORKS, {WiFi.SSID(id), String(WiFi.RSSI(id)), (WiFi.encryptionType(id) == WIFI_AUTH_OPEN ? "NO" : "YES")}, false);
        send_response(data);
        delay(1);
    }
    // final response
    std::vector<uint8_t> data =
        improv::build_rpc_response(improv::GET_WIFI_NETWORKS, std::vector<std::string>{}, false);
    send_response(data);
}

bool onCommandCallback(improv::ImprovCommand cmd) {
    switch (cmd.command) {
        case improv::Command::GET_CURRENT_STATE: {
            if ((WiFi.status() == WL_CONNECTED)) {
                set_state(improv::State::STATE_PROVISIONED);
                std::vector<uint8_t> data = improv::build_rpc_response(improv::GET_CURRENT_STATE, getLocalUrl(), false);
                send_response(data);

            } else {
                set_state(improv::State::STATE_AUTHORIZED);
            }

            break;
        }

        case improv::Command::WIFI_SETTINGS: {
            if (cmd.ssid.length() == 0) {
                set_error(improv::Error::ERROR_INVALID_RPC);
                break;
            }

            set_state(improv::STATE_PROVISIONING);

            if (connectWifi(cmd.ssid, cmd.password)) {
                // blink_led(100, 3);

                // TODO: Persist credentials here

                set_state(improv::STATE_PROVISIONED);
                std::vector<uint8_t> data = improv::build_rpc_response(improv::WIFI_SETTINGS, getLocalUrl(), false);
                send_response(data);
                server.begin();

            } else {
                set_state(improv::STATE_STOPPED);
                set_error(improv::Error::ERROR_UNABLE_TO_CONNECT);
            }

            break;
        }

        case improv::Command::GET_DEVICE_INFO: {
            std::vector<std::string> infos = {
                // Firmware name
                "CO2-Gadget-TTGO_TDISPLAY_SANDWICH",
                // Firmware version
                "v8.0.000",
                // Hardware chip/variant
                "ESP32",
                // Device name
                "CO2-Gadget-S"};
            std::vector<uint8_t> data = improv::build_rpc_response(improv::GET_DEVICE_INFO, infos, false);
            send_response(data);
            break;
        }

        case improv::Command::GET_WIFI_NETWORKS: {
            getAvailableWifiNetworks();
            break;
        }

        default: {
            set_error(improv::ERROR_UNKNOWN_RPC);
            return false;
        }
    }

    return true;
}

void improvLoop() {
    if (Serial.available() > 0) {
        uint8_t b = Serial.read();

        if (parse_improv_serial_byte(x_positionImprov, b, x_bufferImprov, onCommandCallback, onErrorCallback)) {
            x_bufferImprov[x_positionImprov++] = b;
        } else {
            x_positionImprov = 0;
        }
    }
}
#endif  // CO2_Gadget_Improv_h