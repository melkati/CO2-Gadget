// Function to fetch version information from the server and display it in the status line
function displayVersion() {
    fetchVersion()
        .then(versionInfo => {
            let txt = `CO2 Gadget: v${versionInfo.firmVerMajor}.${versionInfo.firmVerMinor}.${versionInfo.firmRevision}`;
            if (versionInfo.firmBranch) txt += `-${versionInfo.firmBranch}`;
            txt += ` (Flavour: ${versionInfo.firmFlavour})`;
            if (versionInfo.firmBuildDate) txt += ` \u2014 Built: ${versionInfo.firmBuildDate}`;
            if (versionInfo.firmBuildTime) txt += ` at ${versionInfo.firmBuildTime}`;
            document.getElementById("co2GadgetVersion").innerText = txt;
        })
        .catch(error => {
            console.error('Error:', error);
        });
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

let uptimeIntervalId = null;

// Function to Fetch status data from /getCaptivePortalStatusAsJson endpoint and populate the form
function loadCaptivePortalStatusFromServer() {
    fetchWithTimeout('/getCaptivePortalStatusAsJson', {}, 6000)
        .then(response => {
            if (!response.ok) throw new Error('Response not OK: ' + response.status);
            return response.json();
        })
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
    fetchWithTimeout('/status', {}, 8000)
        .then(response => {
            if (!response.ok) throw new Error('Response not OK: ' + response.status);
            return response.json();
        })
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
                        updateUptime(element, data[key], data.lowPowerMode, data.waitToDeep);
                    } else if (key === 'lowPowerMode') {
                        switch (data[key]) {
                            case 0:
                                element.textContent = "Disabled";
                                break;
                            case 1:
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

// function updateUptime(element, initialUptime) {
//     const startTime = Date.now();
//     const initialUptimeMs = initialUptime;

//     function formatUptime(uptimeMs) {
//         const seconds = Math.floor(uptimeMs / 1000);
//         const minutes = Math.floor(seconds / 60);
//         const hours = Math.floor(minutes / 60);
//         const days = Math.floor(hours / 24);

//         const formattedHours = hours - (days * 24);
//         const formattedMinutes = minutes - (days * 24 * 60) - (hours * 60);
//         const formattedSeconds = seconds - (days * 24 * 60 * 60) - (hours * 60 * 60) - (minutes * 60);

//         return `${days}d ${formattedHours}h ${formattedMinutes}m ${formattedSeconds}s`;
//     }

//     function update() {
//         const currentTime = Date.now();
//         const elapsedTime = currentTime - startTime;
//         const currentUptime = initialUptimeMs + elapsedTime;

//         element.textContent = formatUptime(currentUptime);
//     }

//     update(); // Initial update
//     setInterval(update, 1000); // Update every second
// }


function updateUptime(element, initialUptime, lowPowerMode, waitToDeep) {
    const startTime = Date.now();
    const initialUptimeMs = initialUptime;

    function formatUptime(uptimeMs) {
        const seconds = Math.floor(uptimeMs / 1000);
        const minutes = Math.floor(seconds / 60);
        const hours = Math.floor(minutes / 60);
        const days = Math.floor(hours / 24);

        const formattedHours = hours % 24; // Correct calculation for hours
        const formattedMinutes = minutes % 60; // Correct calculation for minutes
        const formattedSeconds = seconds % 60; // Correct calculation for seconds

        return `${days}d ${formattedHours}h ${formattedMinutes}m ${formattedSeconds}s`;
    }

    function update() {
        const currentTime = Date.now();
        const elapsedTime = currentTime - startTime;
        const currentUptime = initialUptimeMs + elapsedTime;
        const currentUptimeSecs = Math.floor(currentUptime / 1000);
        const isLowPowerEnabled = (lowPowerMode === 1 || lowPowerMode === 2);

        if (isLowPowerEnabled && Number.isFinite(waitToDeep)) {
            const remainingSecs = Math.max(0, Math.floor(waitToDeep - currentUptimeSecs));
            element.innerHTML = `${formatUptime(currentUptime)}<br><span style="font-size:.9em;opacity:.85">Deep sleep in: ${remainingSecs}s</span>`;
        } else {
            element.textContent = formatUptime(currentUptime);
        }
    }

    update(); // Initial update
    if (uptimeIntervalId) {
        clearInterval(uptimeIntervalId);
    }
    uptimeIntervalId = setInterval(update, 1000); // Update every second
}

function fillFeaturesFromServer() {
    function setFeature(badgeId, valId, val) {
        const el = document.getElementById(valId);
        const badge = document.getElementById(badgeId);
        if (el) el.textContent = val ? '\u2713' : '\u2717';
        if (badge) {
            badge.classList.toggle('feature-on', !!val);
            badge.classList.toggle('feature-off', !val);
        }
    }
    setFeature('featureBLEItem',           'featureBLE',           features.SUPPORT_BLE);
    setFeature('featureBTHomeBLEItem',     'featureBTHomeBLE',     features.SUPPORT_BTHOME_BLE);
    setFeature('featureBuzzerItem',        'featureBuzzer',        features.SUPPORT_BUZZER);
    setFeature('featureESPNowItem',        'featureESPNow',        features.SUPPORT_ESPNOW);
    setFeature('featureMDNSItem',          'featureMDNS',          features.SUPPORT_MDNS);
    setFeature('featureMQTTItem',          'featureMQTT',          features.SUPPORT_MQTT);
    setFeature('featureMQTTDiscoveryItem', 'featureMQTTDiscovery', features.SUPPORT_MQTT_DISCOVERY);
    setFeature('featureOTAItem',           'featureOTA',           features.SUPPORT_OTA);
    if (features.SUPPORT_LOW_POWER) {
        document.getElementById('featureLowPowerItem').classList.remove('hidden');
        document.getElementById('lowPowerOptionsFieldset').classList.remove('hidden');
        setFeature('featureLowPowerItem', 'featureLowPower', features.SUPPORT_LOW_POWER);
    } else {
        document.getElementById('featureLowPowerItem').classList.add('hidden');
        document.getElementById('lowPowerOptionsFieldset').classList.add('hidden');
    }
}

function updateCO2Value(co2) {
    document.getElementById("co2").innerHTML = co2;
}

function updateTemperatureValue(temperature) {
    document.getElementById("temperature").innerHTML = temperature;
}

function updateHumidityValue(humidity) {
    document.getElementById("humidity").innerHTML = humidity;
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

function fetchAndUpdateCO2Value() {
    readCO2Data()
        .then(co2 => {
            updateCO2Value(co2);
        })
        .catch(error => {
            console.error('Error updating CO2 value:', error);
        });
}

function fetchAndUpdateTemperatureValue() {
    readTemperatureData()
        .then(temperature => {
            updateTemperatureValue(temperature);
        })
        .catch(error => {
            console.error('Error updating temperature value:', error);
        });
}

function fetchAndUpdateHumidityValue() {
    readHumidityData()
        .then(humidity => {
            updateHumidityValue(humidity);
        })
        .catch(error => {
            console.error('Error updating humidity value:', error);
        });
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
    // Clock update — purely local, no fetch
    updateStatusClock();
}, 1000);

setInterval(function () {
    // Sensor and heap data — polled every 5 s to avoid overloading the ESP32
    fetchAndUpdateBatteryVoltage();
    fetchAndUpdateFreeHeap();
    fetchAndUpdateMinFreeHeap();
    fetchAndUpdateCO2Value();
    fetchAndUpdateTemperatureValue();
    fetchAndUpdateHumidityValue();
}, 5000);

setInterval(function () {
    // Captive-portal / connectivity status — sufficient at 5 s
    loadCaptivePortalStatusFromServer();
}, 5000);

/** Updates the real-time clock display on the status page using the stored timezone. */
function updateStatusClock() {
    const el = document.getElementById('currentTime');
    const elOff = document.getElementById('currentTimeOffset');
    if (!el) return;
    const offset = (typeof getTzOffsetHours === 'function') ? getTzOffsetHours() : -new Date().getTimezoneOffset() / 60;
    const pad = n => String(n).padStart(2, '0');
    const d = new Date(Date.now() + offset * 3600000);
    el.textContent = pad(d.getUTCHours()) + ':' + pad(d.getUTCMinutes()) + ':' + pad(d.getUTCSeconds());
    if (elOff) {
        const sign = offset >= 0 ? '+' : '−';
        const h = Math.floor(Math.abs(offset));
        const m = Math.round((Math.abs(offset) - h) * 60);
        elOff.textContent = 'UTC' + sign + h + (m ? ':' + pad(m) : '');
    }
}

window.onload = function () {
    // Initial fetch on load so data shows immediately without waiting for first interval
    fetchAndUpdateBatteryVoltage();
    fetchAndUpdateCO2Value();
    fetchAndUpdateTemperatureValue();
    fetchAndUpdateHumidityValue();
};

document.addEventListener("DOMContentLoaded", function () {
    // Get the current URL
    var currentURL = window.location.href;

    // Check if the current URL contains "status.html"
    if (currentURL.includes("status.html")) {
        highlightCurrentPage(); // Highlight the current page in the navigation bar
        // Set initial values when the page loads
        loadStatusFromServer();
        displayVersion();
        loadCaptivePortalStatusFromServer();
        getFeaturesAsJson().then(fillFeaturesFromServer);
        updateStatusClock(); // show clock immediately, don't wait 1s
    }
});
