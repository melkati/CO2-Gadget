// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                            SETUP MQTT FUNCTIONALITY                               *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on
#if defined SUPPORT_MQTT
#include <PubSubClient.h>

String topic;
char charPublish[20];
PubSubClient mqttClient(espClient);
#endif

#ifdef SUPPORT_MQTT
void mqttReconnect() {
  if (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    topic = rootTopic + "/#";
    if (mqttClient.connect((topic).c_str())) {
      Serial.println("connected");
      // Subscribe
      mqttClient.subscribe((topic).c_str());
    } else {
      Serial.println(" not possible to connect");
    }
  }
}

// Function called when data is received via MQTT
void callbackMQTT(char *topic, byte *message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Payload: ");
  String messageTemp;
  String topicTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  if (strcmp(topic, "SCD30/calibration") == 0) {
    calibrationValue = messageTemp.toInt();
    printf("Received calibration command with value %d\n", calibrationValue);
    pendingCalibration = true;    
  }

  if (strcmp(topic, "SCD30/ambientpressure") == 0) {
    ambientPressureValue = messageTemp.toInt();
    printf("Received ambient pressure with value %d\n", ambientPressureValue);
    pendingAmbientPressure = true;    
  }
}

void publishIntMQTT(String topic, int16_t payload) {
  dtostrf(payload, 0, 0, charPublish);
  topic = rootTopic + topic;
  Serial.printf("Publishing %d to ", payload);
  Serial.println("topic: " + topic);
  mqttClient.publish((topic).c_str(), charPublish);
}

void publishFloatMQTT(String topic, float payload) {
  dtostrf(payload, 0, 2, charPublish);
  topic = rootTopic + topic;
  Serial.printf("Publishing %.0f to ", payload);
  Serial.println("topic: " + topic);
  mqttClient.publish((topic).c_str(), charPublish);
}

void initMQTT() {
#ifdef SUPPORT_MQTT
  // char mac_address[16];
  mqttClient.setServer(mqtt_server, 1883);
  mqttClient.setCallback(callbackMQTT);

  // Peding: Create rootTopic as "CO2-Gadget-1f:2a" (UNITHOSTNAME + last two
  // bytes of mac address) rootTopic = UNITHOSTNAME; String WiFiMAC =
  // WiFi.macAddress(); WiFiMAC.replace(F(":"),F(""));
  // WiFiMAC.toCharArray(mac_address, 16);
  // Create client ID from MAC address
  // sprintf_P(rootTopic, PSTR(UNITHOSTNAME"-%s%s"), &mac_address[5],
  // &mac_address[6]); // Fix:: conversion error

  if (rootTopic = "") {
    rootTopic = "CO2-Gadget";
  }
#endif
}

void publishMQTT() {
#if defined SUPPORT_MQTT && defined SUPPORT_WIFI
      if (WiFi.status() == WL_CONNECTED) {
        publishIntMQTT("/co2", co2);
        publishFloatMQTT("/temp", temp);
        publishFloatMQTT("/humi", hum);
      }
      // Serial.print("Free heap: ");
      // Serial.println(ESP.getFreeHeap());
#endif
}

void mqttClientLoop() {
#if defined SUPPORT_MQTT && defined SUPPORT_WIFI
  mqttReconnect(); // Make sure MQTT client is connected
  if (mqttClient.connected()) { 
    mqttClient.loop();
  }
#endif
}
#endif