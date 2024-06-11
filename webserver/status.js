// Function to fetch version information from the server
function fetchVersion() {
    fetch('/getVersion')
        .then(response => response.json())
        .then(versionInfo => {
            // Update CO2 Gadget version in the status line
            document.getElementById("co2GadgetVersion").innerText = "CO2 Gadget: v" + versionInfo.firmVerMajor + "." + versionInfo.firmVerMinor + "." + versionInfo.firmRevision + "-" + versionInfo.firmBranch + " (Flavour: " + versionInfo.firmFlavour + ")";
        })
        .catch(error => console.error('Error fetching version information:', error));
}

/**
 * Receive a ESP32 numeric code for WiFi Status.
 * Returns a string with the WiFi Status text for ESP32 numeric codes.
 */
function getWiFiStatusText(wifiStatus) {
    switch (wifiStatus) {
        case 0:
            return "WL_IDLE_STATUS (WiFi is in process of changing between statuses)";
        case 1:
            return "WL_NO_SSID_AVAIL (No SSID is available)";
        case 2:
            return "WL_SCAN_COMPLETED (Scan has completed)";
        case 3:
            return "WL_CONNECTED (Connected to a WiFi network)";
        case 4:
            return "WL_CONNECT_FAILED (Failed to connect to a WiFi network)";
        case 5:
            return "WL_CONNECTION_LOST (Connection lost)";
        case 6:
            return "WL_DISCONNECTED (Disconnected from a WiFi network)";
        default:
            return "Unknown WiFi Status";
    }
}

// Function to Fetch status data from /getCaptivePortalStatusAsJson endpoint and populate the form
function loadCaptivePortalStatusFromServer() {
    fetch('/getCaptivePortalStatusAsJson')
        .then(response => response.json())
        .then(data => {
            console.log('Fetching data successful!');
            // Update DOM with status data
            if (data.captivePortalActive !== undefined) {
                document.getElementById('captivePortalActive').textContent = data.captivePortalActive;
            } else {
                document.getElementById('captivePortalActiveItem').style.display = 'none';
            }

            if (data.timeCaptivePortalStarted !== undefined) {
                document.getElementById('timeCaptivePortalStarted').textContent = data.timeCaptivePortalStarted;
            } else {
                document.getElementById('timeCaptivePortalStartedItem').style.display = 'none';
            }

            if (data.timeToWaitForCaptivePortal !== undefined) {
                document.getElementById('timeToWaitForCaptivePortal').textContent = data.timeToWaitForCaptivePortal;
            } else {
                document.getElementById('timeToWaitForCaptivePortalItem').style.display = 'none';
            }

            if (data.captivePortalTimeLeft !== undefined && data.captivePortalActive) {
                document.getElementById('captivePortalTimeLeft').textContent = data.captivePortalTimeLeft;
            } else {
                document.getElementById('captivePortalTimeLeftItem').style.display = 'none';
            }

            if (data.captivePortalDebug !== undefined) {
                document.getElementById('captivePortalDebug').textContent = data.captivePortalDebug;
            } else {
                document.getElementById('captivePortalDebugItem').style.display = 'none';
            }

            if (data.relaxedSecurity !== undefined) {
                document.getElementById('relaxedSecurity').textContent = data.relaxedSecurity;
            } else {
                document.getElementById('relaxedSecurityItem').style.display = 'none';
            }

        })
        .catch(error => console.error('Error fetching status:', error));
}

// Function to Fetch status data from /status endpoint and populate the form
function loadStatusFromServer() {
    fetch('/status')
        .then(response => response.json())
        .then(data => {
            console.log('Fetching data successful!');
            // Update DOM with status data
            const elements = {
                mainDeviceSelected: 'mainDeviceSelected',
                CO2: 'co2',
                Temperature: 'temperature',
                Humidity: 'humidity',
                WiFiStatus: 'wifiStatus',
                SSID: 'ssid',
                WiFiPassword: 'wifiPass',
                IP: 'ip',
                RSSI: 'rssi',
                MACAddress: 'macAddress',
                hostName: 'hostName',
                useStaticIP: 'useStaticIP',
                staticIP: 'staticIP',
                gateway: 'gateway',
                subnet: 'subnet',
                dns1: 'dns1',
                dns2: 'dns2',
                rootTopic: 'rootTopic',
                discoveryTopic: 'discoveryTopic',
                mqttClientId: 'mqttClientId',
                mqttBroker: 'mqttBroker',
                mqttUser: 'mqttUser',
                mqttPassword: 'mqttPass',
                peerESPNowAddress: 'peerESPNowAddress',
                activeWiFi: 'activeWiFi',
                activeMQTT: 'activeMQTT',
                activeBLE: 'activeBLE',
                activeOTA: 'activeOTA',
                troubledWiFi: 'troubledWiFi',
                troubledMQTT: 'troubledMQTT',
                troubledESPNow: 'troubledESPNow',
                measurementInterval: 'measurementInterval',
                sampleInterval: 'sampleInterval',
                calibrationValue: 'calibrationValue',
                pendingCalibration: 'pendingCalibration',
                freeHeap: 'freeHeap',
                minFreeHeap: 'minFreeHeap',
                uptime: 'uptime',
                lowPowerMode: 'lowPowerMode',
                waitToDeep: 'waitToDeep',
                timeSleeping: 'timeSleeping',
                cyclsWifiConn: 'cyclsWifiConn',
                cycRedrawDis: 'cycRedrawDis',
                actBLEOnWake: 'actBLEOnWake',
                actWifiOnWake: 'actWifiOnWake',
                actMQTTOnWake: 'actMQTTOnWake',
                actESPnowWake: 'actESPnowWake',
                displayOnWake: 'displayOnWake'
            };

            Object.keys(elements).forEach(key => {
                const elementId = elements[key];
                const element = document.getElementById(elementId);
                const itemElement = document.getElementById(`${elementId}Item`);

                if (!element) {
                    console.warn(`Element with id ${elementId} not found.`);
                    return;
                }

                if (data[key] !== undefined) {
                    if (key === 'WiFiStatus') {
                        element.textContent = getWiFiStatusText(data[key]) + " (" + data[key] + ")";
                    } else if (key === 'uptime') {
                        updateUptime(element, data[key]);
                    } else if (key === 'lowPowerMode') {
                        switch (data[key]) {
                            case 0:
                                element.textContent = "Disabled";
                                break;
                            case 2:
                                element.textContent = "Enabled";
                                break;
                            default:
                                element.textContent = "Unknown";
                        }
                    } else if (typeof data[key] === 'boolean') {
                        element.textContent = data[key] ? 'Yes' : 'No';
            } else {
                        element.textContent = data[key];
                    }
            } else {
                    if (itemElement) itemElement.style.display = 'none';
            }
            });
        })
        .catch(error => console.error('Error fetching status:', error));
}

function updateUptime(element, initialUptime) {
    const startTime = Date.now();
    const initialUptimeMs = initialUptime;

    function formatUptime(uptimeMs) {
        const seconds = Math.floor(uptimeMs / 1000);
        const minutes = Math.floor(seconds / 60);
        const hours = Math.floor(minutes / 60);
        const days = Math.floor(hours / 24);

        const formattedHours = hours - (days * 24);
        const formattedMinutes = minutes - (days * 24 * 60) - (hours * 60);
        const formattedSeconds = seconds - (days * 24 * 60 * 60) - (hours * 60 * 60) - (minutes * 60);

        return `${days}d ${formattedHours}h ${formattedMinutes}m ${formattedSeconds}s`;
    }

    function update() {
        const currentTime = Date.now();
        const elapsedTime = currentTime - startTime;
        const currentUptime = initialUptimeMs + elapsedTime;

        element.textContent = formatUptime(currentUptime);
    }

    update(); // Initial update
    setInterval(update, 1000); // Update every second
}

function fillFeaturesFromServer() {
    // Update DOM with feature data
    document.getElementById('featureBLE').textContent = features.SUPPORT_BLE;
    document.getElementById('featureBuzzer').textContent = features.SUPPORT_BUZZER
    document.getElementById('featureESPNow').textContent = features.SUPPORT_ESPNOW;
    document.getElementById('featureMDNS').textContent = features.SUPPORT_MDNS;
    document.getElementById('featureMQTT').textContent = features.SUPPORT_MQTT;
    document.getElementById('featureMQTTDiscovery').textContent = features.SUPPORT_MQTT_DISCOVERY;
    document.getElementById('featureOTA').textContent = features.SUPPORT_OTA;
}

function updateBatteryVoltage(voltage) {
    document.getElementById("batVoltage").innerHTML = voltage;
}

function updateFreeHeap(freeHeap) {
    document.getElementById("freeHeap").innerHTML = freeHeap;
}

function updateMinFreeHeap(minFreeHeap) {
    document.getElementById("minFreeHeap").innerHTML = minFreeHeap;
}

function fetchAndUpdateBatteryVoltage() {
    readBatteryVoltage()
        .then(voltage => {
            updateBatteryVoltage(voltage);
        })
        .catch(error => {
            console.error('Error updating battery voltage:', error);
        });
}

function fetchAndUpdateFreeHeap() {
    readFreeHeap()
        .then(freeHeap => {
            updateFreeHeap(freeHeap);
        })
        .catch(error => {
            console.error('Error updating free heap:', error);
        });
}

function fetchAndUpdateMinFreeHeap() {
    readMinFreeHeap()
        .then(minFreeHeap => {
            updateMinFreeHeap(minFreeHeap);
        })
        .catch(error => {
            console.error('Error updating min free heap:', error);
        });
}

setInterval(function () {
    // Call a function repetatively with 1 Second interval
    fetchAndUpdateBatteryVoltage();
    fetchAndUpdateFreeHeap();
    fetchAndUpdateMinFreeHeap();
    loadCaptivePortalStatusFromServer();
}, 1000); // 1000mS  update rate

window.onload = function () {
    fetchAndUpdateBatteryVoltage();
};

document.addEventListener("DOMContentLoaded", function () {
    // Get the current URL
    var currentURL = window.location.href;

    // Check if the current URL contains "status.html"
    if (currentURL.includes("status.html")) {
        highlightCurrentPage(); // Highlight the current page in the navigation bar
        // Set initial values when the page loads
        loadStatusFromServer();
        fetchVersion;
        loadCaptivePortalStatusFromServer();
        loadFeaturesFromServer();
        setTimeout(function () {
            fillFeaturesFromServer();
        }, 500);        
    }
});
