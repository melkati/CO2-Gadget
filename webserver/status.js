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

/**
 * Updates the DOM with feature data for CO2 Gadget.
 */
function showCO2GadgetFeatures() {
    document.getElementById('featureBLE').textContent = features.SUPPORT_BLE;
    document.getElementById('featureBuzzer').textContent = features.SUPPORT_BUZZER;
    document.getElementById('featureESPNow').textContent = features.SUPPORT_ESPNOW;
    document.getElementById('featureMDNS').textContent = features.SUPPORT_MDNS;
    document.getElementById('featureMQTT').textContent = features.SUPPORT_MQTT;
    document.getElementById('featureMQTTDiscovery').textContent = features.SUPPORT_MQTT_DISCOVERY;
    document.getElementById('featureOTA').textContent = features.SUPPORT_OTA;
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
            if (data.mainDeviceSelected !== undefined) {
                document.getElementById('mainDeviceSelected').textContent = data.mainDeviceSelected;
            } else {
                document.getElementById('mainDeviceSelected').style.display = 'none';
            }
            if (data.CO2 !== undefined) {
                document.getElementById('co2').textContent = data.CO2;
            } else {
                document.getElementById('co2Item').style.display = 'none';
            }

            if (data.Temperature !== undefined) {
                document.getElementById('temperature').textContent = data.Temperature;
            } else {
                document.getElementById('temperatureItem').style.display = 'none';
            }

            if (data.Humidity !== undefined) {
                document.getElementById('humidity').textContent = data.Humidity;
            } else {
                document.getElementById('humidityItem').style.display = 'none';
            }

            if (data.WiFiStatus !== undefined) {
                document.getElementById('wifiStatus').textContent = getWiFiStatusText(data.WiFiStatus) + " (" + data.WiFiStatus + ")";
            } else {
                document.getElementById('wifiStatusItem').style.display = 'none';
            }

            if (data.SSID !== undefined) {
                document.getElementById('ssid').textContent = data.SSID;
            } else {
                document.getElementById('ssidItem').style.display = 'none';
            }

            if (data.WiFiPassword !== undefined) {
                document.getElementById('wifiPass').textContent = data.WiFiPassword;
            } else {
                document.getElementById('wifiPassItem').style.display = 'none';
            }

            if (data.IP !== undefined) {
                document.getElementById('ip').textContent = data.IP;
            } else {
                document.getElementById('ipItem').style.display = 'none';
            }

            if (data.RSSI !== undefined) {
                document.getElementById('rssi').textContent = data.RSSI;
            } else {
                document.getElementById('rssiItem').style.display = 'none';
            }

            if (data.MACAddress !== undefined) {
                document.getElementById('macAddress').textContent = data.MACAddress;
            } else {
                document.getElementById('macAddressItem').style.display = 'none';
            }

            if (data.hostName !== undefined) {
                document.getElementById('hostName').textContent = data.hostName;
            } else {
                document.getElementById('hostNameItem').style.display = 'none';
            }

            if (data.useStaticIP !== undefined) {
                document.getElementById('useStaticIP').textContent = data.useStaticIP;
            } else {
                document.getElementById('useStaticIPItem').style.display = 'none';
            }

            if (data.staticIP !== undefined) {
                document.getElementById('staticIP').textContent = data.staticIP;
            } else {
                document.getElementById('staticIPItem').style.display = 'none';
            }

            if (data.gateway !== undefined) {
                document.getElementById('gateway').textContent = data.gateway;
            } else {
                document.getElementById('gatewayItem').style.display = 'none';
            }

            if (data.subnet !== undefined) {
                document.getElementById('subnet').textContent = data.subnet;
            } else {
                document.getElementById('subnetItem').style.display = 'none';
            }

            if (data.dns1 !== undefined) {
                document.getElementById('dns1').textContent = data.dns1;
            } else {
                document.getElementById('dns1Item').style.display = 'none';
            }

            if (data.dns2 !== undefined) {
                document.getElementById('dns2').textContent = data.dns2;
            } else {
                document.getElementById('dns2Item').style.display = 'none';
            }

            if (data.rootTopic !== undefined) {
                document.getElementById('rootTopic').textContent = data.rootTopic;
            } else {
                document.getElementById('rootTopicItem').style.display = 'none';
            }

            if (data.discoveryTopic !== undefined) {
                document.getElementById('discoveryTopic').textContent = data.discoveryTopic;
            } else {
                document.getElementById('discoveryTopicItem').style.display = 'none';
            }

            if (data.mqttClientId !== undefined) {
                document.getElementById('mqttClientId').textContent = data.mqttClientId;
            } else {
                document.getElementById('mqttClientIdItem').style.display = 'none';
            }

            if (data.mqttBroker !== undefined) {
                document.getElementById('mqttBroker').textContent = data.mqttBroker;
            } else {
                document.getElementById('mqttBrokerItem').style.display = 'none';
            }

            if (data.mqttUser !== undefined) {
                document.getElementById('mqttUser').textContent = data.mqttUser;
            } else {
                document.getElementById('mqttUserItem').style.display = 'none';
            }

            if (data.mqttPassword !== undefined) {
                document.getElementById('mqttPass').textContent = data.mqttPassword;
            } else {
                document.getElementById('mqttPassItem').style.display = 'none';
            }

            if (data.peerESPNowAddress !== undefined) {
                document.getElementById('peerESPNowAddress').textContent = data.peerESPNowAddress;
            } else {
                document.getElementById('peerESPNowAddressItem').style.display = 'none';
            }

            if (data.activeWiFi !== undefined) {
                document.getElementById('activeWiFi').textContent = data.activeWiFi;
            } else {
                document.getElementById('activeWiFiItem').style.display = 'none';
            }

            if (data.activeMQTT !== undefined) {
                document.getElementById('activeMQTT').textContent = data.activeMQTT;
            } else {
                document.getElementById('activeMQTTItem').style.display = 'none';
            }

            if (data.activeBLE !== undefined) {
                document.getElementById('activeBLE').textContent = data.activeBLE;
            } else {
                document.getElementById('activeBLEItem').style.display = 'none';
            }

            if (data.activeOTA !== undefined) {
                document.getElementById('activeOTA').textContent = data.activeOTA;
            } else {
                document.getElementById('activeOTAItem').style.display = 'none';
            }

            if (data.troubledWiFi !== undefined) {
                document.getElementById('troubledWiFi').textContent = data.troubledWiFi;
            } else {
                document.getElementById('troubledWiFiItem').style.display = 'none';
            }

            if (data.troubledMQTT !== undefined) {
                document.getElementById('troubledMQTT').textContent = data.troubledMQTT;
            } else {
                document.getElementById('troubledMQTTItem').style.display = 'none';
            }

            if (data.troubledESPNow !== undefined) {
                document.getElementById('troubledESPNow').textContent = data.troubledESPNow;
            } else {
                document.getElementById('troubledESPNowItem').style.display = 'none';
            }

            if (data.measurementInterval !== undefined) {
                document.getElementById('measurementInterval').textContent = data.measurementInterval;
            } else {
                document.getElementById('measurementIntervalItem').style.display = 'none';
            }

            if (data.sampleInterval !== undefined) {
                document.getElementById('sampleInterval').textContent = data.sampleInterval;
            } else {
                document.getElementById('sampleIntervalItem').style.display = 'none';
            }

            if (data.calibrationValue !== undefined) {
                document.getElementById('calibrationValue').textContent = data.calibrationValue;
            } else {
                document.getElementById('calibrationValueItem').style.display = 'none';
            }

            if (data.pendingCalibration !== undefined) {
                document.getElementById('pendingCalibration').textContent = data.pendingCalibration;
            } else {
                document.getElementById('pendingCalibrationItem').style.display = 'none';
            }

            if (data.freeHeap !== undefined) {
                document.getElementById('freeHeap').textContent = data.freeHeap;
            } else {
                document.getElementById('freeHeapItem').style.display = 'none';
            }

            if (data.minFreeHeap !== undefined) {
                document.getElementById('minFreeHeap').textContent = data.minFreeHeap;
            } else {
                document.getElementById('minFreeHeapItem').style.display = 'none';
            }

            if (data.uptime !== undefined) {
                // Convert data.uptime (in melliseconds) to human readable format (HH:MM:SS)
                var seconds = Math.floor(data.uptime / 1000);
                var minutes = Math.floor(seconds / 60);
                var hours = Math.floor(minutes / 60);
                var days = Math.floor(hours / 24);

                hours = hours - (days * 24);
                minutes = minutes - (days * 24 * 60) - (hours * 60);
                seconds = seconds - (days * 24 * 60 * 60) - (hours * 60 * 60) - (minutes * 60);


                document.getElementById('uptime').textContent = data.uptime;
            } else {
                document.getElementById('uptimeItem').style.display = 'none';
            }

        })
        .catch(error => console.error('Error fetching status:', error));
}

function loadFeaturesFromServer() {
    fetch('/getFeaturesAsJson')
        .then(response => response.json())
        .then(data => {
            console.log('Fetching loadFeaturesFromServer successful!');
            // Update DOM with status data
            if (data.BLE !== undefined) {
                features.SUPPORT_BLE = data.BLE;
                document.getElementById('featureBLE').textContent = data.BLE;
            } else {
                document.getElementById('featureBLEItem').style.display = 'none';
            }
            if (data.Buzzer !== undefined) {
                features.SUPPORT_BUZZER = data.Buzzer;
                document.getElementById('featureBuzzer').textContent = data.Buzzer;
            } else {
                document.getElementById('featureBuzzerItem').style.display = 'none';
            }
            if (data.ESPNow !== undefined) {
                features.SUPPORT_ESPNOW = data.ESPNow;
                document.getElementById('featureESPNow').textContent = data.ESPNow;
            } else {
                document.getElementById('featureESPNowItem').style.display = 'none';
            }
            if (data.MDNS !== undefined) {
                features.SUPPORT_MDNS = data.MDNS;
                document.getElementById('featureMDNS').textContent = data.MDNS;
            } else {
                document.getElementById('featureMDNSItem').style.display = 'none';
            }
            if (data.MQTT !== undefined) {
                features.SUPPORT_MQTT = data.MQTT;
                document.getElementById('featureMQTT').textContent = data.MQTT;
            } else {
                document.getElementById('featureMQTTItem').style.display = 'none';
            }
            if (data.MQTTDiscovery !== undefined) {
                features.SUPPORT_MQTT_DISCOVERY = data.MQTTDiscovery;
                document.getElementById('featureMQTTDiscovery').textContent = data.MQTTDiscovery;
            } else {
                document.getElementById('featureMQTTDiscoveryItem').style.display = 'none';
            }
            if (data.OTA !== undefined) {
                features.SUPPORT_OTA = data.OTA;
                document.getElementById('featureOTA').textContent = data.OTA;
            } else {
                document.getElementById('featureOTAItem').style.display = 'none';
            }
            if (data.mDNS !== undefined) {
                document.getElementById('mDNSName').textContent = data.mDNS;
            } else {
                document.getElementById('mDNSNameItem').style.display = 'none';
            }
        }
        )
        .catch(error => console.error('Error fetching features:', error));
}

function getBatteryVoltage() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        console.log(this.responseText);
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("batVoltage").innerHTML =
                this.responseText;
        }
    };
    xhttp.open("GET", "readBatteryVoltage", true);
    xhttp.send();
}

function getFreeHeap() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        console.log(this.responseText);
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("freeHeap").innerHTML =
                this.responseText;
        }
    };
    xhttp.open("GET", "getFreeHeap", true);
    xhttp.send();
}

function getMinFreeHeap() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        console.log(this.responseText);
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("minFreeHeap").innerHTML =
                this.responseText;
        }
    };
    xhttp.open("GET", "getMinFreeHeap", true);
    xhttp.send();
}

setInterval(function () {
    // Call a function repetatively with 1 Second interval
    getBatteryVoltage();
    getFreeHeap();
    getMinFreeHeap();
    loadCaptivePortalStatusFromServer();
}, 1000); // 1000mS  update rate

window.onload = function () {
    getBatteryVoltage();
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
    }
});
