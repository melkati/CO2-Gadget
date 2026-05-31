var canPingServer = true;

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
function displayServerStatusDot(show, colorClass = 'status-dot-cyan', title = '') {
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
    fetchWithTimeout('/pingServer', {}, 3000)
        .then(response => {
            canPingServer = response.ok;
            if (!response.ok) captivePortalActive = false;
        })
        .catch(error => {
            console.error('Error pinging server:', error);
            canPingServer = false;
            captivePortalActive = false;
        });
}

/**
 * Updates the server status dot based on the captive portal status.
 */
function updateServerStatusDot() {
    let show, colorClass, title;

    if (!canPingServer) {
        show = true;
        colorClass = 'status-dot-red';
        title = 'Connection to server lost';
    } else if (!captivePortalActive) {
        show = false;
        colorClass = 'status-dot-hidden';
        title = 'Captive portal inactive';
    } else if (forcedCaptivePortal) {
        show = true;
        colorClass = 'status-dot-blue';
        title = 'Captive portal active (test mode)';
    } else {
        show = true;
        colorClass = 'status-dot-cyan';
        title = 'Captive portal active';
    }

    // Call displayServerStatusDot with the new state
    displayServerStatusDot(show, colorClass, title);

    // if (captivePortalDebug) {
    //     console.log('Updated server status dot based on captive portal status');
    //     console.log('captivePortalActive:', captivePortalActive, 'forcedCaptivePortal:', forcedCaptivePortal);
    // }
}

document.addEventListener("DOMContentLoaded", function () {
    // Poll at 5 s — reduces request rate on ESP32 while still detecting connectivity loss promptly
    setInterval(updateServerStatusDot, 5000);
    setInterval(checkServerConnection, 5000);
});