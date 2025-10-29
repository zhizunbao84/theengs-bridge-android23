#!/bin/bash
set -e
SDK_DIR="$(cd "$(dirname "$0")/../sdk" && pwd)"
mkdir -p "$SDK_DIR"
cd "$SDK_DIR"

# 下载 commandlinetools
CMDLINE=commandlinetools-linux-8512546_latest.zip
wget -q https://dl.google.com/android/repository/$CMDLINE
unzip -q $CMDLINE && rm $CMDLINE
mkdir -p cmdline-tools/latest
mv cmdline-tools/* cmdline-tools/latest/ 2>/dev/null || true

# 接受许可 & 安装平台/NDK
export ANDROID_SDK_ROOT=$SDK_DIR
yes | ./cmdline-tools/latest/bin/sdkmanager --licenses
./cmdline-tools/latest/bin/sdkmanager \
    "platform-tools" \
    "platforms;android-23" \
    "ndk;21.4.7075529"
