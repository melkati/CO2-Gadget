#!/usr/bin/env python3
"""
CO2 Gadget Web Emulator
=======================
Serves the webserver/ HTML/JS/CSS files locally and mocks all ESP32 REST
API endpoints so the UI works without real hardware.

A floating emulator panel is injected into every HTML page so you can
toggle compiled features and adjust mock sensor values in-page.

Usage:
    pip install flask
    python server.py [--port 8080]
"""
import argparse
import os
import random
import re
import time

from flask import Flask, Response, jsonify, request, send_from_directory

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
WEBSERVER_DIR = os.path.normpath(os.path.join(BASE_DIR, "..", "webserver"))
PLATFORMIO_INI = os.path.normpath(os.path.join(BASE_DIR, "..", "platformio.ini"))

# Files that must be prepended to every page-specific script.
# Mirrors the build system that concatenates these into each *.js.gz bundle.
_JS_COMMON_FILES = ["common.js", "themes.js", "debugWindow.js"]

# Map of request filename → source filename in webserver/ (for aliased names)
_JS_ALIAS = {"index.v2.js": "index.js"}

app = Flask(__name__)

# ── platformio.ini feature parser ─────────────────────────────────────────────

_DEFINE_TO_FEATURE = {
    "SUPPORT_BLE":             "BLE",
    "SUPPORT_BTHOME_BLE":      "BTHomeBLE",
    "SUPPORT_BUZZER":          "Buzzer",
    "SUPPORT_ESPNOW":          "EspNow",
    "SUPPORT_MDNS":            "mDNS",
    "SUPPORT_MQTT":            "MQTT",
    "SUPPORT_MQTT_DISCOVERY":  "MQTTDiscovery",
    "SUPPORT_OTA":             "OTA",
    "SUPPORT_LOW_POWER":       "LowPower",
    "SUPPORT_CIRCULAR_BUFFER": "CircularBuffer",
}

_DEFAULT_FEATURES: dict = {v: False for v in _DEFINE_TO_FEATURE.values()}


def _parse_platformio_features(ini_path: str) -> dict:
    """Return {featureKey: bool} derived from the [features] section of platformio.ini."""
    result = dict(_DEFAULT_FEATURES)
    try:
        with open(ini_path, encoding="utf-8-sig") as f:
            lines = f.readlines()
    except OSError as e:
        print(f"[EMU] Warning: cannot read {ini_path}: {e}")
        return result

    in_features = False
    for raw in lines:
        line = raw.strip()
        if line.startswith("["):
            in_features = line.startswith("[features]")
            continue
        if not in_features:
            continue
        is_commented = line.startswith(";")
        parts = re.sub(r"^[;\s]+", "", line).split()
        if not parts:
            continue
        token = parts[0]
        m = re.match(r"-DSUPPORT_(\w+)", token)
        if not m:
            continue
        define = "SUPPORT_" + m.group(1)
        feat_key = _DEFINE_TO_FEATURE.get(define)
        if feat_key is not None:
            result[feat_key] = not is_commented
    return result


# ── Mutable emulator state ────────────────────────────────────────────────────

features: dict = _parse_platformio_features(PLATFORMIO_INI)

sensors: dict = {
    "co2":             850,
    "temperature":     22.5,
    "humidity":        55.0,
    "batteryVoltage":  3.85,
}

prefs: dict = {
    "supportBTHomeBLE":     True,
    "relaxedSecurity":      True,
    "activeWIFI":           True,
    "activeBLE":            True,
    "activeBTHome":         True,
    "bthomeEncryption":     False,
    "bthomeBindKey":        "00112233445566778899aabbccddeeff",
    "activeMQTT":           False,
    "activeESPNOW":         False,
    "wifiSSID":             "MyNetwork",
    "wifiPass":             "password123",
    "hostName":             "co2-gadget",
    "useStaticIP":          False,
    "staticIP":             "",
    "gateway":              "",
    "subnet":               "",
    "dns1":                 "",
    "dns2":                 "",
    "mqttClientId":         "CO2Gadget",
    "mqttShowInCon":        False,
    "rootTopic":            "CO2_Gadget",
    "mqttBroker":           "192.168.1.1",
    "mqttUser":             "",
    "mqttPass":             "",
    "tToPubMQTT":           30,
    "tKeepAlMQTT":          60,
    "channelESPNow":        1,
    "boardIdESPNow":        0,
    "peerESPNowAddress":    "FF:FF:FF:FF:FF:FF",
    "tToPubESPNow":         30,
    "tKeepAlESPNow":        60,
    "selCO2Sensor":         0,
    "autoSelfCal":          False,
    "customCalValue":       415,
    "co2OrangeRange":       800,
    "co2RedRange":          1000,
    "tempOffset":           0.0,
    "showFahrenheit":       False,
    "altitudeMeters":       0,
    "measurementInterval":  10,
    "debugSensors":         False,
    "neopixBright":         50,
    "selNeopxType":         0,
    "outModeRelay":         False,
    "toneBzrBeep":          1000,
    "durBzrBeep":           150,
    "timeBtwnBzr":          -1,
    "hasBattery":           True,
    "batDischgd":           3000,
    "batChargd":            4200,
    "vRef":                 3300,
    "showTemp":             True,
    "showHumidity":         True,
    "showBattery":          True,
    "showStatusIcons":      True,
    "wakeOnCO2Alert":       False,
    "showCO2":              True,
    # low power
    "lowPowerMode":         0,
    "waitToDeep":           60,
    "timeSleeping":         300,
    "cyclsWifiConn":        1,
    "cycRedrawDis":         0,
    "actBLEOnWake":         True,
    "actWifiOnWake":        True,
    "actMQTTOnWake":        False,
    "actESPnowWake":        False,
    "displayOnWake":        True,
    "tToDispOff":           60,
    "dispOffOnExP":         False,
    "displayReverse":       False,
    "DisplayBright":        128,
    "cpNoTimeout":          False,
    "cpRelaxedSec":         False,
    "cpDebug":              False,
    "cpWaitTime":           60,
}

def _default_threshold() -> dict:
    return {
        "enabled": False,
        "thrKeepAlive": 60,
        "thrOnlyInLowP": False,
        "thrCo2Abs": 50,
        "thrCo2Per": 10,
        "thrTempAbs": 1.0,
        "thrTempPer": 5,
        "thrHumAbs": 5,
        "thrHumPer": 10,
        "thrCo2CombAnd": False,
        "thrTempCombAnd": False,
        "thrHumCombAnd": False,
    }

# 4 threshold slots: Display, Bluetooth, MQTT, ESPNOW  (index 0-3)
thresholds: list = [_default_threshold() for _ in range(4)]

_start_time = time.time()

version_info: dict = {
    "firmVerMajor":  0,
    "firmVerMinor":  14,
    "firmRevision":  "015-modernization-v2",
    "firmBranch":    "modernization/v2",
    "firmFlavour":   "esp32dev",
    "firmBuildDate": "2026-06-12",
    "firmBuildTime": "00:00:00",
}

# ── Floating panel injected into every HTML page ──────────────────────────────
#
# All IDs/classes are prefixed __emu_ to avoid colliding with page styles.
#
# Feature toggles:
#   1. POST /emu/features  → update server state
#   2. Patch window.features + window.supportBTHomeBLE
#   3. Call applyFeatureVisibility() / initNavBar() / fillFeaturesFromServer()
#
# Sensor sliders:
#   - window.fetch is intercepted so /readCO2, /readTemperature, /readHumidity,
#     /readBatteryVoltage return from _emuSensors immediately — no poll-wait.
#   - After every slider move the page's own refresh functions are called so
#     values update at once, not at the next 5-15 s interval.
#   - Values are also POSTed to /emu/sensors (via the original fetch, not the
#     intercepted one) for session persistence across page navigations.

_EMU_INJECT = r"""
<!-- ═══════════ CO2 Gadget Emulator Panel (injected by webEmu/server.py) ═══════════ -->
<style>
#__emu_fab{position:fixed;bottom:18px;right:18px;z-index:99999;
  background:#e94560;color:#fff;border:none;border-radius:24px;
  padding:7px 14px;font-size:13px;font-weight:700;cursor:pointer;
  box-shadow:0 3px 12px rgba(0,0,0,.4);letter-spacing:.02em;
  display:flex;align-items:center;gap:6px;transition:background .15s;}
#__emu_fab:hover{background:#c73652;}
#__emu_fab svg{width:14px;height:14px;fill:currentColor;flex-shrink:0;}

#__emu_panel{position:fixed;bottom:58px;right:18px;z-index:99998;
  width:300px;max-height:calc(100vh - 80px);overflow-y:auto;
  background:#1e1e2e;border:1px solid #3b3b5c;border-radius:10px;
  box-shadow:0 6px 28px rgba(0,0,0,.55);font-family:system-ui,sans-serif;
  font-size:13px;color:#e0e0f0;display:none;}
#__emu_panel.open{display:block;}

.__emu_hdr{background:#16162a;padding:10px 14px;border-radius:10px 10px 0 0;
  display:flex;align-items:center;justify-content:space-between;
  border-bottom:1px solid #3b3b5c;}
.__emu_hdr span{font-weight:700;color:#e94560;font-size:13px;}
.__emu_hdr small{color:#888;font-size:11px;margin-left:6px;}

.__emu_sec{padding:10px 14px;}
.__emu_sec+.__emu_sec{border-top:1px solid #2a2a44;}
.__emu_sec_title{font-size:10px;font-weight:700;letter-spacing:.08em;
  text-transform:uppercase;color:#888;margin-bottom:8px;}

/* feature toggles */
.__emu_feat_row{display:flex;align-items:center;gap:8px;padding:3px 0;cursor:pointer;}
.__emu_feat_row:hover .__emu_label{color:#fff;}
.__emu_label{flex:1;font-size:12px;color:#ccc;cursor:pointer;user-select:none;}
.__emu_label.off{color:#666;text-decoration:line-through;}
.__emu_sw{position:relative;width:32px;height:17px;flex-shrink:0;}
.__emu_sw input{opacity:0;width:0;height:0;position:absolute;}
.__emu_sw_track{position:absolute;inset:0;border-radius:9px;
  background:#444;transition:background .2s;cursor:pointer;}
.__emu_sw_track::before{content:"";position:absolute;
  width:11px;height:11px;left:3px;bottom:3px;border-radius:50%;
  background:#fff;transition:transform .2s;}
.__emu_sw input:checked+.__emu_sw_track{background:#4caf50;}
.__emu_sw input:checked+.__emu_sw_track::before{transform:translateX(15px);}

/* sensor sliders */
.__emu_sensor_row{margin-bottom:8px;}
.__emu_sensor_lbl{display:flex;justify-content:space-between;
  align-items:center;margin-bottom:3px;}
.__emu_sensor_lbl span{font-size:11px;color:#999;}
.__emu_sensor_lbl input[type=number]{width:62px;background:#2a2a44;
  border:1px solid #3b3b5c;color:#e0e0f0;border-radius:4px;
  padding:2px 5px;font-size:12px;text-align:right;}
.__emu_sensor_row input[type=range]{width:100%;accent-color:#e94560;
  height:4px;cursor:pointer;}

/* status bar */
.__emu_status{padding:6px 14px;font-size:10px;color:#666;
  border-top:1px solid #2a2a44;text-align:center;min-height:22px;}
.__emu_status .ok{color:#4caf50;}
.__emu_status .err{color:#e94560;}
</style>

<button id="__emu_fab" title="Toggle Emulator Panel">
  <svg viewBox="0 0 512 512"><path d="M495.9 166.6c3.2 8.7.5 18.4-6.4 24.6l-43.3 39.4c1.1 8.3 1.7 16.8 1.7 25.4s-.6 17.1-1.7 25.4l43.3 39.4c6.9 6.2 9.6 15.9 6.4 24.6-4.4 11.9-9.7 23.3-15.8 34.3l-4.7 8.1c-6.6 11-14 21.4-22.1 31.2-5.9 7.2-15.7 9.6-24.5 6.8l-55.7-17.7c-13.4 10.3-28.2 18.9-44 25.4l-12.5 57.1c-2 9.1-9 16.3-18.2 17.8C284.7 510.8 270.5 512 256 512s-28.7-1.2-42.5-3.5c-9.2-1.5-16.2-8.7-18.2-17.8l-12.5-57.1c-15.8-6.5-30.6-15.1-44-25.4L83.1 425.9c-8.8 2.8-18.6.3-24.5-6.8-8.1-9.8-15.5-20.2-22.1-31.2l-4.7-8.1C25.7 368.9 20.4 357.5 16 345.6c-3.2-8.7-.5-18.4 6.4-24.6l43.3-39.4C64.6 273.1 64 264.6 64 256s.6-17.1 1.7-25.4L22.4 191.2c-6.9-6.2-9.6-15.9-6.4-24.6 4.4-11.9 9.7-23.3 15.8-34.3l4.7-8.1c6.6-11 14-21.4 22.1-31.2 5.9-7.2 15.7-9.6 24.5-6.8l55.7 17.7c13.4-10.3 28.2-18.9 44-25.4l12.5-57.1c2-9.1 9-16.3 18.2-17.8C227.3 1.2 241.5 0 256 0s28.7 1.2 42.5 3.5c9.2 1.5 16.2 8.7 18.2 17.8l12.5 57.1c15.8 6.5 30.6 15.1 44 25.4l55.7-17.7c8.8-2.8 18.6-.3 24.5 6.8 8.1 9.8 15.5 20.2 22.1 31.2l4.7 8.1c6.1 11 11.4 22.4 15.8 34.3zM256 336a80 80 0 1 0 0-160 80 80 0 1 0 0 160z"/></svg>
  EMU
</button>

<div id="__emu_panel">
  <div class="__emu_hdr">
    <span>⚙ Emulator <small>codex/bthome-ble</small></span>
  </div>

  <div class="__emu_sec">
    <div class="__emu_sec_title">Compiled Features</div>
    <div id="__emu_feat_list"></div>
  </div>

  <div class="__emu_sec">
    <div class="__emu_sec_title">Mock Sensor Values</div>
    <div class="__emu_sensor_row">
      <div class="__emu_sensor_lbl">
        <span>CO₂ (ppm)</span>
        <input type="number" id="__emu_co2_n" min="400" max="5000" value="850">
      </div>
      <input type="range" id="__emu_co2_r" min="400" max="5000" step="10" value="850">
    </div>
    <div class="__emu_sensor_row">
      <div class="__emu_sensor_lbl">
        <span>Temperature (°C)</span>
        <input type="number" id="__emu_tmp_n" min="-10" max="60" step="0.5" value="22.5">
      </div>
      <input type="range" id="__emu_tmp_r" min="-10" max="60" step="0.5" value="22.5">
    </div>
    <div class="__emu_sensor_row">
      <div class="__emu_sensor_lbl">
        <span>Humidity (%)</span>
        <input type="number" id="__emu_hum_n" min="0" max="100" value="55">
      </div>
      <input type="range" id="__emu_hum_r" min="0" max="100" step="1" value="55">
    </div>
    <div class="__emu_sensor_row">
      <div class="__emu_sensor_lbl">
        <span>Battery (V)</span>
        <input type="number" id="__emu_bat_n" min="2.5" max="4.2" step="0.01" value="3.85">
      </div>
      <input type="range" id="__emu_bat_r" min="2.5" max="4.2" step="0.01" value="3.85">
    </div>
  </div>

  <div class="__emu_status" id="__emu_status">loading…</div>
</div>

<script>
(function () {
  'use strict';

  // ── Fetch interceptor — must be installed first ───────────────────────────
  //
  // Sensor endpoints return from _emuSensors immediately so the page always
  // sees up-to-date values without waiting for the next poll interval.
  // Server POSTs go through _origFetch to avoid intercepting ourselves.

  const _origFetch = window.fetch.bind(window);
  const _emuSensors = { co2: 850, temperature: 22.5, humidity: 55.0, batteryVoltage: 3.85 };

  window.fetch = function (resource, init) {
    const url  = typeof resource === 'string' ? resource : (resource && resource.url) || '';
    const path = url.split('?')[0];
    function ok(body) {
      return Promise.resolve(new Response(body, { status: 200, headers: { 'Content-Type': 'text/plain' } }));
    }
    if (path === '/readCO2')            return ok(String(Math.round(_emuSensors.co2)));
    if (path === '/readTemperature')    return ok(_emuSensors.temperature.toFixed(1));
    if (path === '/readHumidity')       return ok(String(Math.round(_emuSensors.humidity)));
    if (path === '/readBatteryVoltage') return ok(_emuSensors.batteryVoltage.toFixed(2));
    return _origFetch(resource, init);
  };

  // ── Constants ─────────────────────────────────────────────────────────────

  const API_TO_JS = {
    BLE:            'SUPPORT_BLE',
    BTHomeBLE:      'SUPPORT_BTHOME_BLE',
    Buzzer:         'SUPPORT_BUZZER',
    EspNow:         'SUPPORT_ESPNOW',
    mDNS:           'SUPPORT_MDNS',
    MQTT:           'SUPPORT_MQTT',
    MQTTDiscovery:  'SUPPORT_MQTT_DISCOVERY',
    OTA:            'SUPPORT_OTA',
    LowPower:       'SUPPORT_LOW_POWER',
    CircularBuffer: 'SUPPORT_CIRCULAR_BUFFER',
  };

  const LABELS = {
    BLE:            'BLE (Sensirion / MyAmbiance)',
    BTHomeBLE:      'BTHome BLE (Home Assistant)',
    Buzzer:         'Buzzer',
    EspNow:         'ESP-NOW',
    mDNS:           'mDNS',
    MQTT:           'MQTT',
    MQTTDiscovery:  'MQTT Discovery',
    OTA:            'OTA Updates',
    LowPower:       'Low Power / Deep Sleep',
    CircularBuffer: 'Charts (Circular Buffer)',
  };

  const fab    = document.getElementById('__emu_fab');
  const panel  = document.getElementById('__emu_panel');
  const status = document.getElementById('__emu_status');
  const OPEN_KEY = '__emu_open';

  // ── Collapse / expand ─────────────────────────────────────────────────────
  function setOpen(open) {
    panel.classList.toggle('open', open);
    sessionStorage.setItem(OPEN_KEY, open ? '1' : '0');
  }
  fab.addEventListener('click', () => setOpen(!panel.classList.contains('open')));
  if (sessionStorage.getItem(OPEN_KEY) === '1') setOpen(true);

  // ── Status line ───────────────────────────────────────────────────────────
  function setStatus(msg, cls) {
    status.innerHTML = '<span class="' + (cls || '') + '">' + msg + '</span>';
  }

  // ── Trigger the page's own sensor-display functions immediately ───────────
  function refreshPageSensors() {
    // status.js
    if (typeof fetchAndUpdateCO2Value         === 'function') fetchAndUpdateCO2Value();
    if (typeof fetchAndUpdateTemperatureValue  === 'function') fetchAndUpdateTemperatureValue();
    if (typeof fetchAndUpdateHumidityValue     === 'function') fetchAndUpdateHumidityValue();
    if (typeof fetchAndUpdateBatteryVoltage    === 'function') fetchAndUpdateBatteryVoltage();
    // index.js
    if (typeof updateCO2Data         === 'function') updateCO2Data();
    if (typeof updateTemperatureData  === 'function') updateTemperatureData();
    if (typeof updateHumidityData     === 'function') updateHumidityData();
  }

  // ── Apply feature change to the page's live JS state ─────────────────────
  function applyFeatureToPage(apiKey, checked) {
    const jsKey = API_TO_JS[apiKey];
    if (jsKey && window.features) window.features[jsKey] = checked;
    if (apiKey === 'BTHomeBLE' && typeof window.supportBTHomeBLE !== 'undefined') {
      window.supportBTHomeBLE = checked;
    }
    if (typeof window.applyFeatureVisibility === 'function') window.applyFeatureVisibility();
    if (typeof window.initNavBar             === 'function') window.initNavBar();
    if (typeof window.fillFeaturesFromServer === 'function') window.fillFeaturesFromServer();
  }

  // ── Feature toggles ───────────────────────────────────────────────────────
  function buildFeatureList(feats) {
    const list = document.getElementById('__emu_feat_list');
    list.innerHTML = '';
    for (const [key, label] of Object.entries(LABELS)) {
      const on = !!feats[key];
      const row = document.createElement('label');
      row.className = '__emu_feat_row';
      row.innerHTML =
        '<span class="__emu_sw">' +
          '<input type="checkbox" id="__emuf_' + key + '"' + (on ? ' checked' : '') + '>' +
          '<span class="__emu_sw_track"></span>' +
        '</span>' +
        '<span class="__emu_label' + (on ? '' : ' off') + '" id="__emul_' + key + '">' + label + '</span>';
      list.appendChild(row);

      document.getElementById('__emuf_' + key).addEventListener('change', function () {
        const checked = this.checked;
        document.getElementById('__emul_' + key).classList.toggle('off', !checked);
        _origFetch('/emu/features', {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify({ [key]: checked }),
        })
        .then(r => {
          if (r.ok) {
            setStatus(label + ' → ' + (checked ? 'ON' : 'OFF'), 'ok');
            applyFeatureToPage(key, checked);
          } else {
            setStatus('Save failed (' + r.status + ')', 'err');
          }
        })
        .catch(e => setStatus('Error: ' + e.message, 'err'));
      });
    }
  }

  // ── Sensor sliders ────────────────────────────────────────────────────────
  function readSliders() {
    return {
      co2:            parseFloat(document.getElementById('__emu_co2_n').value),
      temperature:    parseFloat(document.getElementById('__emu_tmp_n').value),
      humidity:       parseFloat(document.getElementById('__emu_hum_n').value),
      batteryVoltage: parseFloat(document.getElementById('__emu_bat_n').value),
    };
  }

  function syncSlidersFromState(s) {
    [['__emu_co2_r',  '__emu_co2_n',  s.co2],
     ['__emu_tmp_r',  '__emu_tmp_n',  s.temperature],
     ['__emu_hum_r',  '__emu_hum_n',  s.humidity],
     ['__emu_bat_r',  '__emu_bat_n',  s.batteryVoltage]
    ].forEach(([rid, nid, val]) => {
      document.getElementById(rid).value = val;
      document.getElementById(nid).value = val;
    });
    Object.assign(_emuSensors, s);
  }

  let _sensorTimer = null;
  function onSensorInput() {
    // 1. Update local intercept table immediately
    Object.assign(_emuSensors, readSliders());
    // 2. Refresh page displays without waiting for the next poll interval
    refreshPageSensors();
    // 3. Persist to server (debounced — skip if rapid sliding)
    clearTimeout(_sensorTimer);
    _sensorTimer = setTimeout(() => {
      _origFetch('/emu/sensors', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(_emuSensors),
      })
      .then(r => { if (r.ok) setStatus('CO₂ ' + Math.round(_emuSensors.co2) + ' ppm · ' +
                                       _emuSensors.temperature.toFixed(1) + '°C · ' +
                                       Math.round(_emuSensors.humidity) + '% RH', 'ok'); })
      .catch(e => setStatus('Sensor error: ' + e.message, 'err'));
    }, 400);
  }

  function linkSlider(rid, nid) {
    const r = document.getElementById(rid);
    const n = document.getElementById(nid);
    r.addEventListener('input', () => { n.value = r.value; onSensorInput(); });
    n.addEventListener('input', () => { r.value = n.value; onSensorInput(); });
  }
  linkSlider('__emu_co2_r', '__emu_co2_n');
  linkSlider('__emu_tmp_r', '__emu_tmp_n');
  linkSlider('__emu_hum_r', '__emu_hum_n');
  linkSlider('__emu_bat_r', '__emu_bat_n');

  // ── Bootstrap ─────────────────────────────────────────────────────────────
  _origFetch('/emu/state')
    .then(r => r.json())
    .then(d => {
      buildFeatureList(d.features);
      syncSlidersFromState(d.sensors);
      setStatus('Ready', 'ok');
    })
    .catch(e => setStatus('Load error: ' + e.message, 'err'));
})();
</script>
<!-- ══════════════════════════════════════════════════════════════════════════ -->
"""


def inject_emu_panel(html: str) -> str:
    """Insert the floating emulator panel just before </body>."""
    tag = "</body>"
    idx = html.lower().rfind(tag)
    if idx == -1:
        return html + _EMU_INJECT
    return html[:idx] + _EMU_INJECT + html[idx:]


def serve_html(filename: str) -> Response:
    """Read an HTML file from webserver/, inject the panel, and return it."""
    path = os.path.join(WEBSERVER_DIR, filename)
    try:
        with open(path, encoding="utf-8") as f:
            html = f.read()
    except OSError:
        return Response("Not found", status=404)
    return Response(inject_emu_panel(html), mimetype="text/html")


# ── ESP32 API mocks ───────────────────────────────────────────────────────────

@app.route("/getFeaturesAsJson")
def api_features():
    return jsonify(features)


@app.route("/getActualSettingsAsJson")
def api_settings():
    out = dict(prefs)
    out["supportBTHomeBLE"] = features.get("BTHomeBLE", False)
    return jsonify(out)


@app.route("/savePreferences", methods=["POST"])
def api_save():
    data = request.get_json(force=True, silent=True) or {}
    prefs.update(data)
    return Response("OK", status=200)


@app.route("/getVersion")
def api_version():
    return jsonify(version_info)


@app.route("/readCO2")
def api_co2():
    return str(int(sensors["co2"]))


@app.route("/readTemperature")
def api_temp():
    return f"{sensors['temperature']:.1f}"


@app.route("/readHumidity")
def api_hum():
    return f"{sensors['humidity']:.0f}"


@app.route("/readBatteryVoltage")
def api_batt():
    return f"{sensors['batteryVoltage']:.2f}"


@app.route("/getMeasurementInterval")
def api_interval():
    return str(prefs.get("measurementInterval", 10))


@app.route("/getFreeHeap")
def api_heap():
    return str(random.randint(180_000, 220_000))


@app.route("/getMinFreeHeap")
def api_min_heap():
    return str(random.randint(140_000, 170_000))


@app.route("/settings")
def api_settings_cmd():
    p = request.args
    if "CalibrateCO2" in p:
        print(f"[EMU] CalibrateCO2 = {p['CalibrateCO2']}")
    if "SetVRef" in p:
        prefs["vRef"] = int(p["SetVRef"])
    if "ToggleDisplayReverse" in p:
        prefs["displayReverse"] = not prefs.get("displayReverse", False)
    if "setDisplayBrightness" in p:
        prefs["DisplayBright"] = int(p["setDisplayBrightness"])
    if "showTemp" in p:
        prefs["showTemp"] = p["showTemp"].lower() == "true"
    if "showHumidity" in p:
        prefs["showHumidity"] = p["showHumidity"].lower() == "true"
    if "showBattery" in p:
        prefs["showBattery"] = p["showBattery"].lower() == "true"
    return Response("OK", status=200)


@app.route("/restart")
def api_restart():
    print("[EMU] Restart requested (simulated)")
    return Response("OK", status=200)


@app.route("/goLowPower")
def api_low_power():
    print("[EMU] goLowPower (simulated)")
    return Response("OK", status=200)


@app.route("/getThresholdsAsJson")
def api_thresholds():
    return jsonify(thresholds)


@app.route("/saveThresholds", methods=["POST"])
def api_save_thresholds():
    data = request.get_json(force=True, silent=True) or {}
    new_list = data.get("thresholds", [])
    for i, slot in enumerate(new_list):
        if i < len(thresholds):
            thresholds[i].update(slot)
    return Response("OK", status=200)


@app.route("/getHistory")
def api_history():
    return Response(b"\x00", status=200, mimetype="application/octet-stream")


@app.route("/status")
def api_status():
    """Full status JSON consumed by status.js loadStatusFromServer()."""
    return jsonify({
        "mainDeviceSelected":   "SCD41",
        "CO2":                  int(sensors["co2"]),
        "Temperature":          round(sensors["temperature"], 1),
        "Humidity":             round(sensors["humidity"]),
        "WiFiStatus":           3,
        "SSID":                 prefs.get("wifiSSID", "MyNetwork"),
        "WiFiPassword":         prefs.get("wifiPass", ""),
        "IP":                   "192.168.1.100",
        "RSSI":                 -55,
        "MACAddress":           "AA:BB:CC:DD:EE:FF",
        "hostName":             prefs.get("hostName", "co2-gadget"),
        "useStaticIP":          prefs.get("useStaticIP", False),
        "staticIP":             prefs.get("staticIP", ""),
        "gateway":              prefs.get("gateway", ""),
        "subnet":               prefs.get("subnet", ""),
        "dns1":                 prefs.get("dns1", ""),
        "dns2":                 prefs.get("dns2", ""),
        "rootTopic":            prefs.get("rootTopic", "CO2_Gadget"),
        "discoveryTopic":       "homeassistant/",
        "mqttClientId":         prefs.get("mqttClientId", "CO2Gadget"),
        "mqttBroker":           prefs.get("mqttBroker", ""),
        "mqttUser":             prefs.get("mqttUser", ""),
        "mqttPassword":         prefs.get("mqttPass", ""),
        "peerESPNowAddress":    prefs.get("peerESPNowAddress", "FF:FF:FF:FF:FF:FF"),
        "activeWiFi":           prefs.get("activeWIFI", True),
        "activeMQTT":           prefs.get("activeMQTT", False),
        "activeBLE":            prefs.get("activeBLE", True),
        "activeOTA":            features.get("OTA", True),
        "troubledWiFi":         False,
        "troubledMQTT":         False,
        "troubledESPNow":       False,
        "measurementInterval":  prefs.get("measurementInterval", 10),
        "sampleInterval":       10,
        "calibrationValue":     prefs.get("customCalValue", 415),
        "pendingCalibration":   False,
        "freeHeap":             random.randint(180_000, 220_000),
        "minFreeHeap":          random.randint(140_000, 170_000),
        "uptime":               int((time.time() - _start_time) * 1000),
        "lowPowerMode":         prefs.get("lowPowerMode", 0),
        "waitToDeep":           prefs.get("waitToDeep", 60),
        "timeSleeping":         prefs.get("timeSleeping", 300),
        "cyclsWifiConn":        prefs.get("cyclsWifiConn", 1),
        "cycRedrawDis":         prefs.get("cycRedrawDis", 0),
        "actBLEOnWake":         prefs.get("actBLEOnWake", True),
        "actWifiOnWake":        prefs.get("actWifiOnWake", True),
        "actMQTTOnWake":        prefs.get("actMQTTOnWake", False),
        "actESPnowWake":        prefs.get("actESPnowWake", False),
        "displayOnWake":        prefs.get("displayOnWake", True),
    })


@app.route("/getCaptivePortalStatusAsJson")
def api_cp_status():
    return jsonify({
        "captivePortalActive":        False,
        "timeCaptivePortalStarted":   0,
        "timeToWaitForCaptivePortal": 0,
        "captivePortalTimeLeft":      0,
        "captivePortalDebug":         False,
        "relaxedSecurity":            prefs.get("relaxedSecurity", True),
    })


# ── Emulator control API ──────────────────────────────────────────────────────

@app.route("/emu/state")
def emu_state():
    return jsonify({"features": features, "sensors": sensors})


@app.route("/emu/features", methods=["POST"])
def emu_set_features():
    data = request.get_json(force=True, silent=True) or {}
    for k, v in data.items():
        if k in features:
            features[k] = bool(v)
    return jsonify(features)


@app.route("/emu/sensors", methods=["POST"])
def emu_set_sensors():
    data = request.get_json(force=True, silent=True) or {}
    for k, v in data.items():
        if k in sensors:
            sensors[k] = float(v)
    return jsonify(sensors)


# ── Static file serving with HTML injection ───────────────────────────────────

@app.route("/")
def root():
    return serve_html("index.html")


def serve_js_bundle(filename: str) -> Response:
    """Build a JS bundle from webserver/ source files on the fly.

    Concatenates the common files (common.js, themes.js, debugWindow.js)
    followed by the page-specific script, mirroring what the firmware build
    system packs into each data/*.js.gz file.  Serving from source means the
    user's live edits to webserver/*.js are reflected without rebuilding.
    """
    src_name = _JS_ALIAS.get(filename, filename)
    parts = []
    for name in _JS_COMMON_FILES + [src_name]:
        path = os.path.join(WEBSERVER_DIR, name)
        try:
            with open(path, encoding="utf-8") as f:
                parts.append(f.read())
        except OSError:
            pass  # skip missing file silently
    return Response("\n".join(parts), mimetype="application/javascript")


@app.route("/<path:filename>")
def webserver_static(filename):
    if filename.startswith("emu"):
        return Response("Not found", status=404)
    if filename.endswith(".html"):
        return serve_html(filename)
    if filename.endswith(".js"):
        return serve_js_bundle(filename)
    return send_from_directory(WEBSERVER_DIR, filename)


# ── Entry point ───────────────────────────────────────────────────────────────

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="CO2 Gadget Web Emulator")
    parser.add_argument("--port", type=int, default=8080)
    parser.add_argument("--host", default="127.0.0.1")
    args = parser.parse_args()

    print()
    print("  CO2 Gadget Web Emulator")
    print(f"  webserver/ : {WEBSERVER_DIR}  (live source — edits reflected immediately)")
    print(f"  features   : parsed from {PLATFORMIO_INI}")
    enabled  = [k for k, v in features.items() if v]
    disabled = [k for k, v in features.items() if not v]
    print(f"    ON  : {', '.join(enabled)  or '(none)'}")
    print(f"    OFF : {', '.join(disabled) or '(none)'}")
    print()
    print(f"  Open any page — the ⚙ EMU button appears bottom-right")
    print(f"  http://{args.host}:{args.port}/preferences.html")
    print(f"  http://{args.host}:{args.port}/status.html")
    print(f"  http://{args.host}:{args.port}/")
    print()

    app.run(host=args.host, port=args.port, debug=False)
