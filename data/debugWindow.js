var captivePortalDebug = false;
var forcedCaptivePortalDebug = false;
var debugWindowActive = false;

/**
 * Updates the debug window with the given data.
 * @param {Object} data - The data to display in the debug window.
 */
function updateDebugWindow(data) {
    if (debugWindowActive) {
        const debugContent = document.getElementById('debug-content');
        if (debugContent) {
            // if (captivePortalDebug) console.log('Updating debug window with data:', data);
            let content = `
            <div>captivePortalActive: ${captivePortalActive}</div>
            <div>captivePortalNoTimeout: ${captivePortalNoTimeout}</div>
            <div>timeToWaitForCaptivePortal: ${data.timeToWaitForCaptivePortal}</div>`;

            if (captivePortalActive && !captivePortalNoTimeout) {
                content += `<div>captivePortalTimeLeft: ${captivePortalStatus.captivePortalTimeLeft}</div> `;
            } else {
                if (captivePortalNoTimeout) {
                    content += `<div>Timeout: <strong>Disabled</strong></div>`;
                }
            }

            if (relaxedSecurity) {
                // if (captivePortalDebug) console.log('Adding relaxedSecurity to debug window:', relaxedSecurity);
                content += `<div>relaxedSecurity: ${relaxedSecurity}</div>`;
            }
            if (forcedCaptivePortal) {
                // if (captivePortalDebug) console.log('Adding forcedCaptivePortal to debug window:', forcedCaptivePortal);
                content += `<div>forcedCaptivePortal: ${forcedCaptivePortal}</div>`;
            }
            if (forcedCaptivePortalDebug) {
                // if (captivePortalDebug) console.log('Adding forcedCaptivePortalDebug to debug window:', forcedCaptivePortalDebug);
                content += `<div>forcedCaptivePortalDebug: ${forcedCaptivePortalDebug}</div>`;
            }
            debugContent.innerHTML = content;
            showDebugWindow(true);
        }
    } else {
        showDebugWindow(false);
    }
}

/**
 * Shows or hides the debug window based on the provided flag.
 * @param {boolean} show - Flag indicating whether to show or hide the debug window.
 */
function showDebugWindow(show) {
    const debugWindow = document.getElementById('debug-window');
    if (debugWindow) {
        const isCurrentlyVisible = !debugWindow.classList.contains('hidden-debug-window') && debugWindow.style.display !== 'none';

        if (show && !isCurrentlyVisible) {
            debugWindow.classList.remove('hidden-debug-window');
            debugWindow.style.display = 'block';
            debugWindowActive = true;
            if (captivePortalDebug) console.log('Showing debug window');
        } else if (!show && isCurrentlyVisible) {
            debugWindow.classList.add('hidden-debug-window');
            debugWindow.style.display = 'none';
            debugWindowActive = false;
            if (captivePortalDebug) console.log('Hiding debug window');
        }
    } else {
        console.error('Element with ID "debug-window" not found.');
    }
    showCaptivePortalSettings(show); // Show captive portal settings when debug window is shown (for development)
}

/**
 * Creates a debug window with a close button and debug content.
 */
function createDebugWindow() {
    const debugWindow = document.getElementById('debug-window');
    const closeButton = document.createElement('button');
    closeButton.id = 'close-debug-window';
    closeButton.innerHTML = '[X]';
    closeButton.onclick = () => {
        showDebugWindow(false);
    };
    debugWindow.appendChild(closeButton);

    const debugContent = document.createElement('div');
    debugContent.id = 'debug-content';
    debugWindow.appendChild(debugContent);
}

function initializeDebugWindowUrlParameters() {
    if (window.location.href.includes("forcedDebugWindow")) {
        if (captivePortalDebug) {
            console.log('Forcing debug window to be active by forcedDebugWindow parameter in URL');
            forcedCaptivePortalDebug = true;
            debugWindowActive = true;
        }
    }
}

document.addEventListener("DOMContentLoaded", function () {
    initializeDebugWindowUrlParameters();
    createDebugWindow();
    // setInterval(updateStatusBar, 1000);
});
