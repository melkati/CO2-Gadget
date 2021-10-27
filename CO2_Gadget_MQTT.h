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

  // Loop until we're reconnected
  // while (!mqttClient.connected())
  // {
  //   Serial.print("Attempting MQTT connection...");
  //   // Attempt to connect
  //   topic = rootTopic + "/#";
  //   if (mqttClient.connect((topic).c_str()))
  //   {
  //     Serial.println("connected");
  //     // Subscribe
  //     mqttClient.subscribe((topic).c_str());
  //   }
  //   else
  //   {
  //     Serial.print("failed, rc=");
  //     Serial.print(mqttClient.state());
  //     Serial.println(" try again in 5 seconds");
  //     // Wait 5 seconds before retrying
  //     delay(5000);
  //   }
  // }
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
    printf("Received calibration command with value %d\n", messageTemp.toInt());
    pendingCalibration = true;
    calibrationValue = messageTemp.toInt();
  }

  if (strcmp(topic, "SCD30/ambientpressure") == 0) {
    printf("Received ambient pressure with value %d\n", messageTemp.toInt());
    pendingAmbientPressure = true;
    ambientPressureValue = messageTemp.toInt();
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
#endif