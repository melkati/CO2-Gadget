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

bool firstCO2SensorInit = true;
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

  hum = sensors.getHumidity();
  if (hum == 0.0) hum = sensors.getCO2humi();

  temp = sensors.getTemperature();
  if (temp == 0.0) temp = sensors.getCO2temp();  // TODO: temp could be 0.0

  newReadingsAvailable = true;
}

void onSensorDataError(const char *msg) { Serial.println(msg); }

void initSensors() {
  const int8_t None = -1, AUTO = 0, MHZ19 = 4, CM1106 = 5, SENSEAIRS8 = 6, FAKE=127;
  if (firstCO2SensorInit) {
    Serial.printf("-->[SENS] Using CanAirIO Sensorlib v%s Rev:%d", CSL_VERSION, CSL_REVISION);
    firstCO2SensorInit = false;
  }
  
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

  uint16_t defaultCO2MeasurementInterval = 5;  // TODO: Move to preferences
// Breaking change: https://github.com/kike-canaries/canairio_sensorlib/pull/110
// CanAirIO Sensorlib was multipliying sample time by two until rev 340 (inclusive). Adjust to avoid need for recalibration.
#ifdef CSL_REVISION  // CanAirIO Sensorlib Revision > 340 (341 where CSL_REVISION was included)
  if (sensors.getLibraryRevision() > 340) {
    sensors.setSampleTime(defaultCO2MeasurementInterval * 2);
  } else {
    sensors.setSampleTime(defaultCO2MeasurementInterval);
  }
#else
  sensors.setSampleTime(defaultCO2MeasurementInterval);
#endif

  sensors.setOnDataCallBack(&onSensorDataOk);      // all data read callback
  sensors.setOnErrorCallBack(&onSensorDataError);  // [optional] error callback
  sensors.setDebugMode(debugSensors);              // [optional] debug mode
  sensors.setTempOffset(tempOffset);
  // sensors.setAutoSelfCalibration(false); // TO-DO: Implement in CanAirIO
  // Sensors Lib

  Serial.printf("Selected CO2 Sensor: %d\n", selectedCO2Sensor);

  if (selectedCO2Sensor == AUTO) {
    Serial.println("-->[SENS] Trying to init CO2 sensor: Auto (I2C)");
    sensors.detectI2COnly(true);
    sensors.init();
  } else if (selectedCO2Sensor == MHZ19) {
    Serial.println("-->[SENS] Trying to init CO2 sensor: MHZ19(A/B/C/D)");
    sensors.detectI2COnly(false);
    sensors.init(MHZ19);
  } else if (selectedCO2Sensor == CM1106) {
    Serial.println("-->[SENS] Trying to init CO2 sensor: CM1106");
    sensors.detectI2COnly(false);
    sensors.init(CM1106);
  } else if (selectedCO2Sensor == SENSEAIRS8) {
    Serial.println("-->[SENS] Trying to init CO2 sensor: SENSEAIRS8");
    sensors.detectI2COnly(false);
    sensors.init(SENSEAIRS8);
  }

  if (!sensors.getMainDeviceSelected().isEmpty()) {
    Serial.println("-->[SENS] Sensor configured: " +
                   sensors.getMainDeviceSelected());
  }
  }

void sensorsLoop() {    
  sensors.loop();
}
