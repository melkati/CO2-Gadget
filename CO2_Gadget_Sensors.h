#include <Sensors.hpp>

#undef I2C_SDA
#undef I2C_SCL
#ifdef ALTERNATIVE_I2C_PINS
#define I2C_SDA 22
#define I2C_SCL 21
#else
#define I2C_SDA 21
#define I2C_SCL 22
#endif

bool pendingCalibration = false;
bool newReadingsAvailable = false;
uint16_t calibrationValue = 415;
uint16_t customCalibrationValue = 415;
bool pendingAmbientPressure = false;
uint16_t ambientPressureValue = 0;
uint16_t altidudeMeters = 600;
bool autoSelfCalibration = false;
float tempOffset = 0.0f;

uint16_t co2 = 0;
float temp, hum = 0;

uint16_t co2OrangeRange =
    700; // Default CO2 ppm concentration threshold to display values in orange
         // (user can change on menu and save on preferences)
uint16_t co2RedRange =
    1000; // Default CO2 ppm concentration threshold to display values in red
          // (user can change on menu and save on preferences)

void onSensorDataOk() {
  if (!inMenu) {
    Serial.print("-->[SENS] CO2: " + sensors.getStringCO2());
    Serial.print(" CO2humi: " + String(sensors.getCO2humi()));
    Serial.print(" CO2temp: " + String(sensors.getCO2temp()));

    Serial.print(" H: " + String(sensors.getHumidity()));
    Serial.println(" T: " + String(sensors.getTemperature()));
  }

  co2 = sensors.getCO2();
  temp = sensors.getCO2temp();
  hum = sensors.getCO2humi();
  newReadingsAvailable = true;
}

void onSensorDataError(const char *msg) { Serial.println(msg); }

void initSensors() {
  
  #ifdef ENABLE_PIN
  // Turn On the Sensor (reserved for future use)
  Serial.println("-->[SENS] Turning on sensor..");
  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, ENABLE_PIN_HIGH);
  delay(50);
  #endif

  // Initialize sensors
  Wire.begin(I2C_SDA, I2C_SCL);

  Serial.println("-->[SENS] Detecting sensors..");

  sensors.setSampleTime(5); // config sensors sample time interval
  sensors.setOnDataCallBack(&onSensorDataOk);     // all data read callback
  sensors.setOnErrorCallBack(&onSensorDataError); // [optional] error callback
  sensors.setDebugMode(false);                     // [optional] debug mode
  sensors.detectI2COnly(true);                    // force to only i2c sensors
  
  sensors.setTempOffset(tempOffset);

  sensors.init();
  if (sensors.isPmSensorConfigured())
    Serial.println("-->[SENS] Sensor configured: " +
                   sensors.getPmDeviceSelected());

  delay(500);

  // sensors.setAutoSelfCalibration(false);
}

void sensorsLoop() {    
  sensors.loop();
}