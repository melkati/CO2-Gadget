// Calibration page logic for CO2 Gadget

(function () {
    // Live readings — use dedicated sensor endpoints (same as index page)
    function updateReadings() {
        Promise.all([
            readCO2Data().catch(() => null),
            readTemperatureData().catch(() => null),
            readHumidityData().catch(() => null)
        ]).then(([co2, temp, hum]) => {
            if (co2  !== null) document.getElementById('liveCO2').textContent  = Math.round(co2);
            if (temp !== null) document.getElementById('liveTemp').textContent = temp;
            if (hum  !== null) document.getElementById('liveHum').textContent  = hum;
        });
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
