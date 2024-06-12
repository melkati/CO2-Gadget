// Initialize intervals with default values
let co2Interval = 15000; // 15 seconds
let temperatureInterval = 30000; // 60 seconds
let humidityInterval = 30000; // 60 seconds

// Function to set intervals based on values obtained from the server
function setUpdateIntervals(newCO2Interval) {
    co2Interval = newCO2Interval;
    setInterval(updateCO2Data, co2Interval);
    setInterval(updateTemperatureData, temperatureInterval);
    setInterval(updateHumidityData, humidityInterval);
}

// Function to fetch measurement interval from the server
function updateMeasurementInterval() {
    readMeasurementInterval().then(measurementInterval => {
        console.log('CO2 Measurement Interval', measurementInterval);
        let newCO2Interval = parseInt(measurementInterval) * 1000;
        setUpdateIntervals(newCO2Interval);
    }).catch(error => {
        console.error('Error updating measurement interval', error);
    });
    // fetch('/getMeasurementInterval')
    //     .then(response => response.text())
    //     .then(data => {
    //         console.log('CO2 Measurement Interval', data);
    //         let newCO2Interval = parseInt(data) * 1000;
    //         setUpdateIntervals(newCO2Interval);
    //     })
    //     .catch((error) => {
    //         console.error('Error fetching measurement interval', error);
    //     });
}

/**
 * Updates the CO2 data on the web page.
 * @returns {void}
 */
function updateCO2Data() {
    readCO2Data().then(co2Value => {
        document.querySelector("#CO2Value").textContent = co2Value.toFixed(0);
    }).catch(error => {
        console.error("Error updating CO2 data:", error);
    });
}

/**
 * Updates the temperature data on the web page.
 * @returns {void}
 */
function updateTemperatureData() {
    readTemperatureData().then(temperatureValue => {
        document.querySelector("#TempValue").textContent = temperatureValue;
    }).catch(error => {
        console.error("Error updating temperature data:", error);
    });
}

/**
 * Updates the humidity data on the web page.
 * @returns {void}
 */
function updateHumidityData() {
    readHumidityData().then(humidityValue => {
        document.querySelector("#HumValue").textContent = humidityValue;
    }).catch(error => {
        console.error("Error updating humidity data:", error);
    });
}

document.addEventListener('DOMContentLoaded', function () {
    // Get the current file name
    var currentFileName = window.location.pathname.split("/").pop();

    // Check if the current file is index.html or root
    if (currentFileName === "index.html" || currentFileName === "") {
        updateCO2Data();
        updateTemperatureData();
        updateHumidityData();
        updateMeasurementInterval();
        highlightCurrentPage();
        setInterval(updateCO2Data, co2Interval);
        setInterval(updateTemperatureData, temperatureInterval);
        setInterval(updateHumidityData, humidityInterval);
    }
});
