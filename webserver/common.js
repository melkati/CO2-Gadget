/**
 * Debug flag for captive portal.
 * @type {boolean}
 */
var captivePortalDebug = false;

/**
 * Flag to force captive portal debug mode.
 * @type {boolean}
 */
var forcedCaptivePortalDebug = false;

/**
 * Flag indicating if the captive portal is active.
 * @type {boolean}
 */
var captivePortalActive = false;

/**
 * Flag to force the captive portal to be active.
 * @type {boolean}
 */
var forcedCaptivePortal = false;

/**
 * Flag indicating if the captive portal has no timeout.
 * @type {boolean}
 */
var captivePortalNoTimeout = false;

/**
 * Flag to force the captive portal to have no timeout.
 * @type {boolean}
 */
var forcedCaptivePortalNoTimeout = false;

/**
 * Flag to control relaxed security mode.
 * @type {boolean}
 */
var relaxedSecurity = false;

/**
 * Flag enabled by the URL parameter "relaxedSecurity" to force relaxed security mode.
 * @type {boolean}
 */
var forcedRelaxedSecurity = false;

/**
 * Flag to control captive portal test mode.
 * @type {boolean}
 */
var forceCaptivePortalActive = false;

/**
 * Flag to enable debug output in the console for preferences.
 * @type {boolean}
 */
var preferencesDebug = false;

/**
 * Global object to store the captive portal status.
 * @type {Object}
 * @property {number} captivePortalTimeLeft - Time left for the captive portal.
 */
var captivePortalStatus = {
    captivePortalTimeLeft: 0
};

/**
 * Global object to store the previous state.
 * @type {Object}
 * @property {boolean} forceCaptivePortalActive - Previous state of forceCaptivePortalActive.
 * @property {boolean} captivePortalActive - Previous state of captivePortalActive.
 * @property {boolean} forcedCaptivePortal - Previous state of forcedCaptivePortal.
 * @property {boolean} captivePortalDebug - Previous state of captivePortalDebug.
 * @property {boolean} relaxedSecurity - Previous state of relaxedSecurity.
 * @property {boolean} forcedCaptivePortalDebug - Previous state of forcedCaptivePortalDebug.
 * @property {boolean} captivePortalNoTimeout - Previous state of captivePortalNoTimeout.
 */
var previousData = {
    forceCaptivePortalActive: false,
    captivePortalActive: false,
    forcedCaptivePortal: false,
    captivePortalDebug: false,
    relaxedSecurity: false,
    forcedCaptivePortalDebug: false,
    captivePortalNoTimeout: false
};

/**
 * Global object to store the CO2 Gadget features supported by the device (selected at compile time).
 * @type {Object}
 * @property {boolean} SUPPORT_BLE - Whether BLE is supported.
 * @property {boolean} SUPPORT_BUZZER - Whether a buzzer is supported.
 * @property {boolean} SUPPORT_ESPNOW - Whether ESP-NOW is supported.
 * @property {boolean} SUPPORT_MDNS - Whether mDNS is supported.
 * @property {boolean} SUPPORT_MQTT - Whether MQTT is supported.
 * @property {boolean} SUPPORT_MQTT_DISCOVERY - Whether MQTT Discovery is supported.
 * @property {boolean} SUPPORT_OTA - Whether OTA updates are supported.
 * @property {boolean} SUPPORT_LOW_POWER - Whether low power mode is supported.
 */
var features = {
    SUPPORT_BLE: false,
    SUPPORT_BUZZER: false,
    SUPPORT_ESPNOW: false,
    SUPPORT_MDNS: false,
    SUPPORT_MQTT: false,
    SUPPORT_MQTT_DISCOVERY: false,
    SUPPORT_OTA: false,
    SUPPORT_LOW_POWER: false
};

/**
 * Restarts the ESP32 device after user confirmation.
 */
function restartESP32() {
    const isConfirmed = confirm("Are you sure you want to restart the ESP32?");
    if (isConfirmed) {
        if (preferencesDebug) console.log("Restarting ESP32...");
        fetch('/restart', {
            method: 'GET',
            headers: {
                'Content-Type': 'text/plain'
            }
        })
            .then(response => {
                if (response.ok) {
                    console.log('ESP32 restart initiated');
                } else {
                    throw new Error(`HTTP error! Status: ${response.status}`);
                }
            })
            .catch(error => console.error('Error restarting ESP32:', error));
    }
}

/**
 * Adds the 'active' class to the current page link in the navigation bar.
 */
function highlightCurrentPage() {
    const currentPage = window.location.pathname.split("/").pop();
    const navLinks = document.querySelectorAll(".navbar .nav-content a");

    navLinks.forEach(link => {
        if (link.getAttribute("href") === currentPage) {
            link.classList.add("active");
        }
    });
}

/**
 * Loads features from the server and updates the global features object.
 */
function loadFeaturesFromServer() {
    fetch('/getFeaturesAsJson')
        .then(response => response.json())
        .then(data => {
            console.log('Fetching loadFeaturesFromServer successful!');
            features.SUPPORT_BLE = data.BLE !== undefined ? data.BLE : false;
            features.SUPPORT_BUZZER = data.Buzzer !== undefined ? data.Buzzer : false;
            features.SUPPORT_ESPNOW = data.EspNow !== undefined ? data.EspNow : false;
            features.SUPPORT_MDNS = data.mDNS !== undefined ? data.mDNS : false;
            features.SUPPORT_MQTT = data.MQTT !== undefined ? data.MQTT : false;
            features.SUPPORT_MQTT_DISCOVERY = data.MQTTDiscovery !== undefined ? data.MQTTDiscovery : false;
            features.SUPPORT_OTA = data.OTA !== undefined ? data.OTA : false;
            features.SUPPORT_LOW_POWER = data.LowPower !== undefined ? data.LowPower : false;
        })
        .catch(error => console.error('Error fetching features:', error));
}

/**
 * Fetches the battery voltage from the server.
 * @returns {Promise<string>} A promise that resolves to the battery voltage as a string.
 * @throws {Error} If the network response is not ok or if there is an error fetching the data.
 */
function readBatteryVoltage() {
    return fetch('/readBatteryVoltage')
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok.');
            }
            return response.text();
        })
        .catch(error => {
            console.error('Error fetching battery voltage:', error);
            throw error;
        });
}

/**
 * Fetches CO2 data from the server.
 * @returns {Promise<string>} A promise that resolves to the CO2 value as a string.
 */
function readCO2Data() {
    return fetch("/readCO2")
        .then(response => {
            if (!response.ok) {
                throw new Error("Network response was not ok.");
            }
            return response.text();
        })
        .then(data => parseFloat(data))
        .catch(error => {
            console.error("Error fetching CO2 data:", error);
            throw error;
        });
}

/**
 * Fetches temperature data from the server.
 * @returns {Promise<number>} A promise that resolves to the temperature value.
 */
function readTemperatureData() {
    return fetch("/readTemperature")
        .then(response => {
            if (!response.ok) {
                throw new Error("Network response was not ok.");
            }
            return response.text();
        })
        .then(data => parseFloat(data).toFixed(1))
        .catch(error => {
            console.error("Error fetching temperature data:", error);
            throw error;
        });
}

/**
 * Fetches humidity data from the server.
 * @returns {Promise<string>} A promise that resolves to the humidity value as a string.
 */
function readHumidityData() {
    return fetch("/readHumidity")
        .then(response => {
            if (!response.ok) {
                throw new Error("Network response was not ok.");
            }
            return response.text();
        })
        .then(data => parseFloat(data).toFixed(0))
        .catch(error => {
            console.error("Error fetching humidity data:", error);
            throw error;
        });
}

/**
 * Reads the measurement interval from the server.
 * @returns {Promise<number>} A promise that resolves to the measurement interval in milliseconds.
 * @throws {Error} If the network response is not ok or an error occurs during the fetch operation.
 */
function readMeasurementInterval() {
    return fetch("/getMeasurementInterval")
        .then(response => {
            if (!response.ok) {
                throw new Error("Network response was not ok.");
            }
            return response.text();
        })
        .then(data => parseInt(data) * 1000)
        .catch(error => {
            console.error("Error fetching measurement interval:", error);
            throw error;
        });
}

/**
 * Fetches the free heap memory from the server.
 * @returns {Promise<string>} A promise that resolves to the free heap memory as a string.
 * @throws {Error} If the network response is not ok or if there is an error fetching the data.
 */
function readFreeHeap() {
    return fetch('/getFreeHeap')
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok.');
            }
            return response.text();
        })
        .catch(error => {
            console.error('Error fetching free heap:', error);
            throw error;
        });
}

/**
 * Fetches the minimum free heap memory from the server.
 * @returns {Promise<string>} A promise that resolves to the minimum free heap memory as a string.
 * @throws {Error} If the network response is not ok or if there is an error fetching the data.
 */
function readMinFreeHeap() {
    return fetch('/getMinFreeHeap')
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok.');
            }
            return response.text();
        })
        .catch(error => {
            console.error('Error fetching min free heap:', error);
            throw error;
        });
}

/**
 * Handles the features data and updates the SUPPORT_* properties accordingly.
 * @param {Object} data - The features data object.
 */
function handleFeaturesData(data) {
    features.SUPPORT_BLE = data.BLE !== undefined ? data.BLE : false;
    features.SUPPORT_BUZZER = data.Buzzer !== undefined ? data.Buzzer : false;
    features.SUPPORT_ESPNOW = data.EspNow !== undefined ? data.EspNow : false;
    features.SUPPORT_MDNS = data.mDNS !== undefined ? data.mDNS : false;
    features.SUPPORT_MQTT = data.MQTT !== undefined ? data.MQTT : false;
    features.SUPPORT_MQTT_DISCOVERY = data.MQTTDiscovery !== undefined ? data.MQTTDiscovery : false;
    features.SUPPORT_OTA = data.OTA !== undefined ? data.OTA : false;
    features.SUPPORT_LOW_POWER = data.LowPower !== undefined ? data.LowPower : false;

    if (captivePortalDebug) console.log('Mapped Features:', features);
}

/**
 * Fetches features as JSON from the server and processes the data.
 * @returns {Promise<void>}
 */
function getFeaturesAsJson() {
    return fetch("/getFeaturesAsJson")
        .then(response => {
            if (!response.ok) {
                console.error("Response not OK:", response.status, response.statusText);
                throw new Error("Network response was not ok " + response.statusText);
            }
            return response.json();
        })
        .then(data => {
            console.log("Received JSON:", data);
            handleFeaturesData(data);
        })
        .catch(error => {
            console.error("Error fetching CO2 Gadget features:", error);
        });
}

/**
 * Fetches the version information from the server.
 * @returns {Promise<Object>} A promise that resolves to the version information object.
 * @throws {Error} If the network response is not ok or if there is an error fetching the version.
 */
function fetchVersion() {
    return fetch('/getVersion')
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok.');
            }
            if (captivePortalDebug) console.log('Version information:', response.json());
            return response.json();
        })
        .catch(error => {
            console.error('Error fetching version:', error);
            throw error;
        });
}

/**
 * Retrieves the version string of the firmware.
 * @returns {Promise<string>} A promise that resolves to the version string.
 * @throws {Error} If there is an error getting the version string.
 */
function getVersionStr() {
    return fetchVersion()
        .then(versionInfo => {
            let versionText = `v${versionInfo.firmVerMajor}.${versionInfo.firmVerMinor}.${versionInfo.firmRevision}`;
            if (versionInfo.firmBranch) {
                versionText += `-${versionInfo.firmBranch}`;
            }
            versionText += ` (Flavour: ${versionInfo.firmFlavour})`;
            if (captivePortalDebug) console.log('Version string:', versionText);
            return versionText;
        })
        .catch(error => {
            console.error('Error getting version as string:', error);
            throw error;
        });
}

/**
 * Retrieves a JSON from the server with the current settings
 * and returns a promise that resolves to the JSON object.
 * @returns {Promise<Object>} A promise that resolves to the settings JSON object.
 */
function readPreferencesFromServer() {
    return fetch('/getActualSettingsAsJson')
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok.');
            }
            return response.json();
        })
        .catch(error => {
            console.error('Error fetching preferences:', error);
            throw error;
        });
}

/**
 * Initialize the navbar based on the features supported by the device.
 */
function initNavBar() {
    if (captivePortalDebug)
        console.log("Document loaded. Initializing navbar...");

    if (features.SUPPORT_OTA) {
        const otaLink = document.getElementById("otaLink");
        if (otaLink) {
            otaLink.classList.remove("hidden")
        } else {
            console.error('Element with ID "otaLink" not found.')
        }
    }

    if (features.SUPPORT_LOW_POWER) {
        const lowPowerLink = document.getElementById("lowPowerLink");
        if (lowPowerLink) {
            lowPowerLink.classList.remove("hidden")
        } else {
            console.error('Element with ID "lowPowerLink" not found.')
        }

        const lowPowerIcon = document.getElementById("iconLighting");
        if (lowPowerIcon) {
            lowPowerIcon.classList.remove("hidden");
        } else {
            console.error('Element with ID "iconLighting" not found.')
        }
    }
}

/**
 * Handles the low power mode activation.
 */
function goLowPower() {
    console.log("Low power mode activated");
    fetch('/goLowPower', {
        method: 'GET',
        headers: {
            'Content-Type': 'text/plain'
        }
    })
        .then(response => {
            if (response.ok) {
                console.log('Low power mode activated');
            } else {
                throw new Error(`HTTP error! Status: ${response.status}`);
            }
        })
        .catch(error => console.error('Error activating low power mode:', error));
}

document.addEventListener("DOMContentLoaded", function () {
    getFeaturesAsJson().then(initNavBar);

    // Add device hostName to the existing page title as document.title + (HostName)
    readPreferencesFromServer().then(data => {
        document.title += ` (${data.hostName})`;
    });

    // Llamar a la función goLowPower al hacer clic en el icono de bajo consumo
    document.getElementById('lightingIcon').addEventListener('click', goLowPower);
});
