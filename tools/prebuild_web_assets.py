"""
Pre-build script for PlatformIO: regenerate minified/gzipped web assets.

Called via `extra_scripts` in platformio.ini.
Runs the PowerShell pipeline if any source file in webserver/ is newer
than its corresponding .gz output in data/.
"""

import os
import subprocess
import shutil
import sys
from pathlib import Path

# PlatformIO runs pre: scripts via exec(), which does not define __file__.
# Fall back to cwd when __file__ is absent.
try:
    _script_dir = Path(__file__).resolve().parent
except NameError:
    _script_dir = Path(os.path.abspath(sys.argv[0])).resolve().parent

PROJECT_DIR = _script_dir.parent
WEBSERVER_DIR = PROJECT_DIR / "webserver"
DATA_DIR = PROJECT_DIR / "data"
PIPELINE_SCRIPT = WEBSERVER_DIR / "MinifyCompressAllFiles.ps1"

# Files that, when changed, trigger a rebuild of ALL bundles
TRIGGER_FILES = [
    "common.js",
    "themes.js",
    "captiveportal.js",
    "captivePortalStatusBar.js",
    "serverStatusDot.js",
    "debugWindow.js",
    "style.css",
]

# Page-specific files that only trigger rebuild of their own bundle
PAGE_FILES = {
    "index.js": "index.js.gz",
    "preferences.js": "preferences.js.gz",
    "status.js": "status.js.gz",
    "ota.js": "ota.js.gz",
    "low_power.js": "low_power.js.gz",
    "calibration.js": "calibration.js.gz",
    "charts.js": "charts.js.gz",
}


def needs_rebuild() -> bool:
    """Check if any source file is newer than the corresponding .gz output."""
    if not all((DATA_DIR / name).exists() for name in PAGE_FILES.values()):
        return True  # Missing any .gz file → rebuild

    # Check shared trigger files
    for name in TRIGGER_FILES:
        src = WEBSERVER_DIR / name
        if not src.exists():
            continue
        # If ANY .gz is older than this trigger file → rebuild all
        for gz_name in PAGE_FILES.values():
            gz = DATA_DIR / gz_name
            if gz.exists() and src.stat().st_mtime > gz.stat().st_mtime:
                return True

    # Check page-specific files
    for page_src, page_gz in PAGE_FILES.items():
        src = WEBSERVER_DIR / page_src
        gz = DATA_DIR / page_gz
        if src.exists() and gz.exists() and src.stat().st_mtime > gz.stat().st_mtime:
            return True

    # Check HTML files
    for html_file in WEBSERVER_DIR.glob("*.html"):
        gz = DATA_DIR / f"{html_file.name}.gz"
        if gz.exists() and html_file.stat().st_mtime > gz.stat().st_mtime:
            return True

    # Check CSS
    css_src = WEBSERVER_DIR / "style.css"
    css_gz = DATA_DIR / "style.css.gz"
    if css_src.exists() and css_gz.exists() and css_src.stat().st_mtime > css_gz.stat().st_mtime:
        return True

    return False


def run_pipeline():
    """Execute the PowerShell minification pipeline."""
    print("─" * 60)
    print("  🔨 Web assets: source files changed — rebuilding...")
    print("─" * 60)

    # Detect available PowerShell (cross-platform: pwsh on macOS/Linux, powershell on Windows)
    pwsh = shutil.which("pwsh") or shutil.which("powershell")
    if not pwsh:
        print("  ❌ No PowerShell executable found (tried pwsh, powershell) — cannot rebuild web assets", file=sys.stderr)
        print("  ❌ Run the pipeline manually: powershell -File webserver/MinifyCompressAllFiles.ps1 -DeleteMinifiedFiles", file=sys.stderr)
        print("  ❌ Aborting build to prevent stale assets in firmware", file=sys.stderr)
        sys.exit(1)

    try:
        result = subprocess.run(
            [
                pwsh,
                "-ExecutionPolicy",
                "Bypass",
                "-File",
                str(PIPELINE_SCRIPT),
                "-DeleteMinifiedFiles",
            ],
            cwd=str(PROJECT_DIR),
            capture_output=True,
            text=True,
            timeout=120,
        )
        if result.returncode == 0:
            print(result.stdout)
            print("  ✅ Web assets rebuilt successfully")
        else:
            print(result.stdout)
            print(result.stderr)
            print(f"  ⚠️  Pipeline exited with code {result.returncode}", file=sys.stderr)
    except FileNotFoundError:
        print(f"  ⚠️  PowerShell ({pwsh}) not found — skipping web asset rebuild", file=sys.stderr)
    except subprocess.TimeoutExpired:
        print("  ⚠️  Pipeline timed out — skipping web asset rebuild", file=sys.stderr)


def main():
    if not PIPELINE_SCRIPT.exists():
        print(f"  [web-assets] Pipeline script not found: {PIPELINE_SCRIPT}")
        print(f"  [web-assets] Skipping auto-rebuild. Run manually:")
        print(f'  [web-assets]   powershell -ExecutionPolicy Bypass -File "{PIPELINE_SCRIPT}" -DeleteMinifiedFiles')
        return

    if needs_rebuild():
        run_pipeline()
    else:
        print("  [web-assets] up to date — skipping rebuild")


if __name__ == "__main__":
    main()
