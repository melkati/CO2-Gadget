// Function to toggle the visibility of the status bar
function toggleStatusBar() {
    const statusBar = document.getElementById('captive-portal-status');
    statusBar.classList.toggle('hidden');
}

// Function to update the content of the status bar
function updateStatusBar(content) {
    const statusContent = document.getElementById('status-content');
    statusContent.textContent = content;
}

// Example usage: update the content of the status bar
// Call this function when there are changes in the captive portal status
updateStatusBar('Captive Portal Active - Connected to WiFi network');

// Function to update the navigation bar with captive portal status
function updateCaptivePortalStatus() {
    fetch('/getCaptivePortalStatusAsJson')
        .then(response => response.json())
        .then(data => {
            const statusContainer = document.getElementById('captivePortalStatus');
            if (data.captivePortalActive) {
                if (data.captivePortalNoTimeout) {
                    statusContainer.innerHTML = `
                    <span>Timeout Disabled</span>
                    <label><input type="checkbox" id="disableTimeoutCheckbox" ${data.timeToWaitForCaptivePortal === 0 ? 'checked' : ''}> Never</label>
                    `;
                } else {
                    // Display the countdown and checkbox
                    statusContainer.innerHTML = `
                    <span>Timeout: <strong>${data.captivePortalTimeLeft}</strong>s</span>
                    <label><input type="checkbox" id="disableTimeoutCheckbox" ${data.timeToWaitForCaptivePortal === 0 ? 'checked' : ''}> Never</label>
                `;
                    // Add event listener to the checkbox
                    document.getElementById('disableTimeoutCheckbox').addEventListener('change', function () {
                        const timeToWait = this.checked ? 0 : data.timeToWaitForCaptivePortal;
                        fetch('/disableCaptivePortalTimeout', {
                            method: 'POST',
                            headers: {
                                'Content-Type': 'application/json',
                            },
                            body: JSON.stringify({
                                timeToWaitForCaptivePortal: timeToWait
                            })
                        });
                    });
                }
            } else {
                // Clear the status if the portal is not active
                statusContainer.innerHTML = '';
            }
        })
        .catch(error => console.error('Error fetching captive portal status:', error));
}

// Call the function on page load and every second to update the countdown
updateCaptivePortalStatus();
setInterval(updateCaptivePortalStatus, 1000);
