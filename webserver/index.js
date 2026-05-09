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
 * Updates the stroke color and dash array of an SVG path based on a value.
 * Uses getTotalLength() so it works for both arc gauges and normalized circles (pathLength="100").
 * @param {number} value - Current sensor value.
 * @param {string} elementId - ID of the SVG path element.
 * @param {Array} colorRanges - Array of {min, max, color} objects.
 * @param {number} maxValue - Maximum value for scaling.
 */
function updateStroke(value, elementId, colorRanges, maxValue) {
    const element = document.querySelector(`#${elementId}`);
    if (!element) return;

    let color = colorRanges[colorRanges.length - 1].color;
    for (let range of colorRanges) {
        if (value >= range.min && value <= range.max) {
            color = range.color;
            break;
        }
    }

    const totalLen = (typeof element.getTotalLength === 'function') ? element.getTotalLength() : 100;
    const percentage = Math.max(0, Math.min(1, value / maxValue));
    element.style.stroke = color;
    element.style.strokeDasharray = `${percentage * totalLen} ${totalLen}`;
}

/**
 * Returns quality label and CSS-variable color for a CO₂ reading.
 * @param {number} value - CO₂ ppm value.
 * @returns {{text: string, color: string}}
 */
function getCO2Quality(value) {
    if (value < 600)  return { text: 'Excellent', color: 'var(--q-excellent)' };
    if (value < 800)  return { text: 'Good',      color: 'var(--q-good)' };
    if (value < 1000) return { text: 'Moderate',  color: 'var(--q-moderate)' };
    if (value < 1500) return { text: 'Poor',       color: 'var(--q-poor)' };
    return                    { text: 'Very Poor', color: 'var(--q-bad)' };
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
        { min: 0,              max: co2OrangeRange, color: 'var(--q-good)' },
        { min: co2OrangeRange, max: co2RedRange,    color: 'var(--q-moderate)' },
        { min: co2RedRange,    max: Infinity,        color: 'var(--q-bad)' }
    ];

    readCO2Data().then(co2Value => {
        const valueEl = document.querySelector('#CO2Value');
        if (valueEl) valueEl.textContent = co2Value.toFixed(0);
        updateStroke(co2Value, 'co2Circle', co2ColorRanges, 2000);

        const qual = getCO2Quality(co2Value);
        const qualEl = document.querySelector('#co2QualityLabel');
        if (qualEl) { qualEl.textContent = qual.text; qualEl.style.fill = qual.color; }

        const timeEl = document.querySelector('#updateTime');
        if (timeEl) { const d = new Date(); timeEl.textContent = `Updated ${d.toLocaleTimeString()}`; }
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
        { min: -Infinity, max: 18.5, color: 'var(--accent)' },
        { min: 18.5, max: 22.5, color: '#00CFCF' },
        { min: 22.5, max: 25.5, color: 'var(--good)' },
        { min: 25.5, max: 30.5, color: 'var(--caution)' },
        { min: 30.5, max: Infinity, color: 'var(--bad)' }
    ];

    readTemperatureData().then(temperatureValue => {
        const el = document.querySelector('#TempValue');
        if (el) el.textContent = temperatureValue;
        updateStroke(temperatureValue, 'tempCircle', tempColorRanges, 50);
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
        { min: 0,  max: 30,  color: 'var(--bad)' },
        { min: 30, max: 40,  color: 'var(--caution)' },
        { min: 40, max: 60,  color: 'var(--good)' },
        { min: 60, max: 70,  color: 'var(--caution)' },
        { min: 70, max: 100, color: 'var(--accent)' }
    ];

    readHumidityData().then(humidityValue => {
        const el = document.querySelector('#HumValue');
        if (el) el.textContent = humidityValue;
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

