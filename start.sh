#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
APP_BIN="$SCRIPT_DIR/pb-safety"
APP_DB="$SCRIPT_DIR/PVB_DB.db"

detect_arch_family() {
  case "$1" in
    x86_64|amd64) echo "x86_64" ;;
    aarch64|arm64) echo "aarch64" ;;
    arm|armv7l|armv6l|armhf) echo "arm" ;;
    i386|i686) echo "x86" ;;
    *) echo "unknown" ;;
  esac
}

detect_binary_arch_family() {
  local binary_desc
  binary_desc="$(LC_ALL=C file -b "$APP_BIN" 2>/dev/null || true)"

  case "$binary_desc" in
    *x86-64*|*x86_64*) echo "x86_64" ;;
    *aarch64*|*arm64*|*ARM\ aarch64*) echo "aarch64" ;;
    *ARM*) echo "arm" ;;
    *80386*|*i386*) echo "x86" ;;
    *) echo "unknown" ;;
  esac
}

if [ ! -x "$APP_BIN" ]; then
  chmod +x "$APP_BIN" 2>/dev/null || true
fi

if [ ! -x "$APP_BIN" ]; then
  echo "Ошибка: файл '$APP_BIN' не найден или не является исполняемым."
  exit 126
fi

HOST_ARCH="$(uname -m)"
HOST_FAMILY="$(detect_arch_family "$HOST_ARCH")"
BIN_FAMILY="$(detect_binary_arch_family)"

if [ "$HOST_FAMILY" != "unknown" ] && [ "$BIN_FAMILY" != "unknown" ] && [ "$HOST_FAMILY" != "$BIN_FAMILY" ]; then
  echo "Ошибка: бинарник собран для другой архитектуры."
  echo "  Архитектура системы: $HOST_ARCH"
  echo "  Архитектура бинарника: $BIN_FAMILY"
  echo "Нужно собрать проект на этой системе (или взять артефакт для $HOST_ARCH)."
  exit 126
fi

# If user did not set PVB_DB_PATH manually, try DB near the launcher.
if [ -z "${PVB_DB_PATH:-}" ] && [ -f "$APP_DB" ]; then
  export PVB_DB_PATH="$APP_DB"
fi

exec "$APP_BIN" "$@"
