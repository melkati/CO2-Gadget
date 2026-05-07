# Fase 1 Log — Merge development-low-power
**Sprint:** Modernización v2 | **Fecha inicio:** 2026-05-07  
**Rama objetivo:** `modernization/v2`  
**TIMebox:** 3 días máximo

---

## Análisis previo al merge (2026-05-07)

### Estado de modernization/v2 vs master
- Diferencia mínima (4 archivos, 284 líneas net):
  - `CO2_Gadget.ino`: `AsyncElegantOTA.h` → `ElegantOTA.h` (2 lugares)
  - `CO2_Gadget_WIFI.h`: API `AsyncElegantOTA.*` → `ElegantOTA.*` (2 lugares)
  - `platformio.ini`: 7 cambios de librería (AsyncTCP, ESPAsyncWebServer, ElegantOTA, BLE desactivado, -fpermissive, pinning)
  - `PLAN.md`: plan de modernización (nuevo archivo)

### Magnitud del merge a integrar

#### origin/development (+43 commits sobre master)
- **34 archivos cambiados**, 2081 inserciones, 1544 borrados
- Archivos clave: `CO2_Gadget_Preferences.h` (+265 líneas), `webserver/` completo renovado, `bootlogo.h` simplificado
- Commits destacados: hasBattery, web server improvements, MQTT fixes, OLED fixes

#### origin/development-low-power (+89 commits sobre development)
- **39 archivos cambiados**, 3470 inserciones, 281 borrados
- Archivos nuevos: `CO2_Gadget_DeepSleep.h` (861 líneas!), `webserver/low_power.html` (801 líneas), `webserver/low_power.js` (322 líneas)
- Commits críticos:
  - `ec9e2a1` - Version getters fix
  - `08244d3` - Display reverse on wake + deep sleep refactor
  - `48ad5e0` - Wake up times tracking
  - `feaca42` - e-Ink redraw fixes
  - `d46a0f8` - Force workingOnExternalPower (#235)

### Riesgo evaluado: BAJO-MEDIO
- Los cambios de `modernization/v2` son pequeños y precisos (4 strings)
- `development` y `development-low-power` son lineales (no divergen entre sí)
- Los 3 archivos que modernization/v2 toca también los toca development → conflictos esperados pero manejables

---

## Paso 1: Merge origin/development → modernization/v2

**Fecha:** 2026-05-07  
**Comando:** `git merge --no-commit --no-ff origin/development`

### Resultado: MERGE LIMPIO ✅
- **0 conflictos** — git auto-resolvió todo correctamente
- **34 archivos** en staging listos para commit
- Verificación de integridad post-merge:
  - `platformio.ini`: todos los cambios de modernización preservados
    - `ESP32Async/AsyncTCP @ ^3.4.10` ✅
    - `ESP32Async/ESPAsyncWebServer @ ^3.11.0` ✅  
    - `ayushsharma82/ElegantOTA` ✅
    - `ArduinoJson @ 7.0.1` (pinned) ✅
    - `ArduinoMenu library @ 4.21.4` (pinned) ✅
    - `SUPPORT_BLE` desactivado ✅
    - `-DELEGANTOTA_USE_ASYNC_WEBSERVER=1` ✅
    - `-fpermissive` ✅
    - BLE library comentada ✅
  - `CO2_Gadget.ino`: `ElegantOTA.h` (no AsyncElegantOTA) ✅ (x2)
  - `CO2_Gadget_WIFI.h`: `ElegantOTA.begin/loop()` ✅

### Archivos mergeados (34):
```
CO2_Gadget.ino, CO2_Gadget_Battery.h, CO2_Gadget_EINK.h, CO2_Gadget_Menu.h,
CO2_Gadget_OLED.h, CO2_Gadget_Preferences.h, CO2_Gadget_TFT.h, CO2_Gadget_WIFI.h,
FontNotoSansBold120ptDigits.h, README.md, bootlogo.h, data/*.gz (10 archivos),
platformio.ini, webserver/* (12 archivos)
```

**Estado:** Pendiente de commit (aprobación del usuario requerida)

---

## Paso 2: Merge origin/development-low-power → modernization/v2

**Fecha:** 2026-05-07  
**Comando:** `git merge --no-commit --no-ff origin/development-low-power`

### Resultado: 1 CONFLICTO en platformio.ini ✅ resuelto manualmente

**Archivos auto-mergeados limpios:** 38 de 39  
**Conflicto manual:** `platformio.ini` (2 zonas)

#### Zona 1 — [features] build_flags
- HEAD: `-DELEGANTOTA_USE_ASYNC_WEBSERVER=1`
- low-power: `-DSUPPORT_LOW_POWER`
- **Resolución: AMBOS** (son independientes, los dos necesarios)

#### Zona 2 — [LIBS] lib_deps
- HEAD: `canairio_sensorlib.git#fixOffset`
- low-power: `canairio_sensorlib.git#lowPowerMode` + nuevo `Adafruit_SCD30.git#stopContinuousMeasurement` + BLE reactivado
- **Resolución:** usar `#lowPowerMode` + añadir `Adafruit_SCD30`, mantener BLE **comentado** (deuda técnica)

### Archivos nuevos incorporados:
- `CO2_Gadget_DeepSleep.h` (861 líneas) — implementación completa deep sleep
- `webserver/low_power.html` (801 líneas) — página web low power
- `webserver/low_power.js` (322 líneas)
- `libs/CO2_Gadget_Thresholds/src/` — nueva librería de umbrales
- `data/low_power.html.gz`, `data/low_power.js.gz`

**Commit:** `cb69ada` — closes #188, #235

---

## Paso 3: Verificación de compilación

**Fecha:** 2026-05-07

| Entorno | Estado | RAM | Flash | Tiempo |
|---|---|---|---|---|
| `esp32dev` | ✅ SUCCESS | 18.5% (60636B/327680B) | 67.9% (1.2MB/1.7MB) | 41s |
| `TTGO_TDISPLAY` | ✅ SUCCESS | 18.8% (61648B/327680B) | 76.7% (1.36MB/1.7MB) | 89s |
| `TDISPLAY_S3` | ✅ SUCCESS | 19.3% (63340B/327680B) | 20.2% (1.33MB/6.5MB) | 86s |

**Incidencias durante la compilación:**
- `ModuleNotFoundError: No module named 'intelhex'` en primer intento → resuelto instalando en Python 3.11 interno de PlatformIO
- `git config: Permission denied` en TDISPLAY_S3 → warning de caché, no afecta la compilación
- Nota: `pio` CLI no funciona directamente (Python 3.14 del sistema, PlatformIO requiere 3.10-3.13). Workaround: usar `$env:USERPROFILE\.platformio\python3\python.exe -m platformio`

---

## Decisiones y notas

- **2026-05-07**: Elegido merge completo (no cherry-pick) porque el merge de development fue limpio.
  Esto aumenta la confianza en que development-low-power también será manejable.
- La verificación de compilación se hará tras el segundo merge para no perder tiempo si hay conflictos.

---

## Referencia: Commits críticos de development-low-power

```
ec9e2a1  CO2 Gadget Beta v0.14.014 low-power - Version getters fix
08244d3  Fix: Display reverse on wake + deep sleep refactor
48ad5e0  feat: Add wake up times tracking in deep sleep mode
2644627  CO2 Gadget Beta v0.14.013 low-power - EINKBOARDGDEM0213B74 fix
feaca42  CO2 Gadget Beta v0.14.013 low-power - e-Ink redraw fixes
068cee7  Avoid saving deepSleepData.cyclesLeftToRedrawDisplay to preferences
4a9d8d1  chore: Initialize 'initialized' variable as static in scd30HandleFromDeepSleep
9a859a8  feat: Add boot times tracking in deep sleep mode
db72e76  CO2 Gadget Beta v0.14.012 low-power - Add low power icon to web server
01cd2d4  chore: Add toDeepSleep function forward declaration
ee3c38b  feat: Add low power mode endpoint to web server
d46a0f8  Add force workingOnExternalPower (#235)
```
