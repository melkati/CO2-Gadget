var captivePortalStatusBarActive = false;
var forcedCaptivePortalStatusBar = false;
var captivePortalStatusBarDebug = true;


/**
 * Updates the status bar content and sets up the event listener for the disable timeout checkbox.
 */
function updateStatusBarContent() {
    if (captivePortalActive) {
        // if (captivePortalDebug) console.log('Captive portal active. Showing status bar');
        // updateStatusBar('Captive portal active (test mode)');

        const statusBarContentElement = document.getElementById('status-bar-content');
        // Timeout time: captivePortalStatus.captivePortalTimeLeft
        if (statusBarContentElement) {
            if (!captivePortalNoTimeout) {
                // set id="disableTimeoutCheckbox" to unchecked
                statusBarContentElement.innerHTML = `
                <span>Timeout: <strong>${captivePortalStatus.captivePortalTimeLeft}</strong> seconds</span>
                <label><input type="checkbox" id="disableTimeoutCheckbox"> Disable</label>
            `;
            } else {
                // set id="disableTimeoutCheckbox" to checked
                statusBarContentElement.innerHTML = `
                <span>Timeout: <strong>Disabled</strong></span>
                <label><input type="checkbox" id="disableTimeoutCheckbox" checked> Disable</label>
            `;
            }
        } else {
            console.error('Element with ID "status-bar-content" not found.');
        }

        const disableTimeoutCheckbox = document.getElementById('disableTimeoutCheckbox');
        if (disableTimeoutCheckbox) {
            disableTimeoutCheckbox.checked = captivePortalNoTimeout;
            disableTimeoutCheckbox.addEventListener('change', function () {
                const timeToWait = this.checked ? 0 : 60;
                captivePortalNoTimeout = this.checked;
                console.log('Setting time to wait:', timeToWait);
                console.log('Disabling timeout:', captivePortalNoTimeout);
                setCaptivePortalSettings(timeToWait);
            });
        }

        // if (captivePortalDebug) console.log('Setting initial captive portal settings');
        // setCaptivePortalSettings(60, true);
    } else {
        updateStatusBar('Captive portal inactive');
        showCaptivePortalStatusBar(false);
    }
}

/**
 * Shows or hides the captive portal status bar.
 * @param {boolean} show - Determines whether to show or hide the status bar.
 */
function showCaptivePortalStatusBar(show) {
    if (captivePortalStatusBarActive === show) return;

    const captivePortalStatusBar = document.getElementById('captive-portal-status-bar');
    if (captivePortalStatusBar) {
        if (captivePortalStatusBarDebug) console.log('Captive portal status bar found:', captivePortalStatusBar);

        if (show) {
            if (captivePortalStatusBarDebug) console.log('Showing captive portal status bar');
            captivePortalStatusBar.classList.remove('hidden-captive-portal-status-bar');
        } else {
            if (captivePortalStatusBarDebug) console.log('Hiding captive portal status bar');
            captivePortalStatusBar.classList.add('hidden-captive-portal-status-bar');
        }
        captivePortalStatusBarActive = show;
    } else {
        console.error('Element with ID "captive-portal-status-bar" not found.');
    }
}

/**
 * Initializes the Captive Portal Status Bar.
 */
function initializeCaptivePortalStatusBar() {
    if (captivePortalStatusBarDebug) console.log('Initializing captive portal status bar');

    const captivePortalStatusBar = document.getElementById('captive-portal-status-bar');
    if (captivePortalStatusBar) {
        if (captivePortalStatusBarDebug) console.log('Injecting captive portal status bar content');
        captivePortalStatusBar.innerHTML = `
            <span id="status-bar-content"></span>
            <span id="hide-captive-portal-status-bar-button" onclick="showCaptivePortalStatusBar(false)">
                <svg width="24" height="24" viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg">
                    <circle cx="12" cy="12" r="10" stroke="black" stroke-width="2"/>
                    <line x1="8" y1="8" x2="16" y2="16" stroke="black" stroke-width="2"/>
                    <line x1="16" y1="8" x2="8" y2="16" stroke="black" stroke-width="2"/>
                </svg>
            </span>
        `;
        if (forcedCaptivePortal) showCaptivePortalStatusBar(true);
    } else {
        console.error('Element with ID "captive-portal-status-bar" not found.');
    }
}

function initializeCaptivePortalStatusBarUrlParameters() {
    captivePortalStatusBarDebug = window.location.href.includes("captivePortalStatusBarDebug");
    if (captivePortalStatusBarDebug) console.log('Captive portal status bar debug:', captivePortalStatusBarDebug);
    forcedCaptivePortalStatusBar = window.location.href.includes("forcedCaptivePortalStatusBar");
    if (captivePortalStatusBarDebug) console.log('Captive portal status bar forced:', forcedCaptivePortalStatusBar);
}

/**
 * Updates the content of the status bar.
 * @param {string} content - The content to display in the status bar.
 */
function updateStatusBar(content) {
    if (!captivePortalStatusBarActive) return;
    if (captivePortalStatusBarDebug) console.log('Updating status bar');
    const statusContent = document.getElementById('status-bar-content');
    if (statusContent) {
        statusContent.textContent = content;
        if (captivePortalStatusBarDebug) console.log('Updating status bar content:', content);
    } else {
        console.error('Element with ID "status-bar-content" not found.');
    }
}

document.addEventListener("DOMContentLoaded", function () {
    initializeCaptivePortalStatusBarUrlParameters();
    initializeCaptivePortalStatusBar();
    // setInterval(updateStatusBar, 1000);
    setInterval(updateStatusBarContent, 1000);
});