// Initialize intervals with default values
let co2Interval = 15000; // 15 seconds
let temperatureInterval = 30000; // 60 seconds
let humidityInterval = 30000; // 60 seconds

// Function to set intervals based on values obtained from the server
function setUpdateIntervals(newCO2Interval) {
    co2Interval = newCO2Interval;
    setInterval(getCO2Data, co2Interval);
    setInterval(getTemperatureData, temperatureInterval);
    setInterval(getHumidityData, humidityInterval);
}

// Function to fetch measurement interval from the server
function getMeasurementInterval() {
    fetch('/getMeasurementInterval')
        .then(response => response.text())
        .then(data => {
            console.log('CO2 Measurement Interval', data);
            let newCO2Interval = parseInt(data) * 1000;
            setUpdateIntervals(newCO2Interval);
        })
        .catch((error) => {
            console.error('Error fetching measurement interval', error);
        });
}

function getCO2Data() {
    fetch('/readCO2')
        .then(response => response.text())
        .then(data => {
            let co2Value = parseFloat(data);
            document.querySelector('text#CO2Value').textContent = co2Value.toFixed(0); // No decimals
        })
        .catch((error) => {
            console.error('Error fetching CO2 data', error);
        });
}

function getTemperatureData() {
    fetch('/readTemperature')
        .then(response => response.text())
        .then(data => {
            let temperatureValue = parseFloat(data);
            document.querySelector('text#TempValue').textContent = temperatureValue.toFixed(1); // One decimal
        })
        .catch((error) => {
            console.error('Error fetching temperature data', error);
        });
}

function getHumidityData() {
    fetch('/readHumidity')
        .then(response => response.text())
        .then(data => {
            let humidityValue = parseFloat(data);
            document.querySelector('text#HumValue').textContent = humidityValue.toFixed(0); // No decimals
        })
        .catch((error) => {
            console.error('Error fetching humidity data', error);
        });
}

document.addEventListener('DOMContentLoaded', function () {
    // Get the current file name
    var currentFileName = window.location.pathname.split("/").pop();

    // Check if the current file is index.html or root
    if (currentFileName === "index.html" || currentFileName === "") {
        getCO2Data();
        getTemperatureData();
        getHumidityData();
        getMeasurementInterval();
        highlightCurrentPage(); // Highlight the current page in the navigation bar
        // setInterval(getCO2Data, co2Interval);
        // setInterval(getTemperatureData, temperatureInterval);
        // setInterval(getHumidityData, humidityInterval);
    }
});
