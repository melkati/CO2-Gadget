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
 * @property {boolean} SUPPORT_BTHOME_BLE - Whether BTHome BLE is supported.
 * @property {boolean} SUPPORT_BUZZER - Whether a buzzer is supported.
 * @property {boolean} SUPPORT_ESPNOW - Whether ESP-NOW is supported.
 * @property {boolean} SUPPORT_MDNS - Whether mDNS is supported.
 * @property {boolean} SUPPORT_MQTT - Whether MQTT is supported.
 * @property {boolean} SUPPORT_MQTT_DISCOVERY - Whether MQTT Discovery is supported.
 * @property {boolean} SUPPORT_OTA - Whether OTA updates are supported.
 * @property {boolean} SUPPORT_LOW_POWER - Whether low power mode is supported.
 * @property {boolean} SUPPORT_CIRCULAR_BUFFER - Whether circular buffer (charts) is supported.
 */
var features = {
    SUPPORT_BLE: false,
    SUPPORT_BTHOME_BLE: false,
    SUPPORT_BUZZER: false,
    SUPPORT_ESPNOW: false,
    SUPPORT_MDNS: false,
    SUPPORT_MQTT: false,
    SUPPORT_MQTT_DISCOVERY: false,
    SUPPORT_OTA: false,
    SUPPORT_LOW_POWER: false,
    SUPPORT_CIRCULAR_BUFFER: false
};

/**
 * Wrapper around fetch() that automatically aborts after timeoutMs milliseconds.
 * Prevents hung requests from blocking the retry logic when the ESP32 is busy
 * (e.g. during an EINK refresh or CO2 measurement cycle).
 * @param {string} url - The URL to fetch.
 * @param {RequestInit} [options] - Standard fetch options.
 * @param {number} [timeoutMs=6000] - Abort timeout in milliseconds.
 * @returns {Promise<Response>}
 */
function fetchWithTimeout(url, options, timeoutMs) {
    if (timeoutMs === undefined) timeoutMs = 6000;
    const controller = new AbortController();
    const timeoutId = setTimeout(() => controller.abort(), timeoutMs);
    const fetchOptions = Object.assign({}, options || {}, { signal: controller.signal });
    return fetch(url, fetchOptions)
        .then(function(response) { clearTimeout(timeoutId); return response; })
        .catch(function(error) { clearTimeout(timeoutId); throw error; });
}

/**
 * Restarts the ESP32 device after user confirmation.
 */
function restartESP32() {
    const isConfirmed = confirm("Are you sure you want to restart the ESP32?");
    if (isConfirmed) {
        if (preferencesDebug) console.log("Restarting ESP32...");
        fetchWithTimeout('/restart', { method: 'GET', headers: { 'Content-Type': 'text/plain' } }, 5000)
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
    fetchWithTimeout('/getFeaturesAsJson', {}, 8000)
        .then(response => response.json())
        .then(data => {
            console.log('Fetching loadFeaturesFromServer successful!');
            features.SUPPORT_BLE = data.BLE !== undefined ? data.BLE : false;
            features.SUPPORT_BTHOME_BLE = data.BTHomeBLE !== undefined ? data.BTHomeBLE : false;
            features.SUPPORT_BUZZER = data.Buzzer !== undefined ? data.Buzzer : false;
            features.SUPPORT_ESPNOW = data.EspNow !== undefined ? data.EspNow : false;
            features.SUPPORT_MDNS = data.mDNS !== undefined ? data.mDNS : false;
            features.SUPPORT_MQTT = data.MQTT !== undefined ? data.MQTT : false;
            features.SUPPORT_MQTT_DISCOVERY = data.MQTTDiscovery !== undefined ? data.MQTTDiscovery : false;
            features.SUPPORT_OTA = data.OTA !== undefined ? data.OTA : false;
            features.SUPPORT_LOW_POWER = data.LowPower !== undefined ? data.LowPower : false;
            features.SUPPORT_CIRCULAR_BUFFER = data.CircularBuffer !== undefined ? data.CircularBuffer : false;
        })
        .catch(error => console.error('Error fetching features:', error));
}

/**
 * Fetches the battery voltage from the server.
 * @returns {Promise<string>} A promise that resolves to the battery voltage as a string.
 * @throws {Error} If the network response is not ok or if there is an error fetching the data.
 */
function readBatteryVoltage() {
    return fetchWithTimeout('/readBatteryVoltage', {}, 5000)
        .then(response => {
            if (!response.ok) throw new Error('Network response was not ok.');
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
    return fetchWithTimeout('/readCO2', {}, 5000)
        .then(response => {
            if (!response.ok) throw new Error('Network response was not ok.');
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
    return fetchWithTimeout('/readTemperature', {}, 5000)
        .then(response => {
            if (!response.ok) throw new Error('Network response was not ok.');
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
    return fetchWithTimeout('/readHumidity', {}, 5000)
        .then(response => {
            if (!response.ok) throw new Error('Network response was not ok.');
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
    return fetchWithTimeout('/getMeasurementInterval', {}, 5000)
        .then(response => {
            if (!response.ok) throw new Error('Network response was not ok.');
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
    return fetchWithTimeout('/getFreeHeap', {}, 5000)
        .then(response => {
            if (!response.ok) throw new Error('Network response was not ok.');
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
    return fetchWithTimeout('/getMinFreeHeap', {}, 5000)
        .then(response => {
            if (!response.ok) throw new Error('Network response was not ok.');
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
    features.SUPPORT_BTHOME_BLE = data.BTHomeBLE !== undefined ? data.BTHomeBLE : false;
    features.SUPPORT_BUZZER = data.Buzzer !== undefined ? data.Buzzer : false;
    features.SUPPORT_ESPNOW = data.EspNow !== undefined ? data.EspNow : false;
    features.SUPPORT_MDNS = data.mDNS !== undefined ? data.mDNS : false;
    features.SUPPORT_MQTT = data.MQTT !== undefined ? data.MQTT : false;
    features.SUPPORT_MQTT_DISCOVERY = data.MQTTDiscovery !== undefined ? data.MQTTDiscovery : false;
    features.SUPPORT_OTA = data.OTA !== undefined ? data.OTA : false;
    features.SUPPORT_LOW_POWER = data.LowPower !== undefined ? data.LowPower : false;
    features.SUPPORT_CIRCULAR_BUFFER = data.CircularBuffer !== undefined ? data.CircularBuffer : false;

    if (captivePortalDebug) console.log('Mapped Features:', features);
}

/**
 * Fetches features as JSON from the server and processes the data.
 * @returns {Promise<void>}
 */
function getFeaturesAsJson() {
    return fetchWithTimeout('/getFeaturesAsJson', {}, 8000)
        .then(response => {
            if (!response.ok) {
                console.error('Response not OK:', response.status, response.statusText);
                throw new Error('Network response was not ok ' + response.statusText);
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
    return fetchWithTimeout('/getVersion', {}, 8000)
        .then(response => {
            if (!response.ok) throw new Error('Network response was not ok.');
            return response.json();
        })
        .then(data => {
            if (captivePortalDebug) console.log('Version information:', data);
            return data;
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
            if (versionInfo.firmBuildDate) versionText += ` \u2014 Built: ${versionInfo.firmBuildDate}`;
            if (versionInfo.firmBuildTime) versionText += ` at ${versionInfo.firmBuildTime}`;
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
 * Uses AbortController to enforce an 8-second timeout so that a hung
 * request (e.g. when the device is busy with a CO2 measurement or EINK
 * render) is rejected promptly and the retry logic can kick in.
 * @returns {Promise<Object>} A promise that resolves to the settings JSON object.
 */
function readPreferencesFromServer() {
    return fetchWithTimeout('/getActualSettingsAsJson', {}, 8000)
        .then(response => {
            if (!response.ok) throw new Error('Network response was not ok.');
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
        const otaBadge = document.getElementById("otaBadge");
        if (otaBadge) otaBadge.classList.remove("hidden");
        const maintenanceSection = document.getElementById("maintenanceSection");
        if (maintenanceSection) maintenanceSection.classList.remove("hidden");
    }

    if (features.SUPPORT_CIRCULAR_BUFFER) {
        const chartsLink = document.getElementById("chartsLink");
        if (chartsLink) {
            chartsLink.classList.remove("hidden");
        } else {
            console.error('Element with ID "chartsLink" not found.')
        }
    }

    const lowPowerIcon = document.getElementById("lightingIcon");
    if (lowPowerIcon) lowPowerIcon.classList.toggle("hidden", !features.SUPPORT_LOW_POWER);
}

/**
 * Handles the low power mode activation.
 */
function goLowPower() {
    if (!features.SUPPORT_LOW_POWER) {
        console.warn('Low power support is not compiled into this firmware.');
        return;
    }
    console.log('Low power mode activated');
    fetchWithTimeout('/goLowPower', { method: 'GET', headers: { 'Content-Type': 'text/plain' } }, 5000)
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
        if (data && data.hostName) document.title += ` (${data.hostName})`;
    }).catch(error => console.warn('Could not get hostName for title:', error));

    // Low power icon click handler (element may not be present in all pages)
    const lightingIcon = document.getElementById('lightingIcon');
    if (lightingIcon) lightingIcon.addEventListener('click', goLowPower);

    initFullscreen();
});

/* =========================================================
   Fullscreen management
   ========================================================= */
const _fsExpandPath  = "M32 32C14.3 32 0 46.3 0 64v96c0 17.7 14.3 32 32 32s32-14.3 32-32V96h64c17.7 0 32-14.3 32-32s-14.3-32-32-32H32zM64 352c0-17.7-14.3-32-32-32s-32 14.3-32 32v96c0 17.7 14.3 32 32 32h96c17.7 0 32-14.3 32-32s-14.3-32-32-32H64V352zM320 32c-17.7 0-32 14.3-32 32s14.3 32 32 32h64v64c0 17.7 14.3 32 32 32s32-14.3 32-32V64c0-17.7-14.3-32-32-32H320zM448 352c0-17.7-14.3-32-32-32s-32 14.3-32 32v64H320c-17.7 0-32 14.3-32 32s14.3 32 32 32h96c17.7 0 32-14.3 32-32V352z";
const _fsCompressPath = "M160 64c0-17.7-14.3-32-32-32s-32 14.3-32 32v64H32c-17.7 0-32 14.3-32 32s14.3 32 32 32h96c17.7 0 32-14.3 32-32V64zM32 320c-17.7 0-32 14.3-32 32s14.3 32 32 32H96v64c0 17.7 14.3 32 32 32s32-14.3 32-32V352c0-17.7-14.3-32-32-32H32zM352 64c0-17.7-14.3-32-32-32s-32 14.3-32 32v96c0 17.7 14.3 32 32 32h96c17.7 0 32-14.3 32-32s-14.3-32-32-32H352V64zM320 320c-17.7 0-32 14.3-32 32v96c0 17.7 14.3 32 32 32s32-14.3 32-32V352h64c17.7 0 32-14.3 32-32s-14.3-32-32-32H320z";

function initFullscreen() {
    const btn    = document.getElementById('iconFullscreen');
    if (!btn) return;
    const fspath = document.getElementById('fullscreenPath');
    const fsicon = document.getElementById('fullscreenIcon');
    const navbar = document.getElementById('navbar');
    let hideTimer = null;

    function updateFsIcon(isFs) {
        if (!fspath || !fsicon) return;
        fsicon.setAttribute('viewBox', '0 0 448 512');
        fspath.setAttribute('d', isFs ? _fsCompressPath : _fsExpandPath);
    }

    function showNavbar() {
        if (!navbar) return;
        navbar.classList.add('fs-nav-visible');
        clearTimeout(hideTimer);
        hideTimer = setTimeout(() => navbar.classList.remove('fs-nav-visible'), 2500);
    }

    btn.addEventListener('click', () => {
        if (!document.fullscreenElement) {
            document.documentElement.requestFullscreen().catch(() => {});
        } else {
            document.exitFullscreen().catch(() => {});
        }
    });

    document.addEventListener('fullscreenchange', () => {
        const isFs = !!document.fullscreenElement;
        updateFsIcon(isFs);
        document.body.classList.toggle('is-fullscreen', isFs);
        if (isFs) {
            showNavbar();
            document.addEventListener('mousemove', showNavbar);
        } else {
            clearTimeout(hideTimer);
            if (navbar) navbar.classList.remove('fs-nav-visible');
            document.removeEventListener('mousemove', showNavbar);
        }
    });
}

/* =========================================================
   Timezone helpers — stored in localStorage as 'co2gadget_tz'
   Value is UTC offset in decimal hours (e.g. 1, -5, 5.5)
   ========================================================= */

/**
 * Returns stored UTC offset in hours, or browser's current UTC offset if not set.
 * @returns {number} UTC offset in decimal hours
 */
function getTzOffsetHours() {
    const stored = localStorage.getItem('co2gadget_tz');
    if (stored !== null && stored !== '') return parseFloat(stored);
    return -new Date().getTimezoneOffset() / 60;
}

/**
 * Returns the millisecond shift needed to convert a UTC timestamp (Date.now())
 * to the stored local timezone for display.
 * @returns {number} offset in ms
 */
function getTzOffsetMs() {
    return getTzOffsetHours() * 3600000;
}

/**
 * Saves a UTC offset (decimal hours) to localStorage and triggers a custom event.
 * @param {string|number} offsetHours
 */
function saveTzOffset(offsetHours) {
    localStorage.setItem('co2gadget_tz', String(offsetHours));
    document.dispatchEvent(new CustomEvent('tzChange', { detail: { offset: parseFloat(offsetHours) } }));
}

/**
 * Formats a UTC epoch timestamp (ms) as 'HH:mm:ss' in the stored timezone.
 * @param {number} utcMs  — pure UTC milliseconds (Date.now())
 * @returns {string}
 */
function formatTimeInTz(utcMs) {
    const pad = n => String(n).padStart(2, '0');
    const d = new Date(utcMs + getTzOffsetMs());
    return pad(d.getUTCHours()) + ':' + pad(d.getUTCMinutes()) + ':' + pad(d.getUTCSeconds());
}

/**
 * Formats a UTC epoch timestamp (ms) as 'yyyy-MM-ddTHH:mm' in the stored timezone.
 * @param {number} utcMs  — pure UTC milliseconds
 * @returns {string}
 */
function formatDatetimeLocalInTz(utcMs) {
    const pad = n => String(n).padStart(2, '0');
    const d = new Date(utcMs + getTzOffsetMs());
    return d.getUTCFullYear() + '-' + pad(d.getUTCMonth() + 1) + '-' + pad(d.getUTCDate()) +
        'T' + pad(d.getUTCHours()) + ':' + pad(d.getUTCMinutes());
}

/**
 * Returns a human-readable UTC offset label, e.g. 'UTC+2' or 'UTC−5'.
 * @param {number} offsetHours
 * @returns {string}
 */
function tzOffsetLabel(offsetHours) {
    const sign = offsetHours >= 0 ? '+' : '−';
    const abs = Math.abs(offsetHours);
    const h = Math.floor(abs);
    const m = Math.round((abs - h) * 60);
    return 'UTC' + sign + String(h) + (m ? ':' + String(m).padStart(2, '0') : '');
}
