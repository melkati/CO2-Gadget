// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                            SETUP MQTT FUNCTIONALITY                               *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on
#include <PubSubClient.h>

char charPublish[20];
PubSubClient mqttClient(espClient);

void mqttReconnect() {
  static uint64_t timeStamp = 0;
  uint16_t secondsBetweenRetries = 15; // Keep trying to connect to MQTT broker for 3 minutes (12 times every 15 secs)
  uint16_t maxConnectionRetries = 12;
  static uint16_t connectionRetries = 0;
  if (millis() - timeStamp > (secondsBetweenRetries*1000)) { // Max one try each secondsBetweenRetries*1000 seconds
    timeStamp = millis();
    String subscriptionTopic;
    if (!mqttClient.connected()) {
      Serial.print("Attempting MQTT connection...");
      // Attempt to connect            
      if (mqttClient.connect((mqttClientId).c_str(), (mqttUser).c_str(), (mqttPass).c_str())) {
        Serial.println("connected");
        Serial.print("rootTopic: ");
        Serial.println(rootTopic);
        // Subscribe
        subscriptionTopic = rootTopic + "/calibration";
        mqttClient.subscribe((subscriptionTopic).c_str());
        printf("Suscribing to: %s\n", subscriptionTopic.c_str());
        subscriptionTopic = rootTopic + "/ambientpressure";
        mqttClient.subscribe((subscriptionTopic).c_str());
        printf("Suscribing to: %s\n", subscriptionTopic.c_str());
      } else {
        ++connectionRetries;
        mqttClient.setSocketTimeout(2); //Drop timeout to 2 secs for subsecuents tries
        Serial.printf(" not possible to connect to %s ", mqttBroker.c_str());
        Serial.printf("Connection status:  %d. (%d of %d retries)\n", mqttClient.state(), connectionRetries, maxConnectionRetries); // Possible States: https://pubsubclient.knolleary.net/api#state
        if (connectionRetries >= maxConnectionRetries) {
          activeMQTT = false;
          Serial.println("Max retries to connect to MQTT broker reached, disabling MQTT...");
        }
      }
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
  if (activeMQTT) {
    if (!activeWIFI) {
      activeMQTT = false;
      return;
    }
    if (mqttClient.connected()) {
      mqttClient.disconnect();
    }
    Serial.printf("Initializing MQTT to broker IP: %s\n", mqttBroker.c_str());
    mqttClient.setServer(mqttBroker.c_str(), 1883);
    mqttClient.setCallback(callbackMQTT);
    mqttReconnect();
  }
}

void publishMQTT() {
  if (activeMQTT) {
      if ((WiFi.status() == WL_CONNECTED) && (mqttClient.connected())) {
        publishIntMQTT("/co2", co2);
        publishFloatMQTT("/temp", temp);
        publishFloatMQTT("/humi", hum);
      }
      // Serial.print("Free heap: ");
      // Serial.println(ESP.getFreeHeap());
  }
}

void mqttClientLoop() {
  if (activeMQTT) {
    mqttReconnect(); // Make sure MQTT client is connected
    if (mqttClient.connected()) {
      mqttClient.loop();
    }
  }
}