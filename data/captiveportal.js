var captivePortalDebug = true;
var captivePortalStatusBarActive = false;
var testCaptivePortal = false;
var captivePortalActive = false;

// Global variables to store the previous state
var previousData = {
    captivePortalActive: false,
    testCaptivePortal: false,
    captivePortalNoTimeout: false
};

/**
 * Shows or hides the captive portal status bar.
 * @param {boolean} show - Determines whether to show or hide the status bar.
 */
function showCaptivePortalStatusBar(show) {
    if (!captivePortalStatusBarActive === show) {
        const captivePortalStatusBar = document.getElementById('captive-portal-status-bar');
        if (captivePortalStatusBar) {
            if (captivePortalDebug) console.log('Captive portal status bar found:', captivePortalStatusBar);
            if (show && !captivePortalStatusBarActive) {
                if (captivePortalDebug) console.log('Showing captive portal status bar');
                captivePortalStatusBar.classList.remove('hidden-captive-portal-status-bar');
                captivePortalStatusBarActive = true;
                return 'Captive portal status bar shown';
            }

            if (!show && captivePortalStatusBarActive) {
                if (captivePortalDebug) console.log('Hiding captive portal status bar');
                captivePortalStatusBar.classList.add('hidden-captive-portal-status-bar');
                captivePortalStatusBarActive = false;
                return 'Captive portal status bar hidden';
            }

            return 'Captive portal status bar visibility unchanged';
        } else {
            console.error('Element with ID "captive-portal-status-bar" not found.');
            return 'Error: Element not found';
        }
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
        // console.log('disableTimeoutCheckbox checked:', disableTimeoutCheckbox ? disableTimeoutCheckbox.checked : 'N/A');
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
            captivePortalActive: captivePortalActive,
            captivePortalNoTimeout: captivePortalNoTimeout,
            testCaptivePortal: testCaptivePortal,
            captivePortalDebug: captivePortalDebug
        })
    })
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok');
            }
            if (captivePortalDebug) {
                // console.log('Response from server:', response);
                console.log('Settings updated successfully:', {
                    testCaptivePortal: testCaptivePortal,
                    captivePortalActive: captivePortalActive,
                    captivePortalNoTimeout: captivePortalNoTimeout,
                    timeToWaitForCaptivePortal: timeToWait,
                    captivePortalDebug: captivePortalDebug
                });
            }
        })
        .catch(error => console.error('Error setting captive portal settings:', error));
}

/**
 * Updates the debug window with the given data.
 * @param {Object} data - The data to display in the debug window.
 */
function updateDebugWindow(data) {
    const debugWindow = document.getElementById('debug-window');
    if (debugWindow) {
        debugWindow.innerHTML = `
            <div>captivePortalActive: ${data.captivePortalActive}</div>
            <div>testCaptivePortal: ${data.testCaptivePortal}</div>
            <div>captivePortalNoTimeout: ${data.captivePortalNoTimeout}</div>
            <div>timeToWaitForCaptivePortal: ${data.timeToWaitForCaptivePortal}</div>
            <div>captivePortalTimeLeft: ${data.captivePortalTimeLeft}</div>
            <div>captivePortalDebug: ${data.captivePortalDebug}</div>
        `;
    }
}

/**
 * Shows or hides the debug window based on the provided flag.
 * @param {boolean} show - Determines whether to show or hide the debug window.
 */
function showDebugWindow(show) {
    const debugWindow = document.getElementById('debug-window');
    if (debugWindow) {
        if (show) {
            debugWindow.classList.remove('hidden-debug-window');
        } else {
            debugWindow.classList.add('hidden-debug-window');
        }
    } else {
        console.error('Element with ID "debug-window" not found.');
    }
}


/**
 * Handles the response data from the captive portal status fetch and updates the UI.
 * @param {Object} data - The data received from the server.
 */
function handleCaptivePortalData(data) {
    const changes = {};
    const propertiesToCheck = ['captivePortalActive', 'forceCaptivePortalActive', 'captivePortalNoTimeout'];

    if (captivePortalDebug) console.log('Received captive portal data:', data);

    // Check for changes in properties
    propertiesToCheck.forEach(key => {
        if (data[key] !== previousData[key]) {
            changes[key] = { previous: previousData[key], current: data[key] };
            previousData[key] = data[key];
        }
    });

    if (Object.keys(changes).length > 0 && captivePortalDebug) {
        console.log('Detected changes in captive portal data:', changes);
    }

    // Update debug mode if present in data
    if (data.captivePortalDebug !== undefined) {
        captivePortalDebug = data.captivePortalDebug;
    }

    // Update active states
    forceCaptivePortalActive = data.forceCaptivePortalActive || false;
    captivePortalActive = (data.captivePortalActive || false) || forceCaptivePortalActive;

    if (captivePortalActive) {
        showCaptivePortalStatusBar(true);

        const newStatusContent = forceCaptivePortalActive ? 'Captive portal active (test mode)' : 'Captive portal active';
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
        showCaptivePortalStatusBar(false);
    }

    updateServerStatusDot(); // Ensure the status dot is updated after handling captive portal data

    // Update the debug window if debug mode is active
    if (captivePortalDebug) {
        updateDebugWindow(data);
        showDebugWindow(true);
    } else {
        showDebugWindow(false);
    }
}

/**
 * Handles the response data from the captive portal status fetch and updates the UI.
 * @param {Object} data - The data received from the server.
 */
function handleCaptivePortalDataOld(data) {
    const changes = {};
    const propertiesToCheck = ['captivePortalActive', 'testCaptivePortal', 'captivePortalNoTimeout'];

    if (captivePortalDebug) console.log('Received captive portal data:', data);

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

    if (data.testCaptivePortal) {
        testCaptivePortal = true;
        data.captivePortalActive = true;
    } else {
        testCaptivePortal = false;
    }

    if (data.captivePortalActive) {
        captivePortalActive = true;
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
        showCaptivePortalStatusBar(false);
    }

    updateServerStatusDot(); // Ensure the status dot is updated after handling captive portal data

    // Update the debug window if debug mode is active
    if (captivePortalDebug) {
        updateDebugWindow(data);
        showDebugWindow(true);
    } else {
        showDebugWindow(false);
    }
}

/**
 * Fetches the captive portal settings from the server.
 */
function getCaptivePortalSettings() {
    // if (!captivePortalStatusBarActive) return;
    fetch('/getCaptivePortalStatusAsJson')
        .then(response => response.json())
        .then(captivePortalSettings => {
            handleCaptivePortalData(captivePortalSettings);
        })
        .catch(error => {
            console.error('Error fetching captive portal status:', error);
            showServerStatusDot(true, 'status-dot-red', 'Connection to server lost');
        });
}

let previousServerStatusDotState = {
    show: null,
    colorClass: '',
    title: ''
};

/**
 * Shows or hides the server status dot based on the provided flag.
 * @param {boolean} show - Determines whether to show or hide the server status dot.
 * @param {string} [colorClass] - The color class to apply.
 * @param {string} [title] - The title to apply for hover text.
 */
function showServerStatusDot(show, colorClass = 'status-dot-white', title = '') {
    const serverStatusDot = document.getElementById('server-status-dot');
    if (serverStatusDot) {
        const hasStateChanged =
            previousServerStatusDotState.show !== show ||
            previousServerStatusDotState.colorClass !== colorClass ||
            previousServerStatusDotState.title !== title;

        if (hasStateChanged) {
            serverStatusDot.className = 'status-dot'; // Reset class
            serverStatusDot.classList.add(show ? colorClass : 'status-dot-hidden');
            serverStatusDot.title = title;

            if (captivePortalDebug) {
                console.log(`Server status dot updated: show=${show}, colorClass=${colorClass}, title=${title}`);
            }

            // Update previous state
            previousServerStatusDotState.show = show;
            previousServerStatusDotState.colorClass = colorClass;
            previousServerStatusDotState.title = title;
        }
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
            if (response.ok) {
                updateServerStatusDot(); // Update status dot based on current state
            } else {
                captivePortalActive = false; // Mark captive portal as inactive
                updateServerStatusDot(); // Update status dot to reflect the lost connection
            }
        })
        .catch(error => {
            console.error('Error pinging server:', error);
            captivePortalActive = false; // Mark captive portal as inactive
            updateServerStatusDot(); // Update status dot to reflect the lost connection
        });
}

/**
 * Updates the server status dot based on the captive portal status.
 */
function updateServerStatusDot() {
    let show, colorClass, title;

    if (!captivePortalActive) {
        show = false;
        colorClass = 'status-dot-hidden';
        title = 'Captive portal inactive';
    } else if (forceCaptivePortalActive) {
        show = true;
        colorClass = 'status-dot-blue';
        title = 'Captive portal active (test mode)';
    } else {
        show = true;
        colorClass = 'status-dot-white';
        title = 'Captive portal active';
    }

    // Call showServerStatusDot with the new state
    showServerStatusDot(show, colorClass, title);

    if (captivePortalDebug) {
        console.log('Updated server status dot based on captive portal status');
        console.log('captivePortalActive:', captivePortalActive, 'forceCaptivePortalActive:', forceCaptivePortalActive);
    }
}

/**
 * Initializes the Captive Portal Status Bar.
 */
function initializeCaptivePortalStatusBar() {
    if (captivePortalDebug) console.log('Initializing captive portal status bar');

    const captivePortalStatusBar = document.getElementById("captive-portal-status-bar");
    if (captivePortalStatusBar) {
        if (captivePortalDebug) console.log('Injecting captive portal status bar content');
        captivePortalStatusBar.innerHTML = `
            <span id="status-content"></span>
            <span id="hide-captive-portal-status-bar-button" onclick="showCaptivePortalStatusBar(false)">
                <svg width="24" height="24" viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg">
                    <circle cx="12" cy="12" r="10" stroke="black" stroke-width="2"/>
                    <line x1="8" y1="8" x2="16" y2="16" stroke="black" stroke-width="2"/>
                    <line x1="16" y1="8" x2="8" y2="16" stroke="black" stroke-width="2"/>
                </svg>
            </span>
        `;
        if (testCaptivePortal) showCaptivePortalStatusBar(true);
    } else {
        console.error('Element with ID "captive-portal-status-bar" not found.');
    }
}

/**
 * Sets up the initial settings based on URL parameters.
 */
function setupInitialSettings() {
    if (window.location.href.includes("debugCaptivePortal")) {
        captivePortalDebug = true;
        console.log('Forcing captive portal debug mode to be active by parameter in URL');
    }

    if (window.location.href.includes("testCaptivePortal")) {
        if (captivePortalDebug) console.log('Forcing captive portal to be active in test mode by parameter in URL');
        testCaptivePortal = true;
        captivePortalActive = true;
    }

    setCaptivePortalSettings(60, true);
}

/**
 * Updates the status bar content and sets up the event listener for the disable timeout checkbox.
 */
function updateStatusBarContent() {
    if (captivePortalActive) {
        if (captivePortalDebug) console.log('Captive portal active. Showing status bar');
        // showCaptivePortalStatusBar(true);
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
}

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

/**
 * Initializes after the DOM content has loaded.
 */
document.addEventListener("DOMContentLoaded", function () {
    if (captivePortalDebug) console.log('Document loaded. Initializing captive portal');
    getCaptivePortalSettings(); // Fetch initial settings

    setupInitialSettings();
    initializeCaptivePortalStatusBar();
    updateStatusBarContent();

    setInterval(getCaptivePortalSettings, 1000);
    setInterval(checkServerConnection, 1000);

    highlightCurrentPage(); // Highlight the current page in the navigation
});
