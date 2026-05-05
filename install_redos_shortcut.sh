#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
APP_BIN="$SCRIPT_DIR/pb-safety"
START_SCRIPT="$SCRIPT_DIR/start.sh"
APP_DB="$SCRIPT_DIR/PVB_DB.db"

APP_DIR="$HOME/.local/share/applications"
DESKTOP_DIR="$(xdg-user-dir DESKTOP 2>/dev/null || echo "$HOME/Desktop")"
LAUNCHER_PATH="$APP_DIR/pb-safety.desktop"
DESKTOP_LAUNCHER_PATH="$DESKTOP_DIR/pb-safety.desktop"

if [ ! -x "$APP_BIN" ]; then
  chmod +x "$APP_BIN" 2>/dev/null || true
fi

chmod +x "$START_SCRIPT"

mkdir -p "$APP_DIR"
mkdir -p "$DESKTOP_DIR"

cat > "$LAUNCHER_PATH" <<EOF
[Desktop Entry]
Type=Application
Name=ПВ-БЕЗОПАСНОСТЬ
Comment=Расчеты энергопотенциалов и категорий взрывоопасности
Exec=$START_SCRIPT
Path=$SCRIPT_DIR
Terminal=false
Categories=Utility;
StartupNotify=true
EOF

chmod +x "$LAUNCHER_PATH"
cp -f "$LAUNCHER_PATH" "$DESKTOP_LAUNCHER_PATH"
chmod +x "$DESKTOP_LAUNCHER_PATH"

if command -v gio >/dev/null 2>&1; then
  gio set "$DESKTOP_LAUNCHER_PATH" metadata::trusted true || true
fi

echo "Ярлык создан:"
echo "  - меню приложений: $LAUNCHER_PATH"
echo "  - рабочий стол:    $DESKTOP_LAUNCHER_PATH"

if [ -f "$APP_DB" ]; then
  echo "База найдена: $APP_DB"
else
  echo "Внимание: PVB_DB.db не найден рядом с программой."
fi
