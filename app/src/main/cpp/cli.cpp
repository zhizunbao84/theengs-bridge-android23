#include <iostream>
#include <string>
#include <ArduinoJson.h>
#include "decoder.h"

int main() {
    std::string hex;
    if (!std::getline(std::cin, hex)) return 1;

    StaticJsonDocument<512> doc;
    JsonObject json = doc.to<JsonObject>();

    bool ok = TheengsDecoder::decodeBLEJson(json);   // 全局函数
    if (!ok) return 1;

    std::string out;
    serializeJson(json, out);
    std::cout << out << std::endl;
    return 0;
}
