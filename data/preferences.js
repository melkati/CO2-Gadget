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

function populateFormWithPreferences(preferences) {

    let getPreferencesDebug = true; // Set to false to disable debug output
    if (getPreferencesDebug) {
        console.log('Loading preferences:', preferences);
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

    // Toggle visibility of elements based on checkbox state
    toggle('activeWIFI', 'wifiNetworks');
    toggle('activeMQTT', 'mqttConfig');
    toggle('activeESPNOW', 'espNowConfig');
}

function loadPreferencesFromServer() {
    fetch('/getActualSettingsAsJson')
        .then(response => response.json())
        .then(preferences => {
            populateFormWithPreferences(preferences);
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

// Function to save preferences to server NVR
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

// Function to backup preferences
function backupPreferences() {
    // Collect preferences data
    var preferencesData = collectPreferencesData();

    // Convert preferences data to JSON format
    var jsonData = JSON.stringify(preferencesData);

    // Create a Blob object with the JSON data
    var blob = new Blob([jsonData], { type: 'application/json' });

    // Create a link element
    var a = document.createElement('a');

    // Set the href attribute to a URL representing the Blob data
    a.href = URL.createObjectURL(blob);

    // Set the download attribute to specify the filename
    a.download = 'preferences_backup.json';

    // Append the link to the document body
    document.body.appendChild(a);

    // Programmatically click the link to trigger the download
    a.click();

    // Remove the link from the document body
    document.body.removeChild(a);

    // Show a message to the user indicating that the backup is completed
    alert('Preferences backup completed successfully!');
}

// Function to trigger file input click and restore preferences
function chooseFileAndRestore() {
    document.getElementById('fileInput').click();
}

// Function to handle file selection
function handleFileSelection(event) {
    var file = event.target.files[0];
    if (file) {
        var reader = new FileReader();
        reader.readAsText(file, 'UTF-8');
        reader.onload = function (evt) {
            var preferencesData = evt.target.result;
            restorePreferencesFromData(preferencesData);
        }
        reader.onerror = function (evt) {
            console.error("Error reading file:", evt.target.error);
            alert("Error reading file. Please try again.");
        }
    }
}

function toggle(checkboxId, elementId) {
    const checkbox = document.getElementById(checkboxId);
    if (!checkbox) {
        console.error('Checkbox with id ' + checkboxId + ' is not found.');
        return;
    }

    const element = document.getElementById(elementId);
    if (!element) {
        console.error('Element with id ' + elementId + ' is not found.');
        return;
    }


    // Function to toggle element visibility
    function toggleElement() {
        element.style.display = checkbox.checked ? 'block' : 'none';
        // if (checkbox.checked) {
        //     element.style.display = 'block'; // Show the element if checkbox is checked
        // } else {
        //     element.style.display = 'none'; // Hide the element if checkbox is unchecked
        // }
    }

    // Call toggleElement initially to set initial state
    toggleElement();

    // Add event listener to checkbox to update visibility when checkbox state changes
    checkbox.addEventListener('change', toggleElement);
}

function updateVoltage() {
    fetch('/readBatteryVoltage')
        .then(response => response.text())
        .then(voltage => {
            document.getElementById('currentVoltage').textContent = voltage + ' Volts';
        })
        .catch(error => console.error('Error fetching battery voltage:', error));
}

function updateVRef() {
    // Enviar el nuevo valor de VRef al servidor
    fetch('/settings?SetVRef=' + document.getElementById('vRef').value)
        .then(response => {
            if (!response.ok) {
                throw new Error('Error updating VRef');
            }
            console.log('VRef updated successfully');
        })
        .catch(error => console.error('Error updating VRef:', error));
}

// Update the battery voltage every second
setInterval(updateVoltage, 1000);

// Lisen for input events on the voltage reference field with a delay of 100ms
document.getElementById('vRef').addEventListener('input', () => {
    setTimeout(updateVRef, 100);
});

document.addEventListener("DOMContentLoaded", function () {
    loadPreferencesFromServer();
    fetchVersion();
    toggle('activeWIFI', 'wifiNetworks');
    toggle('activeMQTT', 'mqttConfig');
    toggle('activeESPNOW', 'espNowConfig');
});