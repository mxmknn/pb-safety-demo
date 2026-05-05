#!/bin/zsh

cmake --build build || exit 1
open build/pb-safety.app
