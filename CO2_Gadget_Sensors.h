#include <Sensors.hpp>

bool firstCO2SensorInit = true;
volatile bool pendingCalibration = false;
volatile bool newReadingsAvailable = false;
uint16_t calibrationValue = 415;
uint16_t customCalibrationValue = 415;
bool pendingAmbientPressure = false;
uint16_t ambientPressureValue = 0;
uint16_t altidudeMeters = 600;
bool autoSelfCalibration = true;
float tempOffset = 0.0f;
uint16_t readingPM25 = 0.0;

volatile uint16_t co2 = 0;
float temp, tempFahrenheit, hum = 0;

uint16_t co2OrangeRange =
    700; // Default CO2 ppm concentration threshold to display values in orange
         // (user can change on menu and save on preferences)
uint16_t co2RedRange =
    1000; // Default CO2 ppm concentration threshold to display values in red
          // (user can change on menu and save on preferences)

void onSensorDataOk() {
  if (!inMenu) { //if not in the config menu
    Serial.print("-->[SENS] CO2: " + String(sensors.getCO2())); //.getStringCO2()
    Serial.print(" CO2humi: " + String(sensors.getCO2humi()));
    Serial.print(" CO2temp: " + String(sensors.getCO2temp()));
    Serial.print(" H: " + String(sensors.getHumidity()));
    Serial.print(" T: " + String(sensors.getTemperature()));
    Serial.print(" PM1: " + String(sensors.getStringPM1()));
    Serial.print(" PM4: " + String(sensors.getStringPM4()));        
    Serial.print(" PM25: " + String(sensors.getStringPM25()));
    Serial.println(" PM10: " + String(sensors.getStringPM10()));    
  }

  co2 = sensors.getCO2();

  hum = sensors.getHumidity();
  if (hum == 0.0) hum = sensors.getCO2humi();

  temp = sensors.getTemperature();
  if (temp == 0.0) temp = sensors.getCO2temp();  // TO-DO: temp could be 0.0

  tempFahrenheit = (temp * 1.8 + 32);

  readingPM25 = sensors.getPM25();

  newReadingsAvailable = true;
}

void onSensorDataError(const char *msg) { Serial.println(msg); }

void initSensors() {
  const int8_t None = -1, AUTO = 0, MHZ19 = 4, CM1106 = 5, SENSEAIRS8 = 6, DEMO=127;
  if (firstCO2SensorInit) {
    Serial.printf("-->[SENS] Using sensorlib v%s Rev:%d\n", CSL_VERSION, CSL_REVISION);
    firstCO2SensorInit = false;
  }

  displayNotification("starting sensors", notifyInfo);
  
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

  uint16_t defaultCO2MeasurementInterval = 5;  // TO-DO: Move to preferences
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
  //put in other setup for sensors here
  //sensors.calib(true); // TO-DO: Implement in CanAirIO
  // Sensors Lib
// this part below needs to be fixed to not focus on the co2 only
  Serial.printf("-->[SENS] Selected CO2 Sensor: %d\n", selectedCO2Sensor);

  if (selectedCO2Sensor == AUTO) {
    Serial.println("-->[SENS] Trying to init sensors: Auto (I2C)");
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
  } else {
    sensors.detectI2COnly(false);
    sensors.init();
  }

  if (!sensors.getMainDeviceSelected().isEmpty()) {
    Serial.println("-->[SENS] Sensor configured: " +
                   sensors.getMainDeviceSelected());
  }
}

void sensorsLoop() {    
  sensors.loop();
}
