// Function to fetch version information from the server and display it in the status line
function displayVersion() {
    getVersionStr()
        .then(versionText => {
            document.getElementById("co2GadgetVersion").innerText = "CO2 Gadget: " + versionText;
        })
        .catch(error => {
            console.error('Error:', error);
        });
}

function openTab(evt, tabName) {
    var i, tabContent, tabLinks;
    tabContent = document.getElementsByClassName("tab-content");
    for (i = 0; i < tabContent.length; i++) {
        tabContent[i].style.display = "none";
    }
    tabLinks = document.getElementsByClassName("tab");
    for (i = 0; i < tabLinks.length; i++) {
        tabLinks[i].className = tabLinks[i].className.replace(" active", "");
    }
    document.getElementById(tabName).style.display = "block";
    evt.currentTarget.className += " active";
}

function toggleTab(tabName) {
    var tabContent = document.getElementById(tabName);
    var checkbox = document.getElementById("chk" + tabName);
    if (checkbox.checked) {
        tabContent.style.display = "block";
    } else {
        tabContent.style.display = "none";
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

function loadThresholdsFromServer() {
    fetch('/getThresholdsAsJson')
        .then(response => response.json())
        .then(data => {
            console.log(data);
            document.getElementById("chkDisplay").checked = data[0].enabled;
            document.getElementById("keepAliveTimeDisplay").value = data[0].thrKeepAlive;
            document.getElementById("thrOnlyInLowPDisplay").checked = data[0].thrOnlyInLowP;
            document.getElementById("co2ThresholdAbsDisplay").value = data[0].thrCo2Abs;
            document.getElementById("co2ThresholdPctDisplay").value = data[0].thrCo2Per;
            document.getElementById("tempThresholdAbsDisplay").value = data[0].thrTempAbs;
            document.getElementById("tempThresholdPctDisplay").value = data[0].thrTempPer;
            document.getElementById("humThresholdAbsDisplay").value = data[0].thrHumAbs;
            document.getElementById("humThresholdPctDisplay").value = data[0].thrHumPer;
            document.getElementById("co2ThresholdAnd").checked = data[0].thrCo2CombAnd;
            document.getElementById("tempThresholdAnd").checked = data[0].thrTempCombAnd;
            document.getElementById("humThresholdAnd").checked = data[0].thrHumCombAnd;

            document.getElementById("chkBluetooth").checked = data[1].enabled;
            document.getElementById("keepAliveTimeBluetooth").value = data[1].thrKeepAlive;
            document.getElementById("thrOnlyInLowPBluetooth").checked = data[1].thrOnlyInLowP;
            document.getElementById("co2ThresholdAbsBluetooth").value = data[1].thrCo2Abs;
            document.getElementById("co2ThresholdPctBluetooth").value = data[1].thrCo2Per;
            document.getElementById("tempThresholdAbsBluetooth").value = data[1].thrTempAbs;
            document.getElementById("tempThresholdPctBluetooth").value = data[1].thrTempPer;
            document.getElementById("humThresholdAbsBluetooth").value = data[1].thrHumAbs;
            document.getElementById("humThresholdPctBluetooth").value = data[1].thrHumPer;
            document.getElementById("co2ThresholdAndBluetooth").checked = data[1].thrCo2CombAnd;
            document.getElementById("tempThresholdAndBluetooth").checked = data[1].thrTempCombAnd;
            document.getElementById("humThresholdAndBluetooth").checked = data[1].thrHumCombAnd;

            document.getElementById("chkMQTT").checked = data[2].enabled;
            document.getElementById("keepAliveTimeMQTT").value = data[2].thrKeepAlive;
            document.getElementById("thrOnlyInLowPMQTT").checked = data[2].thrOnlyInLowP;
            document.getElementById("co2ThresholdAbsMQTT").value = data[2].thrCo2Abs;
            document.getElementById("co2ThresholdPctMQTT").value = data[2].thrCo2Per;
            document.getElementById("tempThresholdAbsMQTT").value = data[2].thrTempAbs;
            document.getElementById("tempThresholdPctMQTT").value = data[2].thrTempPer;
            document.getElementById("humThresholdAbsMQTT").value = data[2].thrHumAbs;
            document.getElementById("humThresholdPctMQTT").value = data[2].thrHumPer;
            document.getElementById("co2ThresholdAndMQTT").checked = data[2].thrCo2CombAnd;
            document.getElementById("tempThresholdAndMQTT").checked = data[2].thrTempCombAnd;
            document.getElementById("humThresholdAndMQTT").checked = data[2].thrHumCombAnd;

            document.getElementById("chkESPNOW").checked = data[3].enabled;
            document.getElementById("keepAliveTimeESPNOW").value = data[3].thrKeepAlive;
            document.getElementById("thrOnlyInLowPESPNOW").checked = data[3].thrOnlyInLowP;
            document.getElementById("co2ThresholdAbsESPNOW").value = data[3].thrCo2Abs;
            document.getElementById("co2ThresholdPctESPNOW").value = data[3].thrCo2Per;
            document.getElementById("tempThresholdAbsESPNOW").value = data[3].thrTempAbs;
            document.getElementById("tempThresholdPctESPNOW").value = data[3].thrTempPer;
            document.getElementById("humThresholdAbsESPNOW").value = data[3].thrHumAbs;
            document.getElementById("humThresholdPctESPNOW").value = data[3].thrHumPer;
            document.getElementById("co2ThresholdAndESPNOW").checked = data[3].thrCo2CombAnd;
            document.getElementById("tempThresholdAndESPNOW").checked = data[3].thrTempCombAnd;
            document.getElementById("humThresholdAndESPNOW").checked = data[3].thrHumCombAnd;

            // Toggle visibility of tabs based on checkbox state
            toggle('chkDisplay', 'Display');
            toggle('chkBluetooth', 'Bluetooth');
            toggle('chkMQTT', 'MQTT');
            toggle('chkESPNOW', 'ESPNOW');
        })
        .catch(error => console.error('Error retrieving thresholds:', error));
}

function loadPreferencesFromServer() {
    fetch('/getActualSettingsAsJson')
        .then(response => response.json())
        .then(data => {
            console.log(data);
            // Actualiza los campos de Low Power Options
            document.getElementById("lowPowerMode").value = data.lowPowerMode;
            document.getElementById("waitToDeep").value = data.waitToDeep;
            document.getElementById("timeSleeping").value = data.timeSleeping;
            document.getElementById("cyclsWifiConn").value = data.cyclsWifiConn;
            document.getElementById("cycRedrawDis").value = data.cycRedrawDis;
            document.getElementById("actBLEOnWake").checked = data.actBLEOnWake;
            document.getElementById("actWifiOnWake").checked = data.actWifiOnWake;
            document.getElementById("actMQTTOnWake").checked = data.actMQTTOnWake;
            document.getElementById("actESPnowWake").checked = data.actESPnowWake;
            document.getElementById("deepSleepData.displayOnWake").checked = data.displayOnWake;
        })
        .catch(error => console.error('Error retrieving preferences:', error));
}

function collectThresholdsData() {
    // Collect preferences data from the form
    var thresholdsData = {
        "thresholds": [
            {
                "enabled": document.getElementById("chkDisplay").checked,
                "thrOnlyInLowP": document.getElementById("thrOnlyInLowPDisplay").checked,
                "thrKeepAlive": document.getElementById("keepAliveTimeDisplay").value,
                "thrCo2Abs": document.getElementById("co2ThresholdAbsDisplay").value,
                "thrTempAbs": document.getElementById("tempThresholdAbsDisplay").value,
                "thrHumAbs": document.getElementById("humThresholdAbsDisplay").value,
                "thrCo2Per": document.getElementById("co2ThresholdPctDisplay").value,
                "thrTempPer": document.getElementById("tempThresholdPctDisplay").value,
                "thrHumPer": document.getElementById("humThresholdPctDisplay").value,
                "thrCo2CombAnd": document.getElementById("co2ThresholdAnd").checked,
                "thrTempCombAnd": document.getElementById("tempThresholdAnd").checked,
                "thrHumCombAnd": document.getElementById("humThresholdAnd").checked
            },
            {
                "enabled": document.getElementById("chkBluetooth").checked,
                "thrOnlyInLowP": document.getElementById("thrOnlyInLowPBluetooth").checked,
                "thrKeepAlive": document.getElementById("keepAliveTimeBluetooth").value,
                "thrCo2Abs": document.getElementById("co2ThresholdAbsBluetooth").value,
                "thrTempAbs": document.getElementById("tempThresholdAbsBluetooth").value,
                "thrHumAbs": document.getElementById("humThresholdAbsBluetooth").value,
                "thrCo2Per": document.getElementById("co2ThresholdPctBluetooth").value,
                "thrTempPer": document.getElementById("tempThresholdPctBluetooth").value,
                "thrHumPer": document.getElementById("humThresholdPctBluetooth").value,
                "thrCo2CombAnd": document.getElementById("co2ThresholdAndBluetooth").checked,
                "thrTempCombAnd": document.getElementById("tempThresholdAndBluetooth").checked,
                "thrHumCombAnd": document.getElementById("humThresholdAndBluetooth").checked
            },
            {
                "enabled": document.getElementById("chkMQTT").checked,
                "thrOnlyInLowP": document.getElementById("thrOnlyInLowPMQTT").checked,
                "thrKeepAlive": document.getElementById("keepAliveTimeMQTT").value,
                "thrCo2Abs": document.getElementById("co2ThresholdAbsMQTT").value,
                "thrTempAbs": document.getElementById("tempThresholdAbsMQTT").value,
                "thrHumAbs": document.getElementById("humThresholdAbsMQTT").value,
                "thrCo2Per": document.getElementById("co2ThresholdPctMQTT").value,
                "thrTempPer": document.getElementById("tempThresholdPctMQTT").value,
                "thrHumPer": document.getElementById("humThresholdPctMQTT").value,
                "thrCo2CombAnd": document.getElementById("co2ThresholdAndMQTT").checked,
                "thrTempCombAnd": document.getElementById("tempThresholdAndMQTT").checked,
                "thrHumCombAnd": document.getElementById("humThresholdAndMQTT").checked
            },
            {
                "enabled": document.getElementById("chkESPNOW").checked,
                "thrOnlyInLowP": document.getElementById("thrOnlyInLowPESPNOW").checked,
                "thrKeepAlive": document.getElementById("keepAliveTimeESPNOW").value,
                "thrCo2Abs": document.getElementById("co2ThresholdAbsESPNOW").value,
                "thrTempAbs": document.getElementById("tempThresholdAbsESPNOW").value,
                "thrHumAbs": document.getElementById("humThresholdAbsESPNOW").value,
                "thrCo2Per": document.getElementById("co2ThresholdPctESPNOW").value,
                "thrTempPer": document.getElementById("tempThresholdPctESPNOW").value,
                "thrHumPer": document.getElementById("humThresholdPctESPNOW").value,
                "thrCo2CombAnd": document.getElementById("co2ThresholdAndESPNOW").checked,
                "thrTempCombAnd": document.getElementById("tempThresholdAndESPNOW").checked,
                "thrHumCombAnd": document.getElementById("humThresholdAndESPNOW").checked
            }
        ]
    };
    return thresholdsData;
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

function saveThresholdsToServer() {
    // Collect Thresholds data from the form
    var ThresholdsData = collectThresholdsData();
    console.log("Sending Thresholds to server:", ThresholdsData);

    // Send the preferences data to the server
    var xhr = new XMLHttpRequest();
    xhr.open("POST", "/saveThresholds", true);
    xhr.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
    xhr.send(JSON.stringify(ThresholdsData));

    // Handle the response from the server
    xhr.onload = function () {
        if (xhr.status === 200) {
            console.log("Thresholds updated successfully!");
        } else {
            alert("Error updating thresholds. Please try again.");
        }
    };
}

function savePreferencesToServer() {
    // Collect preferences data from the form
    var lowPowerData = {
        "lowPowerMode": document.getElementById("lowPowerMode").value,
        "waitToDeep": document.getElementById("waitToDeep").value,
        "timeSleeping": document.getElementById("timeSleeping").value,
        "cyclsWifiConn": document.getElementById("cyclsWifiConn").value,
        "cycRedrawDis": document.getElementById("cycRedrawDis").value,
        "actBLEOnWake": document.getElementById("actBLEOnWake").checked,
        "actWifiOnWake": document.getElementById("actWifiOnWake").checked,
        "actMQTTOnWake": document.getElementById("actMQTTOnWake").checked,
        "actESPnowWake": document.getElementById("actESPnowWake").checked,
        "displayOnWake": document.getElementById("deepSleepData.displayOnWake").checked
    };
    console.log("Sending Low Power preferences to server:", lowPowerData);
    fetch('/savePreferences', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify(lowPowerData)
    })
        .then(response => {
            if (response.ok) {
                if (preferencesDebug) console.log("Low Power Lreferences updated successfully!");
            } else {
                alert("Error updating preferences. Please try again.");
            }
        })
        .catch(error => console.error('Error saving Low Power preferences:', error));
}

function saveLowPowerToServer() {
    // Show a popup to indicate that the preferences are being saved
    showSavingPopup();
    saveThresholdsToServer();
    savePreferencesToServer();
}

document.addEventListener("DOMContentLoaded", function () {
    // Get the current URL
    var currentURL = window.location.href;

    // Check if the current URL contains "status.html"
    if (currentURL.includes("low_power.html")) {
        highlightCurrentPage(); // Highlight the current page in the navigation bar
        displayVersion();
    }

    loadPreferencesFromServer();
    loadThresholdsFromServer();

    // Open the first active tab by default
    const tabs = document.querySelectorAll(".tab-content");
    let firstActiveTab = null;

    for (let tab of tabs) {
        const checkbox = document.getElementById("chk" + tab.id);
        if (checkbox && checkbox.checked) {
            firstActiveTab = tab;
            break;
        }
    }

    if (firstActiveTab) {
        firstActiveTab.style.display = "block";
        const tabLink = document.querySelector(`.tab[href="#${firstActiveTab.id}"]`);
        if (tabLink) {
            tabLink.className += " active";
        }
    }
});
