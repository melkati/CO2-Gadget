# Contributing to CO2-Gadget

> Guidelines for developers who want to contribute code, documentation, or testing.
> **Audience:** Developers and contributors.

---

## Getting Started

1. **Fork the repository** on GitHub
2. **Create a feature branch:**
   ```bash
   git checkout -b feat/my-feature
   # or
   git checkout -b fix/my-fix
   ```
3. **Make your changes** — follow the existing code style
4. **Test your changes** — ensure builds succeed for at least one board flavor
5. **Document your code and PR**
6. **Open a Pull Request** targeting the `development` branch

---

## Development Environment

### Prerequisites
- [PlatformIO](https://platformio.org/) (CLI or VS Code extension) — **required**
- Git
- Visual Studio Code (recommended)

### Building
```bash
# Clone
git clone https://github.com/melkati/CO2-Gadget.git
cd CO2-Gadget

# Configure ports in platformio.ini
#   upload_port = COM13
#   monitor_port = COM13

# Build and upload
pio run -e TTGO_TDISPLAY --target upload

# Monitor serial output
pio run -e TTGO_TDISPLAY --target monitor
```

**Note:** Arduino IDE is not officially supported. Use PlatformIO.

---

## Code Style

- Follow the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
- Run `clang-format` (Google style) before committing
- Use descriptive variable names — avoid single-letter names except for loop counters
- Comment public functions with purpose, parameters, and return values
- Use `RTC_DATA_ATTR` for variables that must persist across deep sleep

---

## Pull Request Guidelines

- Keep PRs focused on a single feature or fix
- Target the `development` branch (not `master`)
- Update [CHANGELOG.md](../CHANGELOG.md) with your changes under the appropriate section
- For major changes, open an issue first to discuss
- Include compile results (RAM/Flash usage) for affected environments

### PR Title Convention
```
feat: Add support for [sensor/feature]
fix: Correct [bug description]
docs: Update [topic] documentation
refactor: Improve [component] structure
```

---

## Testing

We have active Alpha and Beta testing programs. Reach out on [Telegram](https://t.me/emariete_chat).

### Before submitting a PR:
1. Compile for at least one board flavor (preferably `esp32dev` for fastest iteration)
2. For hardware-specific changes, test on actual hardware
3. Document any known limitations or side effects

### Compilation tracking:
Record RAM/Flash percentages for affected environments:
```
Environment: <flavor>
RAM: XX.X% (XXXXX bytes) | Flash: XX.X% (XXXXXXX bytes)
[SUCCESS] Took XX.XX seconds
```

---

## Project Resources

- [GitHub Repository](https://github.com/melkati/CO2-Gadget)
- [Issue Tracker](https://github.com/melkati/CO2-Gadget/issues)
- [Telegram Group](https://t.me/emariete_chat)
- [Blog (User Documentation)](https://emariete.com/category/co2-gadget/)
