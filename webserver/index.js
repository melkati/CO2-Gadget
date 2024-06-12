// Initialize intervals with default values
let co2Interval = 15000; // 15 seconds
let temperatureInterval = 30000; // 60 seconds
let humidityInterval = 30000; // 60 seconds

/**
 * Sets the update intervals for CO2, temperature, and humidity data.
 * 
 * @param {number} newCO2Interval - The new interval for updating CO2 data in milliseconds.
 */
function setUpdateIntervals(newCO2Interval) {
    co2Interval = newCO2Interval;
    setInterval(updateCO2Data, co2Interval);
    setInterval(updateTemperatureData, temperatureInterval);
    setInterval(updateHumidityData, humidityInterval);
}

/**
 * Updates the measurement interval for CO2 readings.
 * Reads the current measurement interval, converts it to milliseconds,
 * and sets the update intervals accordingly.
 * 
 * @returns {void}
 */
function updateMeasurementInterval() {
    readMeasurementInterval().then(measurementInterval => {
        console.log('CO2 Measurement Interval', measurementInterval);
        let newCO2Interval = parseInt(measurementInterval) * 1000;
        setUpdateIntervals(newCO2Interval);
    }).catch(error => {
        console.error('Error updating measurement interval', error);
    });
}

/**
 * Updates the stroke color and dash array of an SVG element based on a value and thresholds.
 * @param {number} value - The value to determine the stroke color and dash array.
 * @param {string} elementId - The ID of the SVG element to update.
 * @param {Array} colorRanges - The color ranges array.
 * @param {number} maxValue - The maximum value for calculating the percentage.
 */
function updateStroke(value, elementId, colorRanges, maxValue) {
    const element = document.querySelector(`#${elementId}`);
    if (!element) return;

    let color = 'black'; // Default color in case no match is found
    for (let range of colorRanges) {
        if (value >= range.min && value <= range.max) {
            color = range.color;
            break;
        }
    }

    const percentage = (value / maxValue) * 100;
    element.style.stroke = color;
    element.setAttribute('stroke-dasharray', `${percentage}, 100`);
}

/**
 * Updates the CO2 data on the web page.
 * 
 * @param {number} co2OrangeRange - The threshold value for the orange range.
 * @param {number} co2RedRange - The threshold value for the red range.
 * @returns {Promise<void>} - A promise that resolves when the CO2 data is updated.
 */
function updateCO2Data(co2OrangeRange, co2RedRange) {
    const co2ColorRanges = [
        { min: 0, max: co2OrangeRange, color: 'green' },
        { min: co2OrangeRange, max: co2RedRange, color: 'orange' },
        { min: co2RedRange, max: 2000, color: 'red' }
    ];

    readCO2Data().then(co2Value => {
        document.querySelector("#CO2Value").textContent = co2Value.toFixed(0);
        updateStroke(co2Value, 'co2Circle', co2ColorRanges, 2000);
    }).catch(error => {
        console.error("Error:", error);
    });
}

/**
 * Updates the temperature data on the web page.
 * Reads the temperature data and updates the corresponding elements.
 * @returns {Promise<void>} A promise that resolves when the temperature data is updated.
 */
function updateTemperatureData() {
    const tempColorRanges = [
        { min: 15, max: 18, color: '#0000FF' }, // Blue
        { min: 19, max: 22, color: '#00FFFF' }, // Cyan
        { min: 23, max: 25, color: '#00FF00' }, // Green
        { min: 26, max: 30, color: '#FFFF00' }, // Yellow
        { min: 31, max: 35, color: '#FFA500' }, // Orange
        { min: 36, max: 40, color: '#FF0000' }  // Red
    ];

    readTemperatureData().then(temperatureValue => {
        document.querySelector("#TempValue").textContent = temperatureValue;
        updateStroke(temperatureValue, 'tempCircle', tempColorRanges, 40);
    }).catch(error => {
        console.error("Error:", error);
    });
}

/**
 * Updates the humidity data on the web page.
 * Reads the humidity data, updates the corresponding HTML element,
 * and updates the stroke of a circle based on the humidity value.
 * @returns {void}
 */
function updateHumidityData() {
    const humidityColorRanges = [
        { min: 0, max: 10, color: '#A52A2A' }, // Brown
        { min: 11, max: 20, color: '#FF4500' }, // Dark Orange
        { min: 21, max: 30, color: '#FFA500' }, // Orange
        { min: 31, max: 40, color: '#FFFF00' }, // Yellow
        { min: 41, max: 50, color: '#ADFF2F' }, // Light Green
        { min: 51, max: 60, color: '#00FF00' }, // Green
        { min: 61, max: 70, color: '#00FA9A' }, // Medium Spring Green
        { min: 71, max: 80, color: '#1E90FF' }, // Dodger Blue
        { min: 81, max: 90, color: '#0000FF' }, // Blue
        { min: 91, max: 100, color: '#00008B' } // Dark Blue
    ];

    readHumidityData().then(humidityValue => {
        document.querySelector("#HumValue").textContent = humidityValue;
        updateStroke(humidityValue, 'humiCircle', humidityColorRanges, 100);
    }).catch(error => {
        console.error("Error:", error);
    });
}

function updateCharts(preferences) {
    const { co2OrangeRange, co2RedRange } = preferences;

    updateCO2Data(co2OrangeRange, co2RedRange);
    updateTemperatureData();
    updateHumidityData();
}

document.addEventListener('DOMContentLoaded', function () {
    // Check if the current file is index.html or root
    var currentFileName = window.location.pathname.split("/").pop();
    if (currentFileName === "index.html" || currentFileName === "") {
        readPreferencesFromServer().then(preferences => {
            updateCharts(preferences);
            highlightCurrentPage();

            setInterval(() => updateCharts(preferences), co2Interval);
            setInterval(() => updateTemperatureData(), temperatureInterval);
            setInterval(() => updateHumidityData(), humidityInterval);
        }).catch(error => {
            console.error('Error initializing page:', error);
        });
    }
});

