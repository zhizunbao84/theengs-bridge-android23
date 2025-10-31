#include <jni.h>
#include <string>
#include <ArduinoJson.h>
#include "decoder.h"          // decoder/src/decoder.h

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

    // 2. 调用类成员函数 decodeBLEJson
    bool ok = decodeBLEJson(json);   // 全局函数，无命名空间

    env->ReleaseByteArrayElements(adv, (jbyte *)p, JNI_ABORT);
    env->ReleaseStringUTFChars(jmac, mac);

    if (!ok) return nullptr;

    // 3. 序列化返回
    std::string jsonStr;
    serializeJson(json, jsonStr);
    return env->NewStringUTF(jsonStr.c_str());
}
