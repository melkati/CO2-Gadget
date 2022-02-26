#ifndef CO2_Gadget_ESP_NOW
#define CO2_Gadget_ESP_NOW

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                           SETUP ESP-NOW FUNCTIONALITY                             *********/
/*********                                                                                   *********/
/*****************************************************************************************************/

#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

bool EspNowInititialized = false;
// uint8_t peerESPNowAddress[] = ESPNOW_PEER_MAC_ADDRESS;

// Peer info
esp_now_peer_info_t peerInfo;

// Commands to send/receive via ESP-NOW
int cmdCO2GadgetNone = 0;
int cmdCO2GadgetCalibration = 1;
int cmdCO2GadgetDeepSleepPeriod = 2;
int cmdCO2GadgetTest = 10;

typedef enum : uint8_t {
    MSG_JSON = 0,
    MSG_RESERVED = 1,
    MSG_CO2GADGET = 2,
    MSG_PULSECOUNTER = 3,
    WS_OTHER = 128
} messagetype_t;

// Data structure to send
// Must match the receiver structure
typedef struct struct_message_CO2_Gadget_t {
    messagetype_t messageType = MSG_CO2GADGET;
    int boardID;
    float temp;
    float hum;
    uint16_t co2;
    float battery;
    int readingId;
    int command = cmdCO2GadgetNone;
    uint16_t parameter = 0;
} struct_message_CO2_Gadget_t;

// Create two struct_message data structures for esp-now communications
struct_message_CO2_Gadget_t outgoingReadings;
struct_message_CO2_Gadget_t incomingReadings;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    Serial.printf("-->[ESPN] Last packet sent to %s with status: %s\n", macStr, (status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail"));
}

// Callback when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
    memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));

    if (incomingReadings.boardID == boardIdESPNow) {
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

        if ((incomingReadings.command == cmdCO2GadgetCalibration) && (incomingReadings.boardID == boardIdESPNow)) {
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

void disableESPNow() {
    esp_err_t result = esp_now_deinit();
    if (result == ESP_OK) {
        Serial.println("-->[ESPN] ESP-NOW stoped");
    } else {
        Serial.println("-->[ESPN] Error stoping SP-NOW");
        printESPNowError(result);
    }
    activeESPNOW = false;
    EspNowInititialized = false;
}

void initESPNow() {
    if (!activeESPNOW) return;
    EspNowInititialized = false;
    if ((activeWIFI) && (WiFi.status() == WL_CONNECTED)) {
        channelESPNow = WiFi.channel();
        Serial.printf("-->[ESPN] Initializing ESP-NOW in already connected WiFi channel: %u\n", channelESPNow);
    } else {
        WiFi.mode(WIFI_STA);
        Serial.printf("-->[ESPN] Initializing ESP-NOW in channel: %u\n", channelESPNow);
    }

    esp_wifi_set_channel(channelESPNow, WIFI_SECOND_CHAN_NONE);

    //Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("-->[ESPN] Error initializing ESP-NOW");
        return;
    }

    //Register peer
    memcpy(peerInfo.peer_addr, peerESPNowAddress, 6);
    peerInfo.channel = channelESPNow;
    peerInfo.encrypt = false;

    //Add peer
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("-->[ESPN] Failed to add peer");
        return;
    } else {
        char macStr[18];
        snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X", peerESPNowAddress[0], peerESPNowAddress[1], peerESPNowAddress[2], peerESPNowAddress[3], peerESPNowAddress[4], peerESPNowAddress[5]);
        Serial.printf("-->[ESPN] Added ESP-NOW peer: %s\n", macStr);
    }

    // Register callback function that will be called when data is received
    esp_now_register_recv_cb(OnDataRecv);

    // Register callback function to get the status of Trasnmitted packet
    esp_now_register_send_cb(OnDataSent);

    activeESPNOW = true;
    EspNowInititialized = true;
}

void publishESPNow() {
    if ((!activeESPNOW) || (!EspNowInititialized)) return;
    if (millis() - lastTimeESPNowPublished >= timeBetweenESPNowPublish * 1000) {
        //Set values to send
        outgoingReadings.boardID = boardIdESPNow;
        outgoingReadings.co2 = co2;
        outgoingReadings.temp = temp;
        outgoingReadings.hum = hum;
        outgoingReadings.battery = battery_voltage;
        outgoingReadings.readingId++;

        // Send message via ESP-NOW
        esp_err_t result = esp_now_send(peerESPNowAddress, (uint8_t *)&outgoingReadings, sizeof(outgoingReadings));
        if (result == ESP_OK) {
            Serial.println("-->[ESPN] Sent with success");
        } else {
            printESPNowError(result);
        }
        lastTimeESPNowPublished = millis();
    }
}

#endif  // CO2_Gadget_ESP_NOW
