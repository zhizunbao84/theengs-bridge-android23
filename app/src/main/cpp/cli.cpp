#include <iostream>
#include <string>
#include <optional>
#include "decoder.h"
#include <ArduinoJson.h>

int main() {
    // 从 stdin 读 hex 字符串
    std::string hex;
    if (!std::getline(std::cin, hex)) return 1;

    // 解码（dummy MAC & RSSI）
    std::optional<JsonObject> json = decodeBLE(hex, "00:00:00:00:00:00", -50);
    if (!json) return 1;

    // 输出 JSON
    std::string out;
    serializeJson(*json, out);
    std::cout << out << std::endl;
    return 0;
}
