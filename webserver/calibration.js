// Calibration page logic for CO2 Gadget

(function () {
    // Live readings
    function updateReadings() {
        fetch('/getPreferences')
            .then(r => r.json())
            .then(d => {
                if (d.co2 !== undefined)          document.getElementById('liveCO2').textContent  = d.co2;
                if (d.temperature !== undefined)  document.getElementById('liveTemp').textContent = parseFloat(d.temperature).toFixed(1);
                if (d.humidity !== undefined)     document.getElementById('liveHum').textContent  = parseFloat(d.humidity).toFixed(1);
                const note = document.getElementById('pendingCalibrationNote');
                if (d.pendingCalibration) {
                    note.style.display = '';
                } else {
                    note.style.display = 'none';
                }
            })
            .catch(() => {});
    }

    updateReadings();
    setInterval(updateReadings, 5000);

    // Calibrate button
    document.getElementById('btnCalibrate').addEventListener('click', function () {
        const val = parseInt(document.getElementById('calibValue').value, 10);
        const statusEl = document.getElementById('calibStatus');
        if (isNaN(val) || val < 400 || val > 2000) {
            statusEl.textContent = 'Error: value must be between 400 and 2000 ppm.';
            statusEl.className = 'status-err';
            return;
        }
        statusEl.textContent = 'Sending calibration command\u2026';
        statusEl.className = '';
        fetch('/settings?CalibrateCO2=' + val)
            .then(r => {
                if (r.ok) {
                    return r.text().then(t => {
                        statusEl.textContent = '\u2713 ' + t;
                        statusEl.className = 'status-ok';
                        document.getElementById('pendingCalibrationNote').style.display = '';
                    });
                } else {
                    return r.text().then(t => {
                        statusEl.textContent = '\u2717 ' + t;
                        statusEl.className = 'status-err';
                    });
                }
            })
            .catch(err => {
                statusEl.textContent = '\u2717 Network error: ' + err;
                statusEl.className = 'status-err';
            });
    });
})();
