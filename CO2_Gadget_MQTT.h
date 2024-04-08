#ifndef CO2_Gadget_MQTT_h
#define CO2_Gadget_MQTT_h

// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                            SETUP MQTT FUNCTIONALITY                               *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on
#ifdef SUPPORT_MQTT
#include <PubSubClient.h>

// Forward declarations
bool publishMQTTDiscovery(int qos);
void mqttClientLoop();

// #define DEBUG_MQTT_SENDING

char charPublish[20];
PubSubClient mqttClient(espClient);
#endif

void mqttReconnect() {
#ifdef SUPPORT_MQTT
    static uint64_t timeStamp = 0;
    uint16_t secondsBetweenRetries = 15;  // Keep trying to connect to MQTT broker for 3 minutes (12 times every 15 secs)
    uint16_t maxMQTTmqttConnectionRetries = 2;
    static uint16_t mqttConnectionRetries = 0;
    if ((!troubledWIFI) && (!troubledMQTT)) {
        if (millis() - timeStamp >= (secondsBetweenRetries * 1000)) {  // Max one try each secondsBetweenRetries*1000 seconds
            timeStamp = millis();
            String subscriptionTopic;
            if (!mqttClient.connected()) {
                Serial.printf("-->[MQTT] Attempting MQTT connection... ");
                // Attempt to connect
                if (mqttClient.connect((mqttClientId).c_str(), (mqttUser).c_str(), (mqttPass).c_str())) {
                    Serial.printf("connected\n");
                    Serial.print("-->[MQTT] rootTopic: ");
                    Serial.println(rootTopic);
                    // Subscribe
                    subscriptionTopic = rootTopic + "/calibration";
                    mqttClient.subscribe((subscriptionTopic).c_str());
                    printf("-->[MQTT] Suscribing to: %s\n", subscriptionTopic.c_str());
                    subscriptionTopic = rootTopic + "/ambientpressure";
                    mqttClient.subscribe((subscriptionTopic).c_str());
                    printf("-->[MQTT] Suscribing to: %s\n", subscriptionTopic.c_str());
                    // Send discovery
                    if (!mqttDiscoverySent) {
                        Serial.printf("-->[MQTT] Connected to broker. Sending discovery...\n");
                        publishMQTTDiscovery(0);
                        mqttDiscoverySent = true;
                    }
                } else {
                    ++mqttConnectionRetries;
                    mqttClient.setSocketTimeout(2);  // Drop timeout to 2 secs for subsecuents tries
                    Serial.printf(" not possible to connect to %s ", mqttBroker.c_str());
                    Serial.printf("Connection status:  %d. (%d of %d retries)\n", mqttClient.state(), mqttConnectionRetries, maxMQTTmqttConnectionRetries);  // Possible States: https://pubsubclient.knolleary.net/api#state
                    if (mqttConnectionRetries >= maxMQTTmqttConnectionRetries) {
                        troubledMQTT = true;
                        timeTroubledMQTT = millis();
                        Serial.printf("-->[MQTT] Max retries to connect to MQTT broker reached, will try later...\n");
                    }
                }
            }
        }
    }
#endif
}

// Function called when data is received via MQTT
void callbackMQTT(char *topic, byte *message, unsigned int length) {
    Serial.print("-->[MQTT] Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Payload: ");
    String messageTemp;
    String topicToLookAt;

    for (int i = 0; i < length; i++) {
        Serial.print((char)message[i]);
        messageTemp += (char)message[i];
    }
    Serial.println();

    topicToLookAt = rootTopic + "/calibration";

    if (strcmp(topic, topicToLookAt.c_str()) == 0) {
        calibrationValue = messageTemp.toInt();
        printf("-->[MQTT] Received calibration command with value %d\n", calibrationValue);
        pendingCalibration = true;
    }

    topicToLookAt = rootTopic + "/ambientpressure";

    if (strcmp(topic, topicToLookAt.c_str()) == 0) {
        ambientPressureValue = messageTemp.toInt();
        printf("-->[MQTT] Received ambient pressure with value %d\n", ambientPressureValue);
        pendingAmbientPressure = true;
    }
}

void publishIntMQTT(String topic, int64_t payload) {
#ifdef SUPPORT_MQTT
    dtostrf(payload, 0, 0, charPublish);
    topic = rootTopic + topic;
    if (!inMenu) {
#ifdef DEBUG_MQTT_SENDING
        Serial.printf("-->[MQTT] Publishing %d to ", payload);
        Serial.println("topic: " + topic);
#endif
    }
    mqttClient.publish((topic).c_str(), charPublish);
#endif
}

void publishFloatMQTT(String topic, float payload) {
#ifdef SUPPORT_MQTT
    dtostrf(payload, 0, 2, charPublish);
    topic = rootTopic + topic;
    if (!inMenu) {
#ifdef DEBUG_MQTT_SENDING
        Serial.printf("-->[MQTT] Publishing %.0f to ", payload);
        Serial.println("topic: " + topic);
#endif
    }
    mqttClient.publish((topic).c_str(), charPublish);
#endif
}

void publishStrMQTT(String topic, String payload) {
#ifdef SUPPORT_MQTT
    topic = rootTopic + topic;
    if (!inMenu) {
#ifdef DEBUG_MQTT_SENDING
        Serial.printf("-->[MQTT] Publishing %s to ", payload.c_str());
        Serial.println("topic: " + topic);
#endif
    }
    mqttClient.publish(topic.c_str(), payload.c_str());
#endif
}

void publishStrDiscoveryMQTT(String topic, String payload, int qos) {
#ifdef SUPPORT_MQTT
    if (!inMenu) {
#ifdef DEBUG_MQTT_SENDING
        Serial.printf("-->[MQTT] Publishing discovery %s to ", payload.c_str());
        Serial.println("topic: " + topic);
#endif
    }
    mqttClient.publish(topic.c_str(), payload.c_str(), true);
#endif
}

bool sendMQTTDiscoveryTopic(String deviceClass, String stateClass, String entityCategory,
                            String group, String field, String name, String icon, String unit,
                            int qos) {
    String version = String(CO2_GADGET_VERSION) + String(CO2_GADGET_REV) + " (" + String(FLAVOUR) + ")";
    String hw_version = String(FLAVOUR);

    String maintopic = String(rootTopic);

    String topicFull;
    String configTopic;
    String payload;

    configTopic = field;

    if (field == "problem") {  // Special binary sensor which is based on error topic
        topicFull = discoveryTopic + "binary_sensor/" + maintopic + "/" + configTopic + "/config";
    } else {
        topicFull = discoveryTopic + "sensor/" + maintopic + "/" + configTopic + "/config";
    }

    /* See https://www.home-assistant.io/docs/mqtt/discovery/ */
    payload = String("{") +
              "\"~\": \"" + maintopic + "\"," +
              "\"unique_id\": \"" + maintopic + "-" + configTopic + "\"," +
              "\"object_id\": \"" + maintopic + "_" + configTopic + "\"," +
              "\"name\": \"" + name + "\"," +
              "\"icon\": \"mdi:" + icon + "\"," +
              "\"unit_of_measurement\": \"" + unit + "\",";

    if (field == "problem") {  // Special binary sensor which is based on error topic
        payload += "\"state_topic\": \"~/error\",";
        payload += "\"value_template\": \"{{ 'OFF' if 'no error' in value else 'ON'}}\",";
    } else {
        payload += "\"state_topic\": \"~/" + field + "\",";
    }

    if (deviceClass != "") {
        payload += "\"device_class\": \"" + deviceClass + "\",";
    }

    if (stateClass != "") {
        payload += "\"state_class\": \"" + stateClass + "\",";
    }

    if (entityCategory != "") {
        payload += "\"entity_category\": \"" + entityCategory + "\",";
    }

    payload += String("\"device\": {") +
               "\"identifiers\": [\"" + maintopic + "\"]," +
               "\"name\": \"" + maintopic + "\"," +
               "\"model\": \"CO2 Gadget\"," +
               "\"manufacturer\": \"emariete.com\"," +
               "\"hw_version\": \"" + hw_version + "\"," +
               "\"sw_version\": \"" + version + "\"," +
               "\"configuration_url\": \"http://" + WiFi.localIP().toString() + "\"" +
               "}" +
               "}";

    publishStrDiscoveryMQTT(topicFull, payload, qos);
    return true;
}

bool publishMQTTDiscovery(int qos) {
#ifdef SUPPORT_MQTT
    bool allSendsSuccessed = false;

    if (!mqttClient.connected()) {
        Serial.println("-->[MQTT] Unable to send MQTT Discovery Topics, we are not connected to the MQTT broker!");
        return false;
    }

    // clang-format off
    // TO-DO: Add MAC Address, Hostname, IP and Status to discovery. Don't know why they are not working (home assistant doesn't show them)
    //
    //                                          Device Class        | State Class       | Entity Category   | Group  | Field        | User Friendly Name    | Icon                      | Unit
    allSendsSuccessed |= sendMQTTDiscoveryTopic("",                 "",                  "diagnostic",       "",      "uptime",      "Uptime",               "clock-time-eight-outline", "s",        qos);
    // allSendsSuccessed |= sendMQTTDiscoveryTopic("",                 "",                  "diagnostic",       "",      "MAC",         "MAC Address",          "network-outline",          "",         qos);
    // allSendsSuccessed |= sendMQTTDiscoveryTopic("",                 "",                  "diagnostic",       "",      "hostname",    "Hostname",             "network-outline",          "",         qos);
    allSendsSuccessed |= sendMQTTDiscoveryTopic("",                 "measurement",       "diagnostic",       "",      "freeMem",     "Free Memory",          "memory",                   "B",        qos);
    allSendsSuccessed |= sendMQTTDiscoveryTopic("",                 "",                  "diagnostic",       "",      "wifiRSSI",    "Wi-Fi RSSI",           "wifi",                     "dBm",      qos);
    // allSendsSuccessed |= sendMQTTDiscoveryTopic("",                 "",                  "diagnostic",       "",      "IP",          "IP",                   "network-outline",          "",         qos);
    // allSendsSuccessed |= sendMQTTDiscoveryTopic("",                 "",                  "diagnostic",       "",      "status",      "Status",               "list-status",              "",         qos);
    allSendsSuccessed |= sendMQTTDiscoveryTopic("",                 "measurement",       "diagnostic",       "",      "battery",     "Battery",              "",                         "%",        qos);
    allSendsSuccessed |= sendMQTTDiscoveryTopic("",                 "measurement",       "diagnostic",       "",      "voltage",     "Voltage",              "",                         "V",        qos);

    allSendsSuccessed |= sendMQTTDiscoveryTopic("carbon_dioxide",   "",                  "",                "",      "co2",         "CO2",                  "molecule-co2",                         "ppm",      qos);
    allSendsSuccessed |= sendMQTTDiscoveryTopic("temperature",      "",                  "",                "",      "temp",        "Temperature",          "temperature-celsius",                  "°C",       qos);
    allSendsSuccessed |= sendMQTTDiscoveryTopic("humidity",         "",                  "",                "",      "humi",        "Humidity",             "water-percent",                        "%",        qos);
    // allSendsSuccessed |= sendMQTTDiscoveryTopic("",              "",                  "diagnostic",       "",      "error",       "Error",                "alert-circle-outline",     "",         qos);
    // allSendsSuccessed |= sendMQTTDiscoveryTopic("",              "",                  "diagnostic",       "",      "json",        "JSON",                 "code-json",                "",         qos);
    // allSendsSuccessed |= sendMQTTDiscoveryTopic("",              "",                  "",                 "",      "problem",     "Problem",              "alert-outline",            "",         qos);  // Special binary sensor which is based on error topic
    // clang-format on

    Serial.println("-->[MQTT] Successfully published all MQTT Discovery topics");
    delay(10);
    return allSendsSuccessed;
#endif
}

void initMQTT() {
#ifdef SUPPORT_MQTT
    if (activeMQTT) {
        if (!activeWIFI) {
            activeMQTT = false;
            return;
        }
        if (mqttClient.connected()) {
            mqttClient.disconnect();
        }
        Serial.printf("-->[MQTT] Initializing MQTT to broker IP: %s\n", mqttBroker.c_str());
        mqttClient.setServer(mqttBroker.c_str(), 1883);
        mqttClient.setCallback(callbackMQTT);
        mqttClient.setBufferSize(1024);

        mqttReconnect();
        mqttClientLoop();
    }
#endif
}

void publishMQTTLogData(String logData) {
#ifdef SUPPORT_MQTT
    // if (activeMQTT && !troubledMQTT && !troubledWIFI && (WiFi.status() == WL_CONNECTED) && mqttClient.connected()) {
    //     publishStrMQTT("/log", logData);
    // }
#endif
}

void publishMQTTAlarms() {
    static bool MQTTGreenAlarm, MQTTOrangeAlarm, MQTTRedAlarm = false;

    if ((co2 >= co2OrangeRange) && (MQTTGreenAlarm)) {
        MQTTGreenAlarm = false;
        publishStrMQTT("/green", "OFF");
    }
    if ((co2 < co2OrangeRange) && (!MQTTGreenAlarm)) {
        MQTTGreenAlarm = true;
        publishStrMQTT("/green", "ON");
    }
    if ((co2 >= co2OrangeRange) && (!MQTTOrangeAlarm)) {
        MQTTOrangeAlarm = true;
        publishStrMQTT("/orange", "ON");
    }
    if ((co2 < co2OrangeRange - PIN_HYSTERESIS) && (MQTTOrangeAlarm)) {
        MQTTOrangeAlarm = false;
        publishStrMQTT("/orange", "OFF");
    }
    if ((co2 > co2RedRange) && (!MQTTRedAlarm)) {
        MQTTRedAlarm = true;
        publishStrMQTT("/red", "ON");
    }
    if ((co2 <= co2RedRange - PIN_HYSTERESIS) && (MQTTRedAlarm)) {
        MQTTRedAlarm = false;
        publishStrMQTT("/red", "OFF");
    }
}

void publishMQTTSystemData() {
    publishIntMQTT("/uptime", millis() / 1000);
    publishFloatMQTT("/voltage", batteryVoltage);
    publishIntMQTT("/battery", batteryLevel);
    publishIntMQTT("/freeMem", ESP.getFreeHeap());
    publishIntMQTT("/wifiRSSI", WiFi.RSSI());
    publishStrMQTT("/IP", WiFi.localIP().toString());
    publishStrMQTT("/MAC", WiFi.macAddress());
    publishStrMQTT("/hostname", hostName);
    publishStrMQTT("/status", "OK");
}

void publishMeasurementsMQTT() {
    publishIntMQTT("/co2", co2);
    publishFloatMQTT("/temp", temp);
    publishFloatMQTT("/humi", hum);
}

void publishMQTT(bool forcePublish = false) {
#ifdef SUPPORT_MQTT
    if (activeMQTT && !troubledMQTT && !troubledWIFI && (WiFi.status() == WL_CONNECTED) && mqttClient.connected()) {
        if ((forcePublish) || ((millis() - lastTimeMQTTPublished >= timeBetweenMQTTPublish * 1000) || (millis() - lastTimeMQTTPublished >= timeToKeepAliveMQTT * 1000) || (lastTimeMQTTPublished == 0))) {
            publishMeasurementsMQTT();
            publishMQTTAlarms();
            publishMQTTSystemData();
            lastTimeMQTTPublished = millis();
        }
    }
#endif
}

void mqttClientLoop() {
#ifdef SUPPORT_MQTT
    if (troubledMQTT && (millis() - timeTroubledMQTT >= timeToRetryTroubledMQTT * 1000)) {
        troubledMQTT = false;
        timeTroubledMQTT = 0;
    }

    if (activeMQTT && !troubledMQTT && !troubledWIFI && (WiFi.status() == WL_CONNECTED)) {
        mqttReconnect();
        if (mqttClient.connected()) {
            mqttClient.loop();
        }
    }

    if (activeMQTT && !mqttDiscoverySent && !troubledMQTT && !troubledWIFI && (WiFi.status() == WL_CONNECTED) && mqttClient.connected()) {
        // Serial.printf("-->[MQTT] Connected to broker. Sending discovery...\n");
        publishMQTTDiscovery(0);
        mqttDiscoverySent = true;
    }
#endif
}

#endif  // CO2_Gadget_MQTT_h