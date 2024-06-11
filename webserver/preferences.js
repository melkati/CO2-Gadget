/**
 * Fetches version information from the server and updates the version displayed
 */
function fetchVersion() {
    fetch('/getVersion')
        .then(response => response.json())
        .then(versionInfo => {
            if (preferencesDebug) console.log('Version information:', versionInfo);
            const versionElement = document.getElementById("co2GadgetVersion");
            const versionText = `CO2 Gadget: v${versionInfo.firmVerMajor}.${versionInfo.firmVerMinor}.${versionInfo.firmRevision}-${versionInfo.firmBranch} (Flavour: ${versionInfo.firmFlavour})`;
            versionElement.innerText = versionText;

            // Adjust preferences.html for specific versions
            if (versionInfo.firmFlavour == 'T-Display S3') {
                const displayBrightInput = document.getElementById("DisplayBright");
                displayBrightInput.min = "1";
                displayBrightInput.max = "16";
                displayBrightInput.step = "1";
                const tooltipText = document.querySelector('.tooltip-text');
                let currentText = tooltipText.textContent;
                currentText += ' Valid brightness values: 1-16.';

            } else {
                // Add to tooltip text: Valid brightness values: displayBrightInput.min to displayBrightInput.max
                const displayBrightInput = document.getElementById("DisplayBright");
                let min = displayBrightInput.min;
                let max = displayBrightInput.max;
                const tooltipText = document.querySelector('.tooltip-text');
                let currentText = tooltipText.textContent;
                currentText += ' Valid brightness values: ' + min + ' to ' + max + '.';
                tooltipText.textContent = currentText;
            }

            // TO-DO: Change to use getFeaturesAsJson endpoint to check for "EINK" instead of firmFlavour to reduce complexity
            if (versionInfo.firmFlavour == 'GDEM0213B74' || versionInfo.firmFlavour == 'DEPG0213BN' || versionInfo.firmFlavour == 'GDEW0213M21' || versionInfo.firmFlavour == 'GDEM029T94' || versionInfo.firmFlavour == 'GDEH0154D67-WeAct' || versionInfo.firmFlavour == 'DEPG0213BN-WeAct' || versionInfo.firmFlavour == 'GDEW0213M21-WeAct' || versionInfo.firmFlavour == 'GDEMGxEPD2_290_BS-WeAct') {
                document.getElementById("displayBrightDiv").classList.add("hidden");
                document.getElementById("dispOffOnExPDiv").classList.add("hidden");
                document.getElementById("tToDispOffDiv").classList.add("hidden");
            }
        })
        .catch(error => console.error('Error fetching version information:', error));
}

/**
 * Populates the preferences form with the provided preferences data.
 * @param {Object} preferences - The preferences data to populate the form with.
 */
function populateFormWithPreferences(preferences) {
    // Update relaxedSecurity if present in data
    if (preferences.relaxedSecurity !== undefined) {
        if (preferencesDebug) console.log(`Setting relaxedSecurity field to:`, preferences.relaxedSecurity);
        // If forcedRelaxedSecurity is set because the current URL does contains the "relaxedSecurity" parameter, do not overide the value with the one from the server
        if (!forcedRelaxedSecurity) {
            relaxedSecurity = preferences.relaxedSecurity;
        }
    }

    handlePasswordFields();

    // Helper function to set form values
    const setFormValue = (elementId, value) => {
        const element = document.getElementById(elementId);
        if (element) element.value = value;
        if (preferencesDebug) console.log(`Setting ${elementId} to:`, value);
    };

    // Helper function to set form checkbox
    const setFormCheckbox = (elementId, isChecked) => {
        const element = document.getElementById(elementId);
        if (element) element.checked = isChecked;
        if (preferencesDebug) console.log(`Setting ${elementId} to:`, isChecked);
    };

    setFormCheckbox("activeWIFI", preferences.activeWIFI);
    setFormValue("wifiSSID", preferences.wifiSSID);
    if (relaxedSecurity) setFormValue("wifiPass", preferences.wifiPass);
    setFormValue("hostName", preferences.hostName);
    setFormCheckbox("useStaticIP", preferences.useStaticIP);
    setFormValue("staticIP", preferences.staticIP);
    setFormValue("gateway", preferences.gateway);
    setFormValue("subnet", preferences.subnet);
    setFormValue("dns1", preferences.dns1);
    setFormValue("dns2", preferences.dns2);
    setFormValue("selCO2Sensor", preferences.selCO2Sensor);
    setFormCheckbox("autoSelfCalibration", preferences.autoSelfCal);
    setFormValue("customCalValue", preferences.customCalValue);
    setFormValue("co2OrangeRange", preferences.co2OrangeRange);
    setFormValue("co2RedRange", preferences.co2RedRange);
    setFormValue("tempOffset", preferences.tempOffset);
    setFormCheckbox("showFahrenheit", preferences.showFahrenheit);
    setFormValue("altitudeMeters", preferences.altitudeMeters);
    setFormValue("measurementInterval", preferences.measurementInterval);
    setFormCheckbox("outModeRelay", preferences.outModeRelay);
    setFormValue("channelESPNow", preferences.channelESPNow);
    setFormValue("boardIdESPNow", preferences.boardIdESPNow);
    setFormValue("peerESPNowAddress", preferences.peerESPNowAddress);
    setFormValue("neopixBright", preferences.neopixBright);
    setFormValue("selNeopxType", preferences.selNeopxType);
    setFormCheckbox("activeBLE", preferences.activeBLE);
    setFormCheckbox("activeMQTT", preferences.activeMQTT);
    setFormCheckbox("activeESPNOW", preferences.activeESPNOW);
    setFormValue("mqttClientId", preferences.mqttClientId);
    setFormCheckbox("mqttShowInCon", preferences.mqttShowInCon);
    setFormValue("rootTopic", preferences.rootTopic);
    setFormValue("mqttBroker", preferences.mqttBroker);
    setFormValue("mqttUser", preferences.mqttUser);
    if (relaxedSecurity) setFormValue("mqttPass", preferences.mqttPass);
    setFormValue("batDischgd", preferences.batDischgd);
    setFormValue("batChargd", preferences.batChargd);
    setFormValue("vRef", preferences.vRef);
    setFormValue("tToDispOff", preferences.tToDispOff);
    setFormValue("tToPubMQTT", preferences.tToPubMQTT);
    setFormValue("tToPubESPNow", preferences.tToPubESPNow);
    setFormValue("tKeepAlMQTT", preferences.tKeepAlMQTT);
    setFormValue("tKeepAlESPNow", preferences.tKeepAlESPNow);
    setFormCheckbox("showTemp", preferences.showTemp);
    setFormCheckbox("showHumidity", preferences.showHumidity);
    setFormCheckbox("showBattery", preferences.showBattery);
    setFormCheckbox("showCO2", preferences.showCO2);
    setFormCheckbox("dispOffOnExP", preferences.dispOffOnExP);
    setFormCheckbox("displayReverse", preferences.displayReverse);
    setFormValue("DisplayBright", preferences.DisplayBright);
    setFormCheckbox("debugSensors", preferences.debugSensors);
    setFormValue("toneBzrBeep", preferences.toneBzrBeep);
    setFormValue("durBzrBeep", preferences.durBzrBeep);
    setFormValue("timeBtwnBzr", preferences.timeBtwnBzr);

    // New fields for Captive Portal
    if (preferences.cpNoTimeout !== undefined) {
        setFormCheckbox("cpNoTimeout", preferences.cpNoTimeout);
    }
    if (preferences.cpRelaxedSec !== undefined) {
        setFormCheckbox("cpRelaxedSec", preferences.cpRelaxedSec);
    }
    if (preferences.cpDebug !== undefined) {
        setFormCheckbox("cpDebug", preferences.cpDebug);
    }
    if (preferences.cpWaitTime !== undefined) {
        setFormValue("cpWaitTime", preferences.cpWaitTime);
    }

    toggleVisibility('activeWIFI', 'wifiNetworks', (isChecked) => {
        document.getElementById('mqttConfig').style.display = isChecked ? 'block' : 'none';
    });
    toggleVisibility('activeMQTT', 'mqttConfig');
    toggleVisibility('activeESPNOW', 'espNowConfig');
    toggleVisibility('useStaticIP', 'staticIPSettings');

    // Handle dependencies after form is populated
    handleWiFiMQTTDependency();
}

/**
 * Fetches preferences data from the server and populates the form.
 */
function loadPreferencesFromServer() {
    fetch('/getActualSettingsAsJson' + (relaxedSecurity ? '?relaxedSecurity=true' : ''))
        .then(response => response.json())
        .then(preferences => {
            if (preferencesDebug) console.log('Get preferences from server response:', preferences);
            populateFormWithPreferences(preferences);
        })
        .catch(error => console.error('Error retrieving preferences:', error));
}

/**
 * Collects preferences data from the form.
 * @returns {Object} - The collected preferences data.
 */
function collectPreferencesData() {
    const preferencesData = {
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
        hostName: document.getElementById("hostName").value,
        useStaticIP: document.getElementById("useStaticIP").checked,
        staticIP: document.getElementById("staticIP").value,
        gateway: document.getElementById("gateway").value,
        subnet: document.getElementById("subnet").value,
        dns1: document.getElementById("dns1").value,
        dns2: document.getElementById("dns2").value,
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
        mqttClientId: document.getElementById("mqttClientId").value,
        mqttShowInCon: document.getElementById("mqttShowInCon").checked,
        mqttBroker: document.getElementById("mqttBroker").value,
        mqttUser: document.getElementById("mqttUser").value,
        toneBzrBeep: document.getElementById("toneBzrBeep").value,
        durBzrBeep: document.getElementById("durBzrBeep").value,
        timeBtwnBzr: document.getElementById("timeBtwnBzr").value
    };

    if (relaxedSecurity) {
        preferencesData.wifiPass = document.getElementById("wifiPass").value;
        preferencesData.mqttPass = document.getElementById("mqttPass").value;
    }

    // New fields for Captive Portal
    const cpNoTimeout = document.getElementById("cpNoTimeout");
    if (cpNoTimeout) {
        preferencesData.cpNoTimeout = cpNoTimeout.checked;
    }
    const cpRelaxedSec = document.getElementById("cpRelaxedSec");
    if (cpRelaxedSec) {
        preferencesData.cpRelaxedSec = cpRelaxedSec.checked;
    }
    const cpDebug = document.getElementById("cpDebug");
    if (cpDebug) {
        preferencesData.cpDebug = cpDebug.checked;
    }
    const cpWaitTime = document.getElementById("cpWaitTime");
    if (cpWaitTime) {
        preferencesData.cpWaitTime = cpWaitTime.value;
    }

    console.log("Collected preferences data:", preferencesData);

    return preferencesData;
}

/**
 * Shows a popup message indicating that preferences are being saved.
 */
function showSavingPopup() {
    const popup = document.getElementById("popup");
    popup.style.display = "block";
    console.log("Show popup");

    setTimeout(() => {
        popup.style.display = "none";
        console.log("Hide popup");
    }, 2000);
}

/**
 * Saves preferences to the server.
 */
function savePreferences() {
    showSavingPopup();
    const preferencesData = collectPreferencesData();
    if (preferencesDebug) console.log("Sending preferences to server:", preferencesData);

    fetch('/savePreferences', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify(preferencesData)
    })
        .then(response => {
            if (response.ok) {
                console.log("Preferences updated successfully!");
            } else {
                alert("Error updating preferences. Please try again.");
            }
        })
        .catch(error => console.error('Error saving preferences:', error));
}

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
 * Backs up preferences by saving them to a JSON file.
 */
function backupPreferences() {
    const preferencesData = collectPreferencesData();
    const jsonData = JSON.stringify(preferencesData);
    const blob = new Blob([jsonData], { type: 'application/json' });
    const a = document.createElement('a');

    a.href = URL.createObjectURL(blob);
    a.download = 'preferences_backup.json';
    document.body.appendChild(a);
    a.click();
    document.body.removeChild(a);

    alert('Preferences backup completed successfully!');
}

/**
 * Restores preferences from the provided JSON data.
 * @param {string} preferencesData - The JSON string containing preferences data.
 */
function restorePreferencesFromData(preferencesData) {
    const preferences = JSON.parse(preferencesData);
    populateFormWithPreferences(preferences);
}

/**
 * Triggers file input click event to restore preferences from a file.
 */
function chooseFileAndRestore() {
    document.getElementById('fileInput').click();
}

/**
 * Handles file selection for restoring preferences.
 * @param {Event} event - The file selection event.
 */
function handleFileSelection(event) {
    const file = event.target.files[0];
    if (file) {
        const reader = new FileReader();
        reader.readAsText(file, 'UTF-8');
        reader.onload = (evt) => {
            const preferencesData = evt.target.result;
            restorePreferencesFromData(preferencesData);
        };
        reader.onerror = (evt) => {
            console.error("Error reading file:", evt.target.error);
            alert("Error reading file. Please try again.");
        };
    }
}

/**
 * Toggles the visibility of an element based on a checkbox state and handles subdependencies.
 * @param {string} checkboxId - The ID of the checkbox.
 * @param {string} elementId - The ID of the element to toggle.
 * @param {function} [callback] - Optional callback function to handle additional dependencies.
 */
function toggleVisibility(checkboxId, elementId, callback) {
    const checkbox = document.getElementById(checkboxId);
    const element = document.getElementById(elementId);

    if (!checkbox || !element) {
        console.error(`Checkbox or element not found: ${checkboxId}, ${elementId}`);
        return;
    }

    const toggleElement = () => {
        element.style.display = checkbox.checked ? 'block' : 'none';
        if (callback) callback(checkbox.checked);
    };

    toggleElement();
    checkbox.addEventListener('change', toggleElement);
}

/**
 * Ensures that MQTT is disabled when WiFi is disabled, and enabled when WiFi is enabled.
 */
function handleWiFiMQTTDependency() {
    const wifiCheckbox = document.getElementById('activeWIFI');
    const mqttCheckbox = document.getElementById('activeMQTT');

    if (!wifiCheckbox || !mqttCheckbox) {
        console.error('Checkboxes not found: activeWIFI, activeMQTT');
        return;
    }

    const updateMQTTState = () => {
        if (!wifiCheckbox.checked) {
            mqttCheckbox.checked = false;
            mqttCheckbox.disabled = true;
            document.getElementById('mqttConfig').style.display = 'none';
        } else {
            mqttCheckbox.disabled = false;
        }
    };

    const updateWiFiState = () => {
        if (mqttCheckbox.checked) {
            wifiCheckbox.checked = true;
        }
        toggleVisibility('activeMQTT', 'mqttConfig');
    };

    wifiCheckbox.addEventListener('change', updateMQTTState);
    mqttCheckbox.addEventListener('change', updateWiFiState);

    // Initial call to set the correct state on page load
    updateMQTTState();
    updateWiFiState();
}

/**
 * Updates the battery voltage displayed on the page.
 */
function updateBatteryVoltage(voltage) {
    document.getElementById('currentVoltage').textContent = voltage + ' Volts';
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

/**
 * Updates the VRef value on the server.
 */
function updateVRef() {
    const vRefValue = document.getElementById('vRef').value;
    fetch(`/settings?SetVRef=${vRefValue}`)
        .then(response => {
            if (!response.ok) throw new Error('Error updating VRef');
            console.log('VRef updated successfully');
        })
        .catch(error => console.error('Error updating VRef:', error));
}

/**
 * Enables or disables password fields based on the relaxed security mode.
 */
function handlePasswordFields() {
    const inputField = document.getElementById("wifiSSID");
    const passwordFields = document.querySelectorAll('input[type=password]');

    passwordFields.forEach(field => {
        if (relaxedSecurity) {
            field.removeAttribute('disabled');
            if (preferencesDebug) console.log(`Field ${field.id} enabled`);
        } else {
            field.disabled = true;
            field.value = '';
            field.placeholder = "Password (disabled)";
            if (preferencesDebug) console.log(`Field ${field.id} disabled`);
        }
    });

    if (relaxedSecurity) {
        inputField.removeAttribute('readonly');
    } else {
        inputField.setAttribute('readonly', 'readonly');
    }
}

function calibrateSensor(calibrationValue) {
    // Implement the calibration logic here
    if (calibrationValue > 400 && calibrationValue < 2000) {
        console.log("Calibration process started...");
        console.log("Calibration value:", calibrationValue);
        fetch(`/settings?CalibrateCO2=${calibrationValue}`)
            .then(response => {
                if (!response.ok) throw new Error('Error calibrating CO2 sensor');
                console.log('CO2 sensor calibrated successfully');
            })
            .catch(error => console.error('Error calibrating CO2 sensor:', error));
    } else {
        console.error(
            "Invalid calibration value, please enter a value between 400 and 2000 ppm"
        );
        console.log("Calibration value:", calibrationValue);
    }
}

/**
 * Handles the calibration wizard functionality.
 */
function handleCalibrationWizard() {
    const calibrateButton = document.getElementById("calibrateButton");
    const calibrationModal = document.getElementById("calibrationModal");
    const countdownModal = document.getElementById("countdownModal");
    const closeSpan = document.querySelector(".close");
    const acknowledgeCheckbox = document.getElementById("acknowledgeCheckbox");
    const acknowledgeLabel = document.getElementById("acknowledgeLabel");
    const calibrateNowButton = document.getElementById("calibrateNowButton");
    const countdownSpan = document.getElementById("countdown");
    const currentCO2ValueSpan = document.getElementById("currentCO2Value");

    // Function to fetch current CO2 value from /readCO2 endpoint as text
    function getCurrentCO2Value() {
        fetch("/readCO2")
            .then((response) => response.text())
            .then((data) => {
                let co2Value = parseFloat(data);
                document.getElementById("currentCO2Value").textContent =
                    co2Value.toFixed(0);
            })
            .catch((error) => {
                console.error("Error fetching CO2 data", error);
            });
    }

    // Function to change text color when "Calibrate Now" button is clicked without acknowledging
    function changeTextColor() {
        acknowledgeLabel.style.color = "var(--unchecked-font-color)";
    }

    calibrateButton.addEventListener("click", () => {
        // Uncheck the acknowledge checkbox when modal is opened
        acknowledgeCheckbox.checked = false;
        calibrationModal.style.display = "block";
        // Set the custom calibration value input to the current custom calibration value
        const customCalibrationValueInput = document.getElementById(
            "modalCustomCalibrationValueInput"
        );
        customCalibrationValueInput.value =
            document.getElementById("customCalValue").value;
        // Fetch current CO2 value when modal is opened
        getCurrentCO2Value();
        // Start interval to update CO2 value every 5 seconds
        updateCO2Interval = setInterval(getCurrentCO2Value, 5000);
    });

    closeSpan.addEventListener("click", () => {
        calibrationModal.style.display = "none";
        // Clear interval when modal is closed
        clearInterval(updateCO2Interval);
    });

    acknowledgeCheckbox.addEventListener("change", () => {
        // Reset text color when checkbox is checked
        if (acknowledgeCheckbox.checked) {
            acknowledgeLabel.style.color = ""; // Reset to default color
        }
    });

    calibrateNowButton.addEventListener("click", () => {
        if (!acknowledgeCheckbox.checked) {
            changeTextColor(); // Call the function to change text color
            return; // Exit function if checkbox is not acknowledged
        }

        calibrationModal.style.display = "none";
        countdownModal.style.display = "block";

        const customCalValue = parseInt(
            document.getElementById("modalCustomCalibrationValueInput").value,
            10
        );
        calibrateSensor(customCalValue);

        let countdown = 15;
        countdownSpan.textContent = countdown;

        const interval = setInterval(() => {
            countdown -= 1;
            countdownSpan.textContent = countdown;

            if (countdown <= 0) {
                clearInterval(interval);
                countdownModal.style.display = "none";
            }
        }, 1000);
    });

    window.addEventListener("click", (event) => {
        if (event.target == calibrationModal) {
            calibrationModal.style.display = "none";
            // Clear interval when modal is closed
            clearInterval(updateCO2Interval);
        }
    });
}

/**
 * Sanity Data form before Save 
 */
function sanityCheckData() {
    var txt_error = "";
    let errorMessage = document.getElementById('error-message');

    // Sanyty check for display brightness
    const inputDisplayBrightness = document.getElementById("DisplayBright");
    if (parseInt(inputDisplayBrightness.value) > parseInt(inputDisplayBrightness.max) || parseInt(inputDisplayBrightness.value) < parseInt(inputDisplayBrightness.min)) {
        inputDisplayBrightness.classList.remove('valid');
        inputDisplayBrightness.classList.add('error');

        if (parseInt(inputDisplayBrightness.value) > parseInt(inputDisplayBrightness.max)) inputDisplayBrightness.value = inputDisplayBrightness.max;
        if (parseInt(inputDisplayBrightness.value) < parseInt(inputDisplayBrightness.min)) inputDisplayBrightness.value = inputDisplayBrightness.min;

        txt_error = "Display Brightness value must be >=" + inputDisplayBrightness.min + " and <=" + inputDisplayBrightness.max;
        if (errorMessage) errorMessage.remove(); // Remove previous error messages
        errorMessage = document.createElement('div');
        errorMessage.id = 'error-message';
        errorMessage.className = 'form-error';
        errorMessage.textContent = txt_error;
        inputDisplayBrightness.insertAdjacentElement('afterend', errorMessage);
        if (preferencesDebug) console.log(txt_error);
        return false;
    } else {
        if (errorMessage) errorMessage.remove();
        inputDisplayBrightness.classList.remove('error');
        inputDisplayBrightness.classList.add('valid');
    }

    // Sanyty check for CO2 Orange and Red Range
    const inputco2OrangeRange = document.getElementById("co2OrangeRange");
    const inputco2RedRange = document.getElementById("co2RedRange");
    if (parseInt(inputco2OrangeRange.value) > parseInt(inputco2RedRange.value)) {
        inputco2RedRange.value = parseInt(inputco2OrangeRange.value) + 1;
        inputco2OrangeRange.classList.remove('valid');
        inputco2RedRange.classList.remove('valid');
        inputco2OrangeRange.classList.add('error');
        inputco2RedRange.classList.add('error');
        txt_error = "Red level must be greater than Orange level";
        if (errorMessage) errorMessage.remove(); // Remove previous error messages
        errorMessage = document.createElement('div');
        errorMessage.id = 'error-message';
        errorMessage.className = 'form-error';
        errorMessage.textContent = txt_error;
        inputco2RedRange.insertAdjacentElement('afterend', errorMessage);
        if (preferencesDebug) console.log(txt_error);
        return false;
    } else {
        if (errorMessage) errorMessage.remove();
        inputco2OrangeRange.classList.remove('error');
        inputco2RedRange.classList.remove('error');
        inputco2OrangeRange.classList.add('valid');
        inputco2RedRange.classList.add('valid');
    }
    return true;
}

/**
 * Runtime display reverse 
 */
function toggleDisplayReverse() {
    if (preferencesDebug) console.log("Toggle Display Reverse");
    fetch("/settings?ToggleDisplayReverse")
        .then(response => {
            if (!response.ok) throw new Error('Error reversing display');
            if (preferencesDebug) console.log('Toggle Display Reverse successfully');
        })
        .catch(error => console.error('Error Toggling Display Reverse:', error));
}

/**
 * Runtime display brightness 
 */
function setDisplayBrightness() {
    const inputDisplayBrightness = document.getElementById("DisplayBright").value;
    if (preferencesDebug) console.log("Set Display Brightness = " + inputDisplayBrightness);
    fetch(`/settings?setDisplayBrightness=${inputDisplayBrightness}`)
        .then(response => {
            if (!response.ok) throw new Error('Error setting display brightness');
            if (preferencesDebug) console.log('Set Display brightness successfully');
        })
        .catch(error => console.error('Error Setting Display brightness:', error));
}

/**
* Runtime show/hide Temp/Humidity/Battery in display
*/
function showTempHumBatt() {
    const inputShowTemp = document.getElementById("showTemp").checked;
    const inputShowHumidity = document.getElementById("showHumidity").checked;
    const inputShowBattery = document.getElementById("showBattery").checked;
    if (preferencesDebug) console.log("Show/hide Temp/Humidity/Battery in display");
    fetch(`/settings?showTemp=${inputShowTemp}&showHumidity=${inputShowHumidity}&showBattery=${inputShowBattery}`)
        .then(response => {
            if (!response.ok) throw new Error('Error setting show/hide Temp/Humidity/Battery in display');
            if (preferencesDebug) console.log('Set show/hide Temp/Humidity/Battery in display successfully');
        })
        .catch(error => console.error('Error show/hide Temp/Humidity/Battery in display', error));
}

document.addEventListener("DOMContentLoaded", () => {
    // Get the current URL
    var currentURL = window.location.href;

    // Check if the current URL contains "preferences.html"
    if (currentURL.includes("preferences.html")) {
        highlightCurrentPage(); // Highlight the current page in the navigation bar
        forcedRelaxedSecurity = currentURL.includes("relaxedSecurity");
        relaxedSecurity = forcedRelaxedSecurity;
        forceCaptivePortalActive = currentURL.includes("forceCaptivePortalActive");
        handlePasswordFields();
        setTimeout(() => { loadPreferencesFromServer(); }, 50); // Delay of 100ms
        setTimeout(() => { fetchVersion(); }, 100); // Delay of 100ms
        toggleVisibility('activeWIFI', 'wifiNetworks', (isChecked) => {
            document.getElementById('mqttConfig').style.display = isChecked ? 'block' : 'none';
        });
        toggleVisibility('activeMQTT', 'mqttConfig');
        toggleVisibility('activeESPNOW', 'espNowConfig');
        toggleVisibility('useStaticIP', 'staticIPSettings');
        handleWiFiMQTTDependency();
        setTimeout(() => { handleCalibrationWizard(); }, 200); // Delay of 500ms

        // Update the battery voltage every second
        setInterval(fetchAndUpdateBatteryVoltage, 1000);

        // Listen for input events on the voltage reference field with a delay of 100ms
        document.getElementById('vRef').addEventListener('input', () => {
            setTimeout(updateVRef, 100);
        });
    }
});