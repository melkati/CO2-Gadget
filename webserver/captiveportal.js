var captivePortalDebug = false;
var forcedCaptivePortalDebug = false;
var captivePortalActive = false;
var forcedCaptivePortal = false;
var captivePortalNoTimeout = false;
var forcedCaptivePortalNoTimeout = false;
var relaxedSecurity = false;

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

/**
 * Sets the captive portal settings and sends them to the server.
 * @param {number} timeToWait - The time to wait for the captive portal.
 * @param {boolean} isInitialSetup - Whether this is the initial setup.
 */
function setCaptivePortalSettings(timeToWait, isInitialSetup = false) {

    // captivePortalNoTimeout is not loaded with the form value to obey the override from the URL

    // const disableTimeoutCheckbox = document.getElementById('cpNoTimeout');
    // captivePortalNoTimeout = disableTimeoutCheckbox ? disableTimeoutCheckbox.checked : false;

    // if (!disableTimeoutCheckbox && !isInitialSetup) {
    //     console.error('Element with ID "cpNoTimeout" not found.');
    //     return;
    // }

    // if (captivePortalDebug) {
    //     console.log('Setting captivePortalNoTimeout to:', captivePortalNoTimeout);
    //     console.log('Time to wait for timeout:', timeToWait);
    // }

    // if (disableTimeoutCheckbox && disableTimeoutCheckbox.checked) {
    //     if (captivePortalDebug || forcedCaptivePortalDebug) console.log('Disabling timeout for captive portal');
    //     timeToWait = 0;
    //     captivePortalNoTimeout = true;
    // }

    fetch('/setCaptivePortalSettings', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
            timeToWaitForCaptivePortal: timeToWait,
            captivePortalActive,
            captivePortalNoTimeout,
            forcedCaptivePortal,
            captivePortalDebug,
            relaxedSecurity: relaxedSecurity ? true : undefined
        })
    })
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok');
            }
            if (captivePortalDebug) {
                console.log('Captive Portal settings updated on server successfully:', {
                    forcedCaptivePortal,
                    captivePortalActive,
                    captivePortalNoTimeout,
                    timeToWaitForCaptivePortal: timeToWait,
                    captivePortalDebug,
                    relaxedSecurity
                });
            }
        })
        .catch(error => console.error('Error setting captive portal settings:', error));
}

/**
 * Stores the captive portal status in the captivePortalStatus object.
 * @param {Object} data - The data containing the captive portal status.
 */
function storeCaptivePortalStatus(data) {
    captivePortalStatus.captivePortalTimeLeft = data.captivePortalTimeLeft;
}

/**
 * Handles the response data from the captive portal status fetch and updates the UI.
 * @param {Object} data - The data received from the server.
 */
function handleCaptivePortalData(data) {
    try {
        storeCaptivePortalStatus(data);
        const changes = checkForPropertyChanges(data);
        logPropertyChanges(changes);

        updateDebugMode(data);
        updateActiveStates(data);
        // updateStatusBarContent();

        if (debugWindowActive) {
            updateDebugWindow(data);
        }
    } catch (error) {
        console.error('Error in handleCaptivePortalData function:', error);
        throw error;
    }
}

/**
 * Checks for changes in specific properties compared to previous data.
 * @param {Object} data - The data received from the server.
 * @returns {Object} - Object containing changes in properties.
 */
function checkForPropertyChanges(data) {
    const changes = {};
    const propertiesToCheck = ['forceCaptivePortalActive', 'captivePortalActive', 'forcedCaptivePortal', 'captivePortalDebug', 'relaxedSecurity', 'forcedCaptivePortalDebug', 'captivePortalNoTimeout'];

    propertiesToCheck.forEach(key => {
        if (data[key] !== previousData[key]) {
            changes[key] = { previous: previousData[key], current: data[key] };
            previousData[key] = data[key];
        }
    });

    return changes;
}

/**
 * Logs changes in properties to the console.
 * @param {Object} changes - Object containing changes in properties.
 */
function logPropertyChanges(changes) {
    if (Object.keys(changes).length > 0) {
        console.log('Detected changes in captive portal data:', changes);
    }
}

/**
 * Updates the debug mode if present in the data.
 * @param {Object} data - The data received from the server.
 */
function updateDebugMode(data) {
    if (data.captivePortalDebug !== undefined) {
        if (data.captivePortalDebug !== captivePortalDebug) {
            captivePortalDebug = data.captivePortalDebug;
            console.log('Captive portal debug mode set to:', captivePortalDebug);
        }
    }
}

/**
 * Updates the active states based on the data received.
 * @param {Object} data - The data received from the server.
 */
function updateActiveStates(data) {
    forcedCaptivePortal = data.forceCaptivePortalActive || false;
    captivePortalActive = (data.captivePortalActive || false) || forcedCaptivePortal;
}

/**
 * Handles the features data and updates the SUPPORT_* properties accordingly.
 * @param {Object} features - The features data object.
 */
function handleFeaturesData(data) {
    features = data;
    if (captivePortalDebug) console.log('Features:', features);
}

/**
 * Fetches features as JSON from the server and handles the response.
 */
function getFeaturesAsJson() {
    fetch("/getFeaturesAsJson")
        .then(response => {
            if (!response.ok) {
                // if (captivePortalDebug) console.log("Received response:", response);
                console.error("Response not OK:", response.status, response.statusText);
                throw new Error("Network response was not ok " + response.statusText);
            }
            return response.json();
        })
        .then(data => {
            console.log("Received JSON:", data);
            handleFeaturesData(data);
            showCO2GadgetFeatures();
        })
        .catch(error => {
            console.error("Error fetching CO2 Gadget features:", error);
        });
}

/**
 * Fetches the captive portal settings from the server.
 */
function getCaptivePortalSettings() {
    fetch("/getCaptivePortalStatusAsJson")
        .then(response => {
            // if (captivePortalDebug) console.log("Received response:", response);

            // Check if the response status is OK
            if (!response.ok) {
                console.error("Response not OK:", response.status, response.statusText);
                throw new Error("Network response was not ok " + response.statusText);
            }

            // Convert the response body to JSON
            return response.json();
        })
        .then(captivePortalSettings => {
            // if (captivePortalDebug) console.log("Received JSON:", captivePortalSettings);

            // Handle the JSON data
            try {
                handleCaptivePortalData(captivePortalSettings);
            } catch (e) {
                console.error("Error in handleCaptivePortalData:", e);
                throw e; // Re-throw the error to be caught in the catch block
            }
        })
        .catch(error => {
            console.error("Error fetching captive portal status:", error);
            // showServerStatusDot(true, "status-dot-red", "Connection to server lost");
        });
}

/**
 * Sets up the initial settings based on URL parameters.
 */
function setupInitialSettings() {
    if (window.location.href.includes("forcedCaptivePortal")) {
        if (captivePortalDebug) console.log('Forcing captive portal to be active in test mode by forcedCaptivePortal parameter in URL');
        forcedCaptivePortal = true;
        captivePortalActive = true;
    }

    if (window.location.href.includes("debugCaptivePortal")) {
        forcedCaptivePortalDebug = true;
        captivePortalDebug = true;
        console.log('Forcing captive portal debug mode to be active by debugCaptivePortal parameter in URL');
    }

    setCaptivePortalSettings(60, true);
}

/**
 * Initialize the captive portal for preferences.html.
 */
function initializeCaptivePortal() {
    if (captivePortalDebug) console.log('Document loaded. Initializing captive portal for preferences.html');

    getFeaturesAsJson(); // Fetch features from the server
    if (features.SUPPORT_OTA) {
        const otaLink = document.getElementById('otaLink');
        if (otaLink) {
            otaLink.classList.remove('hidden');
        } else {
            console.error('Element with ID "otaLink" not found.');
        }
    }

    // getCaptivePortalSettings(); // Fetch initial settings
    setupInitialSettings();

    setInterval(getCaptivePortalSettings, 1000);
}


/**
 * Shows or hides the captivePortalSettings fieldset based on the provided flag.
 * @param {boolean} show - Determines whether to show or hide the captivePortalSettings fieldset.
 */
function showCaptivePortalSettings(show) {
    const captivePortalSettings = document.getElementById('captivePortalSettings');
    if (captivePortalSettings) {
        if (show) {
            captivePortalSettings.classList.remove('hidden');
        } else {
            captivePortalSettings.classList.add('hidden');
        }
    } else {
        console.error('Element with ID "captivePortalSettings" not found.');
    }
}

document.addEventListener("DOMContentLoaded", function () {
    const currentPage = window.location.pathname.split("/").pop();

    adjustContentPadding();

    if (currentPage === "preferences.html") {
        initializeCaptivePortal();
        highlightCurrentPage(); // Highlight the current page in the navigation bar

        const debugWindow = document.getElementById('debug-window');
        if (captivePortalDebug || debugWindowActive) {
            if (debugWindow) {
                createDebugWindow(debugWindow);
                debugWindowActive = true;
                debugWindow.style.display = 'block';
            } else {
                console.error('Element with ID "debug-window" not found.');
            }
        } else {
            if (debugWindow) {
                debugWindow.style.display = 'none';
                debugWindowActive = false;
            }
        }
    } else {
        if (captivePortalDebug) console.log('Not on preferences.html, skipping debug window initialization');
    }


});