// https://www.zickty.com/filetogzip

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

// Function to load preferences from the server and populate the form
function loadPreferencesFromServer() {
    let getPreferencesDebug = true; // Set to false to disable debug output

    fetch('/getActualSettingsAsJson')
        .then(response => response.json())
        .then(preferences => {
            if (getPreferencesDebug) {
                console.log('Received preferences:', preferences);
            }

            // Populate the form with the received preferences


            if (getPreferencesDebug) console.log('Setting activeWIFI to:', preferences.activeWIFI);
            document.getElementById("activeWIFI").checked = preferences.activeWIFI;

            if (getPreferencesDebug) console.log('Setting wifiSSID to:', preferences.wifiSSID);
            document.getElementById("wifiSSID").value = preferences.wifiSSID;

            // if (getPreferencesDebug) console.log('Setting wifiPass to:', preferences.wifiPass);
            // document.getElementById("wifiPass").value = preferences.wifiPass;

            if (getPreferencesDebug) console.log('Setting hostName to:', preferences.hostName);
            document.getElementById("hostName").value = preferences.hostName;

            if (getPreferencesDebug) console.log('Setting selCO2Sensor to:', preferences.selCO2Sensor);
            document.getElementById("selCO2Sensor").value = preferences.selCO2Sensor;

            if (getPreferencesDebug) console.log('Setting autoSelfCalibration to:', preferences.autoSelfCal);
            document.getElementById("autoSelfCalibration").checked = preferences.autoSelfCal;

            if (getPreferencesDebug) console.log('Setting customCalValue to:', preferences.customCalValue);
            document.getElementById("customCalValue").value = preferences.customCalValue;

            if (getPreferencesDebug) console.log('Setting co2OrangeRange to:', preferences.co2OrangeRange);
            document.getElementById("co2OrangeRange").value = preferences.co2OrangeRange;

            if (getPreferencesDebug) console.log('Setting co2RedRange to:', preferences.co2RedRange);
            document.getElementById("co2RedRange").value = preferences.co2RedRange;

            if (getPreferencesDebug) console.log('Setting tempOffset to:', preferences.tempOffset);
            document.getElementById("tempOffset").value = preferences.tempOffset;

            if (getPreferencesDebug) console.log('Setting showFahrenheit to:', preferences.showFahrenheit);
            document.getElementById("showFahrenheit").checked = preferences.showFahrenheit;

            if (getPreferencesDebug) console.log('Setting altitudeMeters to:', preferences.altitudeMeters);
            document.getElementById("altitudeMeters").value = preferences.altitudeMeters;

            if (getPreferencesDebug) console.log('Setting measurementInterval to:', preferences.measurementInterval);
            document.getElementById("measurementInterval").value = preferences.measurementInterval;

            if (getPreferencesDebug) console.log('Setting outModeRelay to:', preferences.outModeRelay);
            document.getElementById("outModeRelay").checked = preferences.outModeRelay;

            if (getPreferencesDebug) console.log('Setting channelESPNow to:', preferences.channelESPNow);
            document.getElementById("channelESPNow").value = preferences.channelESPNow;

            if (getPreferencesDebug) console.log('Setting boardIdESPNow to:', preferences.boardIdESPNow);
            document.getElementById("boardIdESPNow").value = preferences.boardIdESPNow;

            if (getPreferencesDebug) console.log('Setting peerESPNowAddress to:', preferences.peerESPNowAddress);
            document.getElementById("peerESPNowAddress").value = preferences.peerESPNowAddress;

            if (getPreferencesDebug) console.log('Setting neopixBright to:', preferences.neopixBright);
            document.getElementById("neopixBright").value = preferences.neopixBright;

            if (getPreferencesDebug) console.log('Setting selNeopxType to:', preferences.selNeopxType);
            document.getElementById("selNeopxType").value = preferences.selNeopxType;

            if (getPreferencesDebug) console.log('Setting activeBLE to:', preferences.activeBLE);
            document.getElementById("activeBLE").checked = preferences.activeBLE;

            if (getPreferencesDebug) console.log('Setting activeMQTT to:', preferences.activeMQTT);
            document.getElementById("activeMQTT").checked = preferences.activeMQTT;

            if (getPreferencesDebug) console.log('Setting activeESPNOW to:', preferences.activeESPNOW);
            document.getElementById("activeESPNOW").checked = preferences.activeESPNOW;

            if (getPreferencesDebug) console.log('Setting mqttClientId to:', preferences.mqttClientId);
            document.getElementById("mqttClientId").value = preferences.mqttClientId;

            if (getPreferencesDebug) console.log('Setting rootTopic to:', preferences.rootTopic);
            document.getElementById("rootTopic").value = preferences.rootTopic;

            if (getPreferencesDebug) console.log('Setting mqttBroker to:', preferences.mqttBroker);
            document.getElementById("mqttBroker").value = preferences.mqttBroker;

            if (getPreferencesDebug) console.log('Setting mqttUser to:', preferences.mqttUser);
            document.getElementById("mqttUser").value = preferences.mqttUser;

            // if (getPreferencesDebug) console.log('Setting mqttPass to:', preferences.mqttPass);
            // document.getElementById("mqttPass").value = preferences.mqttPass;

            if (getPreferencesDebug) console.log('Setting batDischgd to:', preferences.batDischgd);
            document.getElementById("batDischgd").value = preferences.batDischgd;

            if (getPreferencesDebug) console.log('Setting batChargd to:', preferences.batChargd);
            document.getElementById("batChargd").value = preferences.batChargd;

            if (getPreferencesDebug) console.log('Setting vRef to:', preferences.vRef);
            document.getElementById("vRef").value = preferences.vRef;

            if (getPreferencesDebug) console.log('Setting tToDispOff to:', preferences.tToDispOff);
            document.getElementById("tToDispOff").value = preferences.tToDispOff;

            if (getPreferencesDebug) console.log('Setting tToPubMQTT to:', preferences.tToPubMQTT);
            document.getElementById("tToPubMQTT").value = preferences.tToPubMQTT;

            if (getPreferencesDebug) console.log('Setting tToPubESPNow to:', preferences.tToPubESPNow);
            document.getElementById("tToPubESPNow").value = preferences.tToPubESPNow;

            if (getPreferencesDebug) console.log('Setting tKeepAlMQTT to:', preferences.tKeepAlMQTT);
            document.getElementById("tKeepAlMQTT").value = preferences.tKeepAlMQTT;

            if (getPreferencesDebug) console.log('Setting tKeepAlESPNow to:', preferences.tKeepAlESPNow);
            document.getElementById("tKeepAlESPNow").value = preferences.tKeepAlESPNow;

            if (getPreferencesDebug) console.log('Setting showTemp to:', preferences.showTemp);
            document.getElementById("showTemp").checked = preferences.showTemp;

            if (getPreferencesDebug) console.log('Setting showHumidity to:', preferences.showHumidity);
            document.getElementById("showHumidity").checked = preferences.showHumidity;

            if (getPreferencesDebug) console.log('Setting showBattery to:', preferences.showBattery);
            document.getElementById("showBattery").checked = preferences.showBattery;

            if (getPreferencesDebug) console.log('Setting showCO2 to:', preferences.showCO2);
            document.getElementById("showCO2").checked = preferences.showCO2;

            // if (getPreferencesDebug) console.log('Setting showPM25 to:', preferences.showPM25);
            // document.getElementById("showPM25").checked = preferences.showPM25;

            if (getPreferencesDebug) console.log('Setting dispOffOnExP to:', preferences.dispOffOnExP);
            document.getElementById("dispOffOnExP").checked = preferences.dispOffOnExP;

            if (getPreferencesDebug) console.log('Setting displayReverse to:', preferences.displayReverse);
            document.getElementById("displayReverse").checked = preferences.displayReverse;

            if (getPreferencesDebug) console.log('Setting DisplayBright to:', preferences.DisplayBright);
            document.getElementById("DisplayBright").value = preferences.DisplayBright;

            if (getPreferencesDebug) console.log('Setting debugSensors to:', preferences.debugSensors);
            document.getElementById("debugSensors").checked = preferences.debugSensors;

            if (getPreferencesDebug) console.log('Setting toneBzrBeep to:', preferences.toneBzrBeep);
            document.getElementById("toneBzrBeep").value = preferences.toneBzrBeep;

            if (getPreferencesDebug) console.log('Setting durBzrBeep to:', preferences.durBzrBeep);
            document.getElementById("durBzrBeep").value = preferences.durBzrBeep;

            if (getPreferencesDebug) console.log('Setting timeBtwnBzr to:', preferences.timeBtwnBzr);
            document.getElementById("timeBtwnBzr").value = preferences.timeBtwnBzr;

            if (getPreferencesDebug) console.log('Setting lowPowerMode to:', preferences.lowPowerMode);
            document.getElementById("lowPowerMode").value = preferences.lowPowerMode;

            if (getPreferencesDebug) console.log('Setting waitToDeep to:', preferences.waitToDeep);
            document.getElementById("waitToDeep").value = preferences.waitToDeep;

            if (getPreferencesDebug) console.log('Setting timeSleeping to:', preferences.timeSleeping);
            document.getElementById("timeSleeping").value = preferences.timeSleeping;

            if (getPreferencesDebug) console.log('Setting cyclsWifiConn to:', preferences.cyclsWifiConn);
            document.getElementById("cyclsWifiConn").value = preferences.cyclsWifiConn;

            if (getPreferencesDebug) console.log('Setting cycRedrawDis to:', preferences.cycRedrawDis);
            document.getElementById("cycRedrawDis").value = preferences.cycRedrawDis;

            if (getPreferencesDebug) console.log('Setting actBLEOnWake to:', preferences.actBLEOnWake);
            document.getElementById("actBLEOnWake").checked = preferences.actBLEOnWake;

            if (getPreferencesDebug) console.log('Setting actWifiOnWake to:', preferences.actWifiOnWake);
            document.getElementById("actWifiOnWake").checked = preferences.actWifiOnWake;

            if (getPreferencesDebug) console.log('Setting actMQTTOnWake to:', preferences.actMQTTOnWake);
            document.getElementById("actMQTTOnWake").checked = preferences.actMQTTOnWake;

            if (getPreferencesDebug) console.log('Setting actESPnowWake to:', preferences.actESPnowWake);
            document.getElementById("actESPnowWake").checked = preferences.actESPnowWake;

            if (getPreferencesDebug) console.log('Setting deepSleepData.displayOnWake to:', preferences.deepSleepData.displayOnWake);
            document.getElementById("deepSleepData.displayOnWake").checked = preferences.deepSleepData.displayOnWake;


        })
        .catch(error => console.error('Error retrieving preferences:', error));
}

function collectPreferencesData() {
    // Collect preferences data from the form
    var preferencesData = {
        customCalValue: document.getElementById("customCalValue").value,
        tempOffset: document.getElementById("tempOffset").value,
        altitudeMeters: document.getElementById("altitudeMeters").value,
        autoSelfCalibration: document.getElementById("autoSelfCalibration").checked,
        co2OrangeRange: document.getElementById("co2OrangeRange").value,
        co2RedRange: document.getElementById("co2RedRange").value,
        DisplayBright: document.getElementById("DisplayBright").value,
        neopixBright: document.getElementById("neopixBright").value,
        selNeopxType: document.getElementById("selNeopxType").value,
        activeBLE: document.getElementById("activeBLE").checked,
        activeWIFI: document.getElementById("activeWIFI").checked,
        activeMQTT: document.getElementById("activeMQTT").checked,
        activeESPNOW: document.getElementById("activeESPNOW").checked,
        rootTopic: document.getElementById("rootTopic").value,
        batDischgd: document.getElementById("batDischgd").value,
        batChargd: document.getElementById("batChargd").value,
        vRef: document.getElementById("vRef").value,
        tToDispOff: document.getElementById("tToDispOff").value,
        tToPubMQTT: document.getElementById("tToPubMQTT").value,
        tToPubESPNow: document.getElementById("tToPubESPNow").value,
        tKeepAlMQTT: document.getElementById("tKeepAlMQTT").value,
        tKeepAlESPNow: document.getElementById("tKeepAlESPNow").value,
        dispOffOnExP: document.getElementById("dispOffOnExP").checked,
        wifiSSID: document.getElementById("wifiSSID").value,
        // wifiPass: document.getElementById("wifiPass").value,
        hostName: document.getElementById("hostName").value,
        selCO2Sensor: document.getElementById("selCO2Sensor").value,
        debugSensors: document.getElementById("debugSensors").checked,
        displayReverse: document.getElementById("displayReverse").checked,
        showFahrenheit: document.getElementById("showFahrenheit").checked,
        measurementInterval: document.getElementById("measurementInterval").value,
        outModeRelay: document.getElementById("outModeRelay").checked,
        channelESPNow: document.getElementById("channelESPNow").value,
        boardIdESPNow: document.getElementById("boardIdESPNow").value,
        peerESPNowAddress: document.getElementById("peerESPNowAddress").value,
        showTemp: document.getElementById("showTemp").checked,
        showHumidity: document.getElementById("showHumidity").checked,
        showBattery: document.getElementById("showBattery").checked,
        showCO2: document.getElementById("showCO2").checked,
        // showPM25: document.getElementById("showPM25").checked,
        mqttClientId: document.getElementById("mqttClientId").value,
        mqttBroker: document.getElementById("mqttBroker").value,
        mqttUser: document.getElementById("mqttUser").value,
        // mqttPass: document.getElementById("mqttPass").value
        toneBzrBeep: document.getElementById("toneBzrBeep").value,
        durBzrBeep: document.getElementById("durBzrBeep").value,
        timeBtwnBzr: document.getElementById("timeBtwnBzr").value,

        // New preferences for Low Power
        lowPowerMode: document.getElementById("lowPowerMode").value,
        waitToDeep: document.getElementById("waitToDeep").value,
        timeSleeping: document.getElementById("timeSleeping").value,
        cyclsWifiConn: document.getElementById("cyclsWifiConn").value,
        cycRedrawDis: document.getElementById("cycRedrawDis").value,
        actBLEOnWake: document.getElementById("actBLEOnWake").checked,
        actWifiOnWake: document.getElementById("actWifiOnWake").checked,
        actMQTTOnWake: document.getElementById("actMQTTOnWake").checked,
        actESPnowWake: document.getElementById("actESPnowWake").checked,
        displayOnWake: document.getElementById("deepSleepData.displayOnWake").checked
    };
    return preferencesData;
}

function showSavingPopup() {
    // Show the popup
    document.getElementById("popup").style.display = "block";
    console.log("Show popup");

    // After 2 seconds, hide the popup
    setTimeout(function () {
        // Oculta el popup
        document.getElementById("popup").style.display = "none";
        console.log("Hide popup");
    }, 2000);
}

// Function to save preferences
function savePreferences() {
    // Show a popup to indicate that the preferences are being saved
    showSavingPopup();
    // Collect preferences data from the form
    var preferencesData = collectPreferencesData();
    console.log("Sending preferences to server:", preferencesData);

    // Send the preferences data to the server
    var xhr = new XMLHttpRequest();
    xhr.open("POST", "/savepreferences", true);
    xhr.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
    xhr.send(JSON.stringify(preferencesData));

    // Handle the response from the server
    xhr.onload = function () {
        if (xhr.status === 200) {
            console.log("Preferences updated successfully!");
        } else {
            alert("Error updating preferences. Please try again.");
        }
    };
}

function restartESP32() {
    // Show a confirmation dialog
    var isConfirmed = confirm("Are you sure you want to restart the ESP32?");

    // If the user confirms, proceed with the restart
    if (isConfirmed) {
        // Send a request to the ESP32 endpoint to trigger a reset
        fetch('/restart', {
            method: 'GET',
            headers: {
                'Content-Type': 'text/plain'
            }
        })
            .then(response => {
                if (!response.ok) {
                    throw new Error(`HTTP error! Status: ${response.status}`);
                }
                // Reset successful, you can add any additional handling here
                console.log('ESP32 restart initiated');
            })
            .catch(error => {
                // Handle errors during the reset process
                console.error('Error restarting ESP32:', error);
            });
    } else {
        // User chose not to restart, you can add additional logic or leave it empty
    }
}

// Set initial values when the page loads
document.addEventListener("DOMContentLoaded", loadPreferencesFromServer);
document.addEventListener("DOMContentLoaded", fetchVersion);