var captivePortalActive = false;
var testCaptivePortal = false;
var captivePortalStatusBarActive = false;
var captivePortalDebug = false;
var relaxedSecurity = false;
var debugWindowActive = false;

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
    if (captivePortalStatusBarActive === show) return;

    const captivePortalStatusBar = document.getElementById('captive-portal-status-bar');
    if (captivePortalStatusBar) {
        if (captivePortalDebug) console.log('Captive portal status bar found:', captivePortalStatusBar);

        if (show) {
            if (captivePortalDebug) console.log('Showing captive portal status bar');
            captivePortalStatusBar.classList.remove('hidden-captive-portal-status-bar');
        } else {
            if (captivePortalDebug) console.log('Hiding captive portal status bar');
            captivePortalStatusBar.classList.add('hidden-captive-portal-status-bar');
        }
        captivePortalStatusBarActive = show;
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
        console.log('Time to wait:', timeToWait);
    }

    if (disableTimeoutCheckbox && disableTimeoutCheckbox.checked) {
        timeToWait = 0;
        captivePortalNoTimeout = true;
    }

    fetch('/setCaptivePortalSettings', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
            timeToWaitForCaptivePortal: timeToWait,
            captivePortalActive,
            captivePortalNoTimeout,
            testCaptivePortal,
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
                    testCaptivePortal,
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
 * Updates the debug window with the given data.
 * @param {Object} data - The data to display in the debug window.
 */
function updateDebugWindow(data) {
    if (debugWindowActive) {
        const debugContent = document.getElementById('debug-content');
        if (debugContent) {
            if (captivePortalDebug) console.log('Updating debug window with data:', data);
            let content = `
            <div>captivePortalActive: ${data.captivePortalActive}</div>
            <div>captivePortalNoTimeout: ${data.captivePortalNoTimeout}</div>
            <div>timeToWaitForCaptivePortal: ${data.timeToWaitForCaptivePortal}</div>
            <div>captivePortalTimeLeft: ${data.captivePortalTimeLeft}</div>
            <div>captivePortalDebug: ${data.captivePortalDebug}</div>
        `;
            if (data.testCaptivePortal !== undefined) {
                if (captivePortalDebug) console.log('Adding testCaptivePortal to debug window:', data.testCaptivePortal);
                content += `<div>testCaptivePortal: ${data.testCaptivePortal}</div>`;
            }
            if (data.relaxedSecurity !== undefined) {
                if (captivePortalDebug) console.log('Adding relaxedSecurity to debug window:', data.relaxedSecurity);
                content += `<div>relaxedSecurity: ${data.relaxedSecurity}</div>`;
            }
            debugContent.innerHTML = content;
            debugWindow.style.display = 'block';
        }
    } else {
        debugWindow.style.display = 'none';
    }
}

/**
 * Handles the response data from the captive portal status fetch and updates the UI.
 * @param {Object} data - The data received from the server.
 */
function handleCaptivePortalData(data) {
    const changes = {};
    const propertiesToCheck = ['captivePortalActive', 'forceCaptivePortalActive', 'captivePortalNoTimeout', 'captivePortalDebug', 'relaxedSecurity'];

    if (captivePortalDebug) console.log('Received captive portal data from server:', data);

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
        debugWindowActive = captivePortalDebug;
        console.log('Captive portal debug mode set to:', captivePortalDebug);
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

    if (debugWindowActive) {
        updateDebugWindow(data);
    }
}

/**
 * Fetches the captive portal settings from the server.
 */
function getCaptivePortalSettings() {
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
 * Change the theme of the page.
 */
function toggleTheme() {
    const htmlElement = document.documentElement;
    const currentTheme = htmlElement.getAttribute('theme');
    const newTheme = currentTheme === 'dark' ? 'light' : 'dark';

    console.debug('Toggle theme function called');
    console.debug('Current theme:', currentTheme);
    console.debug('New theme:', newTheme);

    htmlElement.setAttribute('theme', newTheme);
    console.debug('Theme set to:', newTheme);

    // Save the selected theme to localStorage
    localStorage.setItem('theme', newTheme);
}

/**
 * Adjust the padding-top of the content based on the navbar height.
 */
function adjustContentPadding() {
    const navbar = document.querySelector('.navbar');
    const content = document.querySelector('.content');

    if (navbar && content) {
        const navbarHeight = navbar.offsetHeight;
        content.style.paddingTop = `${navbarHeight}px`;
    }
}

/**
 * Initialize the theme from localStorage.
 */
function initializeTheme() {
    const savedTheme = localStorage.getItem('theme');
    if (savedTheme) {
        document.documentElement.setAttribute('theme', savedTheme);
    }
}

/**
 * Initialize event listeners for the theme switch.
 */
function initializeThemeSwitch() {
    const colorSwitch = document.querySelector("#iconBulb");
    if (colorSwitch) {
        colorSwitch.addEventListener('click', toggleTheme);
    } else {
        console.error('Element with ID "iconBulb" not found.');
    }
}

/**
 * Initialize the captive portal for preferences.html.
 */
function initializeCaptivePortal() {
    if (captivePortalDebug) console.log('Document loaded. Initializing captive portal for preferences.html');

    getCaptivePortalSettings(); // Fetch initial settings
    setupInitialSettings();
    initializeCaptivePortalStatusBar();
    updateStatusBarContent();

    setInterval(getCaptivePortalSettings, 1000);
    setInterval(checkServerConnection, 1000);

    highlightCurrentPage(); // Highlight the current page in the navigation
}

/**
 * Create the debug window and its content.
 * @param {HTMLElement} debugWindow - The debug window element.
 */
function createDebugWindow(debugWindow) {
    const closeButton = document.createElement('button');
    closeButton.id = 'close-debug-window';
    closeButton.innerHTML = '[X]';
    closeButton.onclick = () => {
        debugWindow.style.display = 'none';
    };
    debugWindow.appendChild(closeButton);

    const debugContent = document.createElement('div');
    debugContent.id = 'debug-content';
    debugWindow.appendChild(debugContent);
}

document.addEventListener("DOMContentLoaded", function () {
    const currentPage = window.location.pathname.split("/").pop();

    adjustContentPadding();
    initializeTheme();
    initializeThemeSwitch();

    if (currentPage === "preferences.html") {
        initializeCaptivePortal();
    } else {
        if (captivePortalDebug) console.log('Not on preferences.html, skipping debug window initialization');
    }

    if (captivePortalDebug) {
        const debugWindow = document.getElementById('debug-window');
        if (debugWindow) {
            createDebugWindow(debugWindow);
            debugWindowActive = true;
            debugWindow.style.display = 'block';
        } else {
            console.error('Element with ID "debug-window" not found.');
        }
    } else {
        debugWindow.style.display = 'none';
        debugWindowActive = true;
    }
});
