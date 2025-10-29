#!/bin/bash
set -e
ROOT="$(cd "$(dirname "$0")/.." && pwd)"

# 1. 自动装 SDK/NDK（若未安装）
[ -z "$ANDROID_SDK_ROOT" ] && export ANDROID_SDK_ROOT=$ROOT/sdk
if [ ! -d "$ANDROID_SDK_ROOT/ndk" ]; then
    "$ROOT/scripts/install-sdk.sh"
fi

# 2. 生成 local.properties
echo "sdk.dir=$ANDROID_SDK_ROOT" > "$ROOT/local.properties"
echo "ndk.dir=$ANDROID_SDK_ROOT/ndk/21.4.7075529" >> "$ROOT/local.properties"

# 3. 编译
cd "$ROOT"
./gradlew assembleDebug
echo "APK ready: $ROOT/app/build/outputs/apk/debug/app-debug.apk"
