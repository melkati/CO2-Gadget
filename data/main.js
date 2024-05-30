// Initialize intervals with default values
let co2Interval = 15000; // 15 seconds
let temperatureInterval = 60000; // 60 seconds
let humidityInterval = 60000; // 60 seconds

// Define timers for data update functions
let co2Timer = setInterval(getCO2Data, co2Interval);
let temperatureTimer = setInterval(getTemperatureData, temperatureInterval);
let humidityTimer = setInterval(getHumidityData, humidityInterval);

// Function to set intervals based on values obtained from the server
function setUpdateIntervals() {
    // Clear current intervals
    clearInterval(co2Timer);
    clearInterval(temperatureTimer);
    clearInterval(humidityTimer);
    // Set new intervals
    co2Timer = setInterval(getCO2Data, co2Interval);
    temperatureTimer = setInterval(getTemperatureData, temperatureInterval);
    humidityTimer = setInterval(getHumidityData, humidityInterval);
}

// Function to fetch measurement interval from the server
function getMeasurementInterval() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            console.log('CO2 Measurement Interval', this.responseText);
            co2Interval = parseInt(this.responseText) * 1000;
            setUpdateIntervals();
        }
    };
    xhttp.onerror = function () {
        console.error('Error fetching measurement interval');
    };
    xhttp.open("GET", "getMeasurementInterval", true);
    xhttp.send();
}

function getCO2Data() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if ((this.readyState == 4) && (this.status == 200)) {
            document.querySelector('text#CO2Value').textContent = this.responseText;
        }
    };
    xhttp.onerror = function () {
        console.error('Error fetching CO2 data');
    };
    xhttp.open("GET", "readCO2", true);
    xhttp.send();
}

function getTemperatureData() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("TempValue").innerHTML = this.responseText;
        }
    };
    xhttp.onerror = function () {
        console.error('Error fetching temperature data');
    };
    xhttp.open("GET", "readTemperature", true);
    xhttp.send();
}

function getHumidityData() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("HumValue").innerHTML = this.responseText;
        }
    };
    xhttp.onerror = function () {
        console.error('Error fetching humidity data');
    };
    xhttp.open("GET", "readHumidity", true);
    xhttp.send();
}

document.addEventListener('DOMContentLoaded', function () {
    // Start the process by fetching the measurement intervals and data when the page loads
    getCO2Data();
    getTemperatureData();
    getHumidityData();
    getMeasurementInterval();
});
