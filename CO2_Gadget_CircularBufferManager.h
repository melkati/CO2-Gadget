// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                          SETUP CIRCULAR BUFFER FUNCTIONALITY                      *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
/*****************************************************************************************************/
// clang-format on

#ifndef CO2_Gadget_CircularBufferManager_h
#define CO2_Gadget_CircularBufferManager_h

#include <Arduino.h>

#include <vector>

// Default sampling interval in seconds (default: 10 seconds)
uint16_t sensorSampleInterval = 10;

// Default interval for moving average calculation in seconds (default: 60 seconds)
uint16_t movingAverageInterval = 60;

// Default capacity for the long-term buffer (storing one day of data with 1-minute intervals)
uint16_t longTermBufferCapacity = 1440;

// Buffers to store sensor data for moving average calculation and long-term storage
std::vector<uint16_t> sensorDataBuffer;
std::vector<uint16_t> longTermBuffer;

// Variables to keep track of the last sample time and last average
uint64_t lastSampleTime = 0;
uint16_t lastAverage = 0;
uint16_t samplesPerInterval = movingAverageInterval / sensorSampleInterval;

// Function to calculate the moving average of a vector buffer
uint16_t calculateMovingAverage(const std::vector<uint16_t>& buffer) {
    uint32_t sum = 0;
    size_t count = buffer.size();

#ifdef DEBUG_CIRCULAR_BUFFER
    if (!inMenu) {
        Serial.print("-->[BUFF] Calculating moving average. Buffer size: ");
        Serial.println(count);
    }
#endif

    for (size_t i = 0; i < count; i++) {
        sum += buffer[i];
#ifdef DEBUG_CIRCULAR_BUFFER
        if (!inMenu) {
            Serial.print("-->[BUFF] Adding value: ");
            Serial.println(buffer[i]);
        }
#endif
    }

    if (count == 0) {
        return 0;
    }

    return static_cast<uint16_t>((sum + count / 2) / count);
}

// Function to update the buffer with a new value based on the interval
void updateBuffer() {
    if (millis() - lastSampleTime >= movingAverageInterval * 1000) {
        uint16_t newValue;
        if (sensorDataBuffer.empty()) {
            newValue = lastAverage;  // Use the last average if no new data was received
#ifdef DEBUG_CIRCULAR_BUFFER
            if (!inMenu) {
                Serial.println("-->[BUFF] No new data received. Using last average.");
            }
#endif
        } else {
            newValue = calculateMovingAverage(sensorDataBuffer);
#ifdef DEBUG_CIRCULAR_BUFFER
            if (!inMenu) {
                Serial.print("-->[BUFF] New average calculated: ");
                Serial.println(newValue);
            }
#endif
        }

        // Update the last average
        lastAverage = newValue;

        // Store the calculated average in the long-term buffer
        if (longTermBuffer.size() >= longTermBufferCapacity) {
            longTermBuffer.erase(longTermBuffer.begin());  // Remove the oldest value if the buffer is full
#ifdef DEBUG_CIRCULAR_BUFFER
            if (!inMenu) {
                Serial.println("-->[BUFF] Long-term buffer full. Removing oldest value.");
            }
#endif
        }
        longTermBuffer.push_back(newValue);

        // Clear the buffer for the next interval
        sensorDataBuffer.clear();

        // Update the last sample time
        lastSampleTime = millis();
#ifdef DEBUG_CIRCULAR_BUFFER
        if (!inMenu) {
            Serial.print("-->[BUFF] Interval updated. New average: ");
            Serial.println(newValue);
            Serial.print("-->[BUFF] Data points in long-term buffer: ");
            Serial.println(longTermBuffer.size());
            // Print the entire long-term buffer
            Serial.println("-->[BUFF] Long-term buffer values:");
            for (size_t i = 0; i < longTermBuffer.size(); i++) {
                Serial.print(longTermBuffer[i]);
                Serial.print(" ");
            }
            Serial.println();
        }
#endif
    }
}

// Function to add a new CO2 value to the buffer
void addCO2Value(uint16_t co2) {
    if (sensorDataBuffer.size() >= samplesPerInterval) {
        // Remove the oldest value to make space for the new value
        sensorDataBuffer.erase(sensorDataBuffer.begin());
#ifdef DEBUG_CIRCULAR_BUFFER
        if (!inMenu) {
            Serial.println("-->[BUFF] Buffer full. Removing oldest CO2 value.");
        }
#endif
    }

    sensorDataBuffer.push_back(co2);
#ifdef DEBUG_CIRCULAR_BUFFER
    if (!inMenu) {
        Serial.print("-->[BUFF] Added new CO2 value: ");
        Serial.println(co2);
        Serial.print("-->[BUFF] Current sensor samples buffer size: ");
        Serial.println(sensorDataBuffer.size());
    }
#endif

    // Update the buffer with the new value
    updateBuffer();
}

// Function to set the sampling interval in seconds
void setSensorSampleInterval(uint16_t intervalSeconds) {
    sensorSampleInterval = intervalSeconds;
    samplesPerInterval = movingAverageInterval / sensorSampleInterval;
#ifdef DEBUG_CIRCULAR_BUFFER
    if (!inMenu) {
        Serial.print("-->[BUFF] Sensor sample interval set to ");
        Serial.print(intervalSeconds);
        Serial.println(" seconds");
    }
#endif
}

// Function to set the moving average interval in seconds
void setMovingAverageInterval(uint16_t intervalSeconds) {
    movingAverageInterval = intervalSeconds;
    samplesPerInterval = movingAverageInterval / sensorSampleInterval;
#ifdef DEBUG_CIRCULAR_BUFFER
    if (!inMenu) {
        Serial.print("-->[BUFF] Moving average interval set to ");
        Serial.print(intervalSeconds);
        Serial.println(" seconds");
    }
#endif
}

// Function to set the capacity of the long-term buffer
void setLongTermBufferCapacity(uint16_t capacity) {
    longTermBufferCapacity = capacity;
#ifdef DEBUG_CIRCULAR_BUFFER
    if (!inMenu) {
        Serial.print("-->[BUFF] Long-term buffer capacity set to ");
        Serial.println(capacity);
    }
#endif
}

// Initialize buffers
void initCircularBuffer() {
    sensorDataBuffer.reserve(samplesPerInterval);
    longTermBuffer.reserve(longTermBufferCapacity);
}

/**
 * @brief Returns buffer data as a JSON string
 * @param start The starting index of the data to retrieve
 * @param end The ending index of the data to retrieve
 * @return JSON string of buffer data
 */
String getBufferData(int start, int end) {
    uint64_t currentTime = millis();
    uint64_t intervalDuration = movingAverageInterval * 1000;
    uint64_t lastTimestamp = currentTime - (longTermBuffer.size() - 1) * intervalDuration;

    String data = "{";
    data += "\"start\":" + String(start) + ",";
    data += "\"end\":" + String(end) + ",";
    data += "\"lastTimestamp\":" + String(lastTimestamp) + ",";
    data += "\"intervalDuration\":" + String(intervalDuration) + ",";
    data += "\"data\":[";

    for (int i = start; i <= end; i++) {
        if (i > start) {
            data += ",";
        }
        data += String(longTermBuffer[i]);
    }

    data += "]}";
    return data;
}

/**
 * @brief Returns the current buffer configuration as a JSON string
 * @return JSON string of buffer configuration
 */
String getBufferConfig() {
    String config = "{";
    config += "\"sensorSampleInterval\":" + String(sensorSampleInterval) + ",";
    config += "\"movingAverageInterval\":" + String(movingAverageInterval) + ",";
    config += "\"longTermBufferCapacity\":" + String(longTermBufferCapacity);
    config += "}";
    return config;
}

/**
 * @brief Updates the buffer configuration
 * @param sensorSampleInterval New sample interval in seconds
 * @param movingAverageInterval New moving average interval in seconds
 * @param longTermBufferCapacity New capacity of the long-term buffer
 * @return JSON string of updated buffer configuration
 */
String updateBufferConfig(uint16_t newSensorSampleInterval, uint16_t newMovingAverageInterval, uint16_t newLongTermBufferCapacity) {
    sensorSampleInterval = newSensorSampleInterval;
    movingAverageInterval = newMovingAverageInterval;
    longTermBufferCapacity = newLongTermBufferCapacity;
    samplesPerInterval = movingAverageInterval / sensorSampleInterval;
    return getBufferConfig();
}

#endif  // CO2_Gadget_CircularBufferManager_h
