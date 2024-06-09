var captivePortalDebug = false;
var forcedCaptivePortalDebug = false;
var captivePortalActive = false;
var forcedCaptivePortal = false;
var captivePortalNoTimeout = false;
var forcedCaptivePortalNoTimeout = false;
var relaxedSecurity = false; // Flag to control relaxed security mode
var forcedRelaxedSecurity = false; // Enadled by url parameter "relaxedSecurity"

// Global variable to control captive portal test mode
var forceCaptivePortalActive = false; // Flag to indicate url parameters include "forceCaptivePortalActive"
var preferencesDebug = false; // Flag to enable debug output in the console

// Global variables to store the captive portal status
var captivePortalStatus = {
    captivePortalTimeLeft: 0
};

// Global variables to store the previous state
var previousData = {
    forceCaptivePortalActive: false,
    captivePortalActive: false,
    forcedCaptivePortal: false,
    captivePortalDebug: false,
    relaxedSecurity: false,
    forcedCaptivePortalDebug: false,
    captivePortalNoTimeout: false
};

// Global variables to store the CO2 Gadget features supported by the device (selected at compile time)
var features = {
    SUPPORT_BLE: false,
    SUPPORT_BUZZER: false,
    SUPPORT_ESPNOW: false,
    SUPPORT_MDNS: false,
    SUPPORT_MQTT: false,
    SUPPORT_MQTT_DISCOVERY: false,
    SUPPORT_OTA: false
};

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

function loadFeaturesFromServer() {
    fetch('/getFeaturesAsJson')
        .then(response => response.json())
        .then(data => {
            console.log('Fetching loadFeaturesFromServer successful!');
            // Update DOM with status data
            if (data.BLE !== undefined) {
                features.SUPPORT_BLE = data.BLE;
            } else {
                features.SUPPORT_BLE = false;
            }

            if (data.Buzzer !== undefined) {
                features.SUPPORT_BUZZER = data.Buzzer;
            }
            else {
                features.SUPPORT_BUZZER = false;
            }

            if (data.EspNow !== undefined) {
                features.SUPPORT_ESPNOW = data.EspNow;
            }
            else {
                features.SUPPORT_ESPNOW = false;
            }

            if (data.MDNS !== undefined) {
                features.SUPPORT_MDNS = data.MDNS;
            }
            else {
                features.SUPPORT_MDNS = false;
            }

            if (data.MQTT !== undefined) {
                features.SUPPORT_MQTT = data.MQTT;
            }
            else {
                features.SUPPORT_MQTT = false;
            }

            if (data.MQTTDiscovery !== undefined) {
                features.SUPPORT_MQTT_DISCOVERY = data.MQTTDiscovery;
            }
            else {
                features.SUPPORT_MQTT_DISCOVERY = false;
            }

            if (data.OTA !== undefined) {
                features.SUPPORT_OTA = data.OTA;
            }
            else {
                features.SUPPORT_OTA = false;
            }
        }
        )
        .catch(error => console.error('Error fetching features:', error));
}