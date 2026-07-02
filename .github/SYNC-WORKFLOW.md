# Documentation Sync Workflow — CO₂ Gadget

> **Purpose:** Ensure GitHub docs and emariete.com blog stay in sync with the firmware after every feature addition, bug fix, or version bump.

---

## 1. Inventory: What needs to stay in sync

### GitHub (English)

| File | What to sync |
|---|---|
| `platformio.ini` → `CO2_GADGET_REV` | **Firmware version** — single source of truth |
| `CHANGELOG.md` | **Release notes** — every feature/fix gets an entry |
| `README.md` | **Current version**, supported features, build flags, project structure, protocols table |
| `docs/build-configuration.md` | Feature flags table |
| `docs/api-reference.md` | REST API endpoints |
| `docs/BTHome.md` | BTHome technical reference |
| `docs/architecture.md` | System architecture (rarely changes) |
| `docs/hardware-pinout.md` | GPIO mappings (rarely changes) |
| `docs/contributing.md` | Contribution guidelines (rarely changes) |

### Blog emariete.com (Spanish, translated to English via TranslatePress)

| Post | Slug | What to sync |
|---|---|---|
| 🏠 Hub | `/medidor-co2-gadget/` | **Version**, protocols, featured features, guide links |
| 📡 BLE guide | `/co2-gadget-guia-ble/` | New BLE protocols (BTHome) |
| 🆕 BTHome guide | `/co2-gadget-guia-bthome/` | BTHome technical content |
| ⚡ WiFi guide | `/co2-gadget-guia-wifi/` | (only when REST API changes) |
| 🗄️ MQTT guide | `/co2-gadget-guia-mqtt/` | (only when HA Discovery changes) |
| 🔋 Low Power guide | `/co2-gadget-guia-low-power/` | New power-saving modes |
| 🔧 Calibration guide | `/co2-gadget-guia-calibracion/` | (only when calibration process changes) |
| 🖥️ Web UI guide | `/co2-gadget-guia-web/` | New pages or features |
| 🔊 Outputs guide | `/co2-gadget-guia-salidas/` | (only when outputs change) |
| 🧪 Sensors guide | `/co2-gadget-guia-sensores/` | New supported sensors |
| 📥 Installation guide | `/co2-gadget-instalacion/` | New boards or install methods |
| 📡 ESP-NOW guide | `/co2-gadget-guia-espnow/` | (only when protocol changes) |
| 📰 Release blog post | `/co2-gadget-bthome-release/` | New version announcements |

### Blog drafts (in repo, `docs/blog-drafts/`)

These are Markdown drafts with content prepared for blog publication. They stay in sync with the blog but may be slightly outdated if edits are made directly in WordPress.

---

## 2. Trigger checklist: When to sync

### 🔴 ALWAYS sync when:
- [ ] **Version bump** (`platformio.ini` → `CO2_GADGET_REV`)
  - Update `CHANGELOG.md` header
  - Update `README.md` "Current version"
  - Update hub page version table in blog
  - Optionally publish blog post announcing the release

- [ ] **New feature added**
  - Add entry in `CHANGELOG.md` (`### Added`)
  - If user-facing: update `README.md` (protocols table, features list, mobile app section, build flags, project structure)
  - If new build flag: update `docs/build-configuration.md`
  - If new API endpoint: update `docs/api-reference.md`
  - If blog-relevant: create or update blog guide
  - Consider blog post announcing the feature

### 🟡 Check when:
- [ ] **Bug fix** — add entry in `CHANGELOG.md` (`### Fixed`)
- [ ] **New board support** — update `README.md` board table + installation guide
- [ ] **New sensor support** — update README sensors table + sensors guide (blog)
- [ ] **API change** — update `docs/api-reference.md` + WiFi guide (blog)

---

## 3. Version bump procedure

When bumping `CO2_GADGET_REV` in `platformio.ini`, check ALL these files:

```
platformio.ini           → CO2_GADGET_REV (single source of truth)
CHANGELOG.md             → header + added/fixed entries
README.md                → "Current version" line
docs/blog-drafts/*.md    → version table (hub)
Blog hub page            → version table (via WP Admin or REST API)
Blog post (if applicable) → new post announcing the version
```

### Version numbering rules
- **MAJOR.MINOR.PATCH-beta** (e.g., `0.16.014-beta`)
- If blog already shows a version (e.g., 013) but firmware hasn't been bumped yet:
  - Do NOT bump to the blog version (would be confusing)
  - Bump to **next available** (e.g., blog says 013 → bump firmware to 014)

---

## 4. Blog update procedure (via WordPress)

### WordPress access
- URL: `https://emariete.com/wp-admin/`
- Auth stored securely in Copilot agent memory (user memory scope)
- **REST API** (preferred): use Application Password with Basic Auth
- **Web login** (visual editing): use WordPress regular password
- **Ask the user** if credentials are needed — never stored in repo files

### Post IDs (CO2 Gadget category = 13)
| Post | ID |
|---|---|
| Hub (`medidor-co2-gadget`) | 238964 |
| BLE guide | 243029 |
| WiFi guide | 243020 |
| MQTT guide | 243026 |
| Low Power guide | 243031 |
| ESP-NOW guide | 243032 |
| Outputs guide | 243033 |
| Calibration guide | 243034 |
| Web UI guide | 243035 |
| Sensors guide | 243028 |
| Installation guide | 243030 |
| BTHome guide | 243087 |
| Release blog post BTHome | 243088 |

### Content format
Blog posts use **Gutenberg block format** with HTML comments:
```html
<!-- wp:paragraph -->
<p>Content here</p>
<!-- /wp:paragraph -->

<!-- wp:heading -->
<h2>Section title</h2>
<!-- /wp:heading -->

<!-- wp:list -->
<ul><li>Item</li></ul>
<!-- /wp:list -->
```

---

## 5. Required tools

| Tool | Purpose |
|---|---|
| Browser (Playwright) | Navigate wp-admin, edit posts visually |
| REST API (fetch) | Read/write posts programmatically |
| `multi_replace_string_in_file` | Bulk edit GitHub docs |
| `create_file` | Create new blog drafts |
| Memory (repo) | Store version, post IDs, workflow notes |

---

## 6. Verification checklist

After any sync operation, verify:

- [ ] `pio run -e TTGO_TDISPLAY` compiles (firmware version is valid)
- [ ] `README.md` — "Current version" matches `platformio.ini`
- [ ] `CHANGELOG.md` — header matches `platformio.ini`
- [ ] Blog hub page — version table matches firmware
- [ ] All affected guides updated on blog
- [ ] No stale version numbers remain (grep for old version)

---

## 7. Quick reference: GitHub file locations

| File | Path |
|---|---|
| Firmware version | `CO2-Gadget/platformio.ini` (line 14) |
| Changelog | `CO2-Gadget/CHANGELOG.md` |
| Main README | `CO2-Gadget/README.md` |
| Build config | `CO2-Gadget/docs/build-configuration.md` |
| API reference | `CO2-Gadget/docs/api-reference.md` |
| BTHome reference | `CO2-Gadget/docs/BTHome.md` |
| Blog drafts | `CO2-Gadget/docs/blog-drafts/` |
