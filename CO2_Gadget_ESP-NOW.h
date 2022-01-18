#ifndef CO2_Gadget_ESP - NOW
#define CO2_Gadget_ESP -NOW

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                           SETUP ESP-NOW FUNCTIONALITY                             *********/
/*********                                                                                   *********/
/*****************************************************************************************************/

#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

bool EspNowInititialized = false;

// Receiver AP MAC: E8:68:E7:0F:08:91
// Receiver STA MAC: E8:68:E7:0F:08:90
// uint8_t broadcastAddress[] = {0xE8, 0x68, 0xE7, 0x0F, 0x08, 0x90};
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Peer info
esp_now_peer_info_t peerInfo;

int cmdNone = 0;  // TO-DO: Change to ENUM if it fits
int cmdCalibration = 1;
int cmdTest = 10;

// Data structure to send
// Must match the receiver structure
typedef struct struct_message {
    int id = ESPNOW_BOARD_ID;
    float temp = 0;
    float hum = 0;
    uint16_t co2 = 0;
    float battery = 0;
    int readingId = 0;
    int command = cmdNone;
    uint16_t parameter = 0;
} struct_message;

// Create two struct_message data structures for esp-now communications
struct_message outgoingReadings;
struct_message incomingReadings;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("-->[ESPN] Last Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
             mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    Serial.print("-->[ESPN] Last Packet Sent to:\t\t");
    Serial.println(macStr);
}

// Callback when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
    memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));

    if (incomingReadings.id == ESPNOW_BOARD_ID) {
        Serial.println("-->[ESPN] Received data for this board via ESP-NOW");
        Serial.printf("-->[ESPN] Received bytes: %u (%u expected)\n", len, sizeof(incomingReadings));

        if (len == sizeof(incomingReadings)) {
            Serial.println("-->[ESPN] Measurements received");
            memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
            Serial.print("-->[ESPN] Bytes received:\t");
            Serial.println(len);
            Serial.print("-->[ESPN] CO2 Level:\t");
            Serial.println(incomingReadings.co2);
            Serial.print("-->[ESPN] Temperature:\t");
            Serial.println(incomingReadings.temp);
            Serial.print("-->[ESPN] Humidity:\t");
            Serial.println(incomingReadings.hum);
            Serial.print("-->[ESPN] Battery:\t");
            Serial.println(incomingReadings.battery);
            Serial.print("-->[ESPN] Command:\t");
            Serial.println(incomingReadings.command);
            Serial.print("-->[ESPN] Parameter:\t");
            Serial.println(incomingReadings.parameter);
        } else {
            Serial.printf("-->[ESPN] ERROR: Incorrect size of received data. Received bytes: %u (%u expected)\n", len, sizeof(incomingReadings));
        }

        if ((incomingReadings.command == cmdCalibration) && (incomingReadings.id == ESPNOW_BOARD_ID)) {
            Serial.println("-->[ESPN] *****************************************************************************");
            Serial.print("-->[ESPN] ***** ");
            Serial.print(incomingReadings.command);
            Serial.print("   Parameter:\t");
            Serial.println(incomingReadings.parameter);
            Serial.println("-->[ESPN] *****************************************************************************");
            calibrationValue = incomingReadings.parameter;
            pendingCalibration = true;
        }
    } else {
        // Serial.printf("Received data via ESP-NOW\nAddressed to ESPNOW_BOARD_ID: %u\n", incomingReadings.id);
        // Serial.printf("Received bytes: %u (%u expected)\n", len, sizeof(incomingReadings));
    }
}

void initESPNow() {
    EspNowInititialized = false;
    if (WiFi.status() == WL_CONNECTED) {
        channelESPNow = WiFi.channel();
        Serial.printf("-->[ESPN] Initializing ESP-NOW in already connected WiFi channel: %u\n", channelESPNow);
    }
    else {
        Serial.printf("-->[ESPN] Initializing ESP-NOW in channel: %u\n", channelESPNow);
    }
        
    esp_wifi_set_channel(channelESPNow, WIFI_SECOND_CHAN_NONE);

    //Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("-->[ESPN] Error initializing ESP-NOW");
        return;
    }

    //Register peer
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = channelESPNow;
    peerInfo.encrypt = false;

    //Add peer
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("-->[ESPN] Failed to add peer");
        return;
    }

    // Register callback function that will be called when data is received
    esp_now_register_recv_cb(OnDataRecv);

    // Register callback function to get the status of Trasnmitted packet
    esp_now_register_send_cb(OnDataSent);

    EspNowInititialized = true;
}

void printESPNowError(esp_err_t result) {
    Serial.print("-->[ESPN] Error ");
    Serial.print(result);
    Serial.println(" sending the data");
    if (result == ESP_ERR_ESPNOW_NOT_INIT) {
        // How did we get so far!!
        Serial.println("-->[ESPN] ESPNOW not Init.");
    } else if (result == ESP_ERR_ESPNOW_ARG) {
        Serial.println("-->[ESPN] Invalid Argument");
    } else if (result == ESP_ERR_ESPNOW_INTERNAL) {
        Serial.println("-->[ESPN] Internal Error");
    } else if (result == ESP_ERR_ESPNOW_NO_MEM) {
        Serial.println("-->[ESPN] ESP_ERR_ESPNOW_NO_MEM");
    } else if (result == ESP_ERR_ESPNOW_NOT_FOUND) {
        Serial.println("-->[ESPN] Peer not found.");
    } else {
        Serial.println("-->[ESPN] Not sure what happened");
    }
}

void publishESPNow() {
    static ulong timeBetweenESPNowSend = 60000;  // Default 60 seconds
    ulong timeLastESPNowSend = 0;
    if ((millis() - timeLastESPNowSend + timeBetweenESPNowSend) > 0) {
        //Set values to send
        outgoingReadings.co2 = co2;
        outgoingReadings.temp = temp;
        outgoingReadings.hum = hum;
        outgoingReadings.battery = battery_voltage;

        // Send message via ESP-NOW
        esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&outgoingReadings, sizeof(outgoingReadings));
        if (result == ESP_OK) {
            // Serial.println("Sent with success");
        } else {
            printESPNowError(result);
        }
        timeLastESPNowSend = millis();
    }
}

#endif  // CO2_Gadget_ESP-NOW
