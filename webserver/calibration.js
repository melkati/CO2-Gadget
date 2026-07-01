// Calibration page logic for CO2 Gadget

(function () {
    var calPollInFlight = false;

    // Poll /getCalibrationStatus to show real warm-up progress
    function pollCalibrationStatus() {
        if (calPollInFlight) return;  // guard against overlapping polls
        calPollInFlight = true;
        var controller = new AbortController();
        var timeoutId = setTimeout(function () { controller.abort(); }, 5000);
        fetch('/getCalibrationStatus', { signal: controller.signal })
            .then(function (r) { return r.json(); })
            .then(function (s) {
                var noteEl = document.getElementById('pendingCalibrationNote');
                if (s.calibrationInProgress) {
                    var warmupText = 'Warm-up: ' + s.warmupReadings + '/' + s.warmupReadingsRequired +
                        ' readings, target ' + s.calibrationTargetPpm + ' ppm';
                    noteEl.textContent = '⏳ ' + warmupText + ' — calibration in progress...';
                    noteEl.className = 'status-pending';
                    noteEl.style.display = '';
                } else if (s.pendingCalibration) {
                    noteEl.textContent = '⚠ A calibration is pending — the sensor will calibrate on its next measurement cycle.';
                    noteEl.className = 'status-pending';
                    noteEl.style.display = '';
                } else {
                    noteEl.style.display = 'none';
                }
            })
            .catch(function () { /* endpoint may not respond during deep sleep */ })
            .finally(function () { clearTimeout(timeoutId); calPollInFlight = false; });
    }

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
    setInterval(pollCalibrationStatus, 8000);  // poll warm-up status every 8s
    pollCalibrationStatus();                   // immediate first poll

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
            .then(function (r) {
                if (r.ok) {
                    return r.text().then(function (t) {
                        statusEl.textContent = '\u2713 ' + t;
                        statusEl.className = 'status-ok';
                        pollCalibrationStatus();  // immediately show warm-up progress
                    });
                } else {
                    return r.text().then(function (t) {
                        statusEl.textContent = '\u2717 ' + t;
                        statusEl.className = 'status-err';
                    });
                }
            })
            .catch(function (err) {
                statusEl.textContent = '\u2717 Network error: ' + err;
                statusEl.className = 'status-err';
            });
    });
})();
