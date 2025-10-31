#include <iostream>
#include <string>
#include "decoder.h"
#include <ArduinoJson.h>

int main() {
    std::string hex;
    if (!std::getline(std::cin, hex)) return 1;

    StaticJsonDocument<512> doc;
    JsonObject json = doc.to<JsonObject>();

    // 实例化解码器并调用成员函数
    TheengsDecoder decoder;
    bool ok = decoder.decodeBLEJson(json);   // 类成员函数

    if (!ok) return 1;

    std::string out;
    serializeJson(json, out);
    std::cout << out << std::endl;
    return 0;
}
