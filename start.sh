#!/usr/bin/env bash

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
APP_BIN="$SCRIPT_DIR/pb-safety"

# If user did not set PVB_DB_PATH manually, try DB near the launcher.
if [ -z "${PVB_DB_PATH:-}" ] && [ -f "$SCRIPT_DIR/PVB_DB.db" ]; then
  export PVB_DB_PATH="$SCRIPT_DIR/PVB_DB.db"
fi

exec "$APP_BIN" "$@"
