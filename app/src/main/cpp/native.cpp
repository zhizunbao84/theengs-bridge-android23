#include <jni.h>
#include <string>
#include <optional>
#include "decoder.h"          // decoder/src/decoder.h
#include <ArduinoJson.h>

extern "C"
JNIEXPORT jstring JNICALL
Java_com_theengs_bridge_BleService_decode(JNIEnv *env, jobject,
                                          jbyteArray adv, jint rssi, jstring jmac) {
    jsize len = env->GetArrayLength(adv);
    uint8_t *p = (uint8_t *)env->GetByteArrayElements(adv, nullptr);
    const char *mac = env->GetStringUTFChars(jmac, nullptr);

    // 1. 构造 JsonObject（ArduinoJson v6）
    StaticJsonDocument<512> doc;
    JsonObject json = doc.to<JsonObject>();

    // 2. 实例化解码器并调用成员函数
    TheengsDecoder decoder;
    bool ok = decoder.decodeBLEJson(json);   // 类成员函数

    env->ReleaseByteArrayElements(adv, (jbyte *)p, JNI_ABORT);
    env->ReleaseStringUTFChars(jmac, mac);

    if (!ok) return nullptr;

    // 3. 序列化返回
    std::string jsonStr;
    serializeJson(json, jsonStr);
    return env->NewStringUTF(jsonStr.c_str());
}
