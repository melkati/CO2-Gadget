// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                            SETUP MQTT FUNCTIONALITY                               *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on
#if defined SUPPORT_MQTT
#include <PubSubClient.h>

char charPublish[20];
PubSubClient mqttClient(espClient);
#endif

#ifdef SUPPORT_MQTT
void mqttReconnect() {
  String subscriptionTopic;
  if (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    subscriptionTopic = rootTopic + "/#";
    if (mqttClient.connect((mqttClientId).c_str())) {
      Serial.println("connected");
      // Subscribe
      mqttClient.subscribe((subscriptionTopic).c_str());
      printf("Suscribing to: %s/n", subscriptionTopic.c_str());
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
  String topicToLookAt;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  topicToLookAt = rootTopic + "/calibration";

  if (strcmp(topic, topicToLookAt.c_str()) == 0) {
    calibrationValue = messageTemp.toInt();
    printf("Received calibration command with value %d\n", calibrationValue);
    pendingCalibration = true;    
  }

  topicToLookAt = rootTopic + "/ambientpressure";

  if (strcmp(topic, topicToLookAt.c_str()) == 0) {
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
  mqttReconnect();
#endif
}

void publishMQTT() {
#if defined SUPPORT_MQTT && defined SUPPORT_WIFI
      if ((WiFi.status() == WL_CONNECTED) && (mqttClient.connected())) {
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