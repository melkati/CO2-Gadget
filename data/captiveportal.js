// Global variables to store the previous state
var previousData = {
    captivePortalActive: false,
    testCaptivePortal: false,
    captivePortalNoTimeout: false
};

var captivePortalDebug = true;
var captivePortalStatusBarActive = false;
var testCaptivePortal = false;
var captivePortalActive = false;

/**
 * Toggles the visibility of the status bar.
 */
function toggleStatusBar() {
    const captivePortalStatusBar = document.getElementById('captive-portal-status-bar');
    if (captivePortalStatusBar) {
        captivePortalStatusBar.classList.toggle('hidden-captive-portal-status-bar');
        captivePortalStatusBarActive = !captivePortalStatusBar.classList.contains('hidden-captive-portal-status-bar');
        if (captivePortalDebug) {
            console.log('Toggling status bar visibility');
            console.log('Status bar visibility is now:', captivePortalStatusBarActive);
        }
    } else {
        console.error('Element with ID "captive-portal-status-bar" not found.');
    }
}

/**
 * Shows or hides the Captive Portal status bar based on the provided flag.
 * @param {boolean} show - Determines whether to show or hide the status bar.
 */
function showCaptivePortalStatusBar(show) {
    const captivePortalStatusBar = document.getElementById('captive-portal-status-bar');
    if (captivePortalStatusBar) {
        if (captivePortalDebug) console.log('Showing captive portal status bar:', show);
        captivePortalStatusBar.classList.toggle('hidden-captive-portal-status-bar', !show);
    } else {
        console.error('Element with ID "captive-portal-status-bar" not found.');
    }
}

/**
 * Updates the content of the status bar.
 * @param {string} content - The content to display in the status bar.
 */
function updateStatusBar(content) {
    const statusContent = document.getElementById('status-content');
    if (statusContent) {
        statusContent.textContent = content;
        if (captivePortalDebug) console.log('Updating status bar content:', content);
    } else {
        console.error('Element with ID "status-content" not found.');
    }
}

/**
 * Sets the captive portal settings and sends them to the server.
 * @param {number} timeToWait - The time to wait for the captive portal.
 * @param {boolean} isInitialSetup - Whether this is the initial setup.
 */
function setCaptivePortalSettings(timeToWait, isInitialSetup = false) {
    const disableTimeoutCheckbox = document.getElementById('disableTimeoutCheckbox');
    let captivePortalNoTimeout = disableTimeoutCheckbox ? disableTimeoutCheckbox.checked : false;

    if (!disableTimeoutCheckbox && !isInitialSetup) {
        console.error('Element with ID "disableTimeoutCheckbox" not found.');
        return;
    }

    if (captivePortalDebug) {
        console.log('Setting captivePortalNoTimeout to:', captivePortalNoTimeout);
        console.log('disableTimeoutCheckbox checked:', disableTimeoutCheckbox ? disableTimeoutCheckbox.checked : 'N/A');
        console.log('Time to wait:', timeToWait);
    }

    if (disableTimeoutCheckbox && disableTimeoutCheckbox.checked) {
        timeToWait = 0;
        captivePortalNoTimeout = true;
    }

    fetch('/setCaptivePortalSettings', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({
            timeToWaitForCaptivePortal: timeToWait,
            captivePortalActive: previousData.captivePortalActive,
            captivePortalNoTimeout: captivePortalNoTimeout,
            testCaptivePortal: previousData.testCaptivePortal
        })
    })
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok');
            }
            if (captivePortalDebug) {
                console.log('Settings updated successfully:', {
                    timeToWaitForCaptivePortal: timeToWait,
                    testCaptivePortal: previousData.testCaptivePortal,
                    captivePortalActive: previousData.captivePortalActive,
                    captivePortalNoTimeout: captivePortalNoTimeout
                });
            }
        })
        .catch(error => console.error('Error setting captive portal settings:', error));
}

/**
 * Handles the response data from the captive portal status fetch and updates the UI.
 * @param {Object} data - The data received from the server.
 */
function handleCaptivePortalData(data) {
    const changes = {};
    const propertiesToCheck = ['captivePortalActive', 'testCaptivePortal', 'captivePortalNoTimeout'];

    propertiesToCheck.forEach(key => {
        if (data[key] !== previousData[key]) {
            changes[key] = { previous: previousData[key], current: data[key] };
            previousData[key] = data[key];
        }
    });

    if (Object.keys(changes).length > 0 && captivePortalDebug) {
        console.log('Detected changes in captive portal data:', changes);
    }

    if (data.captivePortalDebug) {
        captivePortalDebug = data.captivePortalDebug;
    }

    if (data.captivePortalActive) {
        previousData.captivePortalActive = true;
        showCaptivePortalStatusBar(true);

        const newStatusContent = data.testCaptivePortal ? 'Captive portal active (test mode)' : 'Captive portal active';
        updateStatusBar(newStatusContent);

        const statusContentElement = document.getElementById('status-content');
        if (statusContentElement) {
            if (data.captivePortalNoTimeout) {
                statusContentElement.innerHTML = `
                    <span>Timeout Disabled</span>
                    <label><input type="checkbox" id="disableTimeoutCheckbox" ${data.captivePortalNoTimeout ? 'checked' : ''}> Enable</label>
                `;
            } else {
                statusContentElement.innerHTML = `
                    <span>Timeout: <strong>${data.captivePortalTimeLeft}</strong>s</span>
                    <label><input type="checkbox" id="disableTimeoutCheckbox" ${data.timeToWaitForCaptivePortal === 0 ? 'checked' : ''}> Disable</label>
                `;
            }
        } else {
            console.error('Element with ID "status-content" not found.');
        }

        const disableTimeoutCheckbox = document.getElementById('disableTimeoutCheckbox');
        if (disableTimeoutCheckbox) {
            disableTimeoutCheckbox.addEventListener('change', function () {
                const timeToWait = this.checked ? 0 : data.timeToWaitForCaptivePortal;
                console.log('Setting time to wait:', timeToWait);
                setCaptivePortalSettings(timeToWait);
            });
        }
    } else {
        clearCaptivePortalStatusBar();
    }

    showCaptivePortalStatusBar(previousData.captivePortalActive);
}

/**
 * Clears the captive portal status bar content.
 */
function clearCaptivePortalStatusBar() {
    const captivePortalStatusBar = document.getElementById('captive-portal-status-bar');
    if (captivePortalStatusBar) {
        captivePortalStatusBar.innerHTML = '';
        captivePortalStatusBar.classList.add('hidden-captive-portal-status-bar');
        captivePortalStatusBarActive = false;
    } else {
        console.error('Element with ID "captive-portal-status-bar" not found.');
    }
}

/**
 * Fetches the captive portal settings from the server.
 */
function getCaptivePortalSettings() {
    if (!previousData.captivePortalActive) return;

    fetch('/getCaptivePortalStatusAsJson')
        .then(response => response.json())
        .then(handleCaptivePortalData)
        .catch(error => {
            console.error('Error fetching captive portal status:', error);
            showServerStatusDot(true);
        });
}

/**
 * Shows or hides the server status dot based on the provided flag.
 * @param {boolean} show - Determines whether to show or hide the server status dot.
 */
function showServerStatusDot(show) {
    const serverStatusDot = document.getElementById('server-status-dot');
    if (serverStatusDot) {
        serverStatusDot.classList.toggle('hidden', !show);
    } else {
        console.error('Element with ID "server-status-dot" not found.');
    }
}

/**
 * Checks the server connection periodically.
 */
function checkServerConnection() {
    fetch('/pingServer')
        .then(response => {
            showServerStatusDot(!response.ok);
        })
        .catch(error => {
            console.error('Error pinging server:', error);
            showServerStatusDot(true);
        });
}

// Initial setup based on URL parameters
if (window.location.href.includes("debugCaptivePortal")) {
    captivePortalDebug = true;
    console.log('Forcing captive portal debug mode to be active by parameter in URL');
}

if (window.location.href.includes("testCaptivePortal")) {
    if (captivePortalDebug) console.log('Forcing captive portal to be active in test mode by parameter in URL');
    testCaptivePortal = true;
    captivePortalActive = true;
    previousData.testCaptivePortal = true;
    previousData.captivePortalActive = true;
}

// Fetch initial settings and set intervals to update them periodically
document.addEventListener("DOMContentLoaded", function () {
    if (captivePortalDebug) console.log('Document loaded. Initializing captive portal status bar');

    // Injecting the content dynamically for the status bar
    const captivePortalStatusBar = document.getElementById("captive-portal-status-bar");
    if (captivePortalStatusBar) {
        if (captivePortalDebug) console.log('Injecting captive portal status bar content');
        captivePortalStatusBar.innerHTML = `
            <span id="status-content"></span>
            <button id="hide-captive-portal-status-bar-button" onclick="toggleStatusBar()">Hide</button>
        `;
    } else {
        console.error('Element with ID "captive-portal-status-bar" not found.');
    }

    if (captivePortalActive) {
        if (captivePortalDebug) console.log('Captive portal active. Showing status bar');
        showCaptivePortalStatusBar(true);
        updateStatusBar('Captive portal active (test mode)');

        const statusContentElement = document.getElementById('status-content');
        if (statusContentElement) {
            statusContentElement.innerHTML = `
                <span>Timeout: <strong>60</strong>s</span>
                <label><input type="checkbox" id="disableTimeoutCheckbox"> Disable</label>
            `;
        } else {
            console.error('Element with ID "status-content" not found.');
        }

        const disableTimeoutCheckbox = document.getElementById('disableTimeoutCheckbox');
        if (disableTimeoutCheckbox) {
            disableTimeoutCheckbox.addEventListener('change', function () {
                const timeToWait = this.checked ? 0 : 60;
                console.log('Setting time to wait:', timeToWait);
                setCaptivePortalSettings(timeToWait);
            });
        }

        // Set initial settings on the server
        if (captivePortalDebug) console.log('Setting initial captive portal settings');
        setCaptivePortalSettings(60, true);
    }

    setInterval(getCaptivePortalSettings, 1000);
    setInterval(checkServerConnection, 1000);
});
