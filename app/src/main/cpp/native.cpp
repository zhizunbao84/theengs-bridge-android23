#include <jni.h>
#include <string>
#include <optional>
#include "decoder.h"
#include <ArduinoJson.h>

extern "C"
JNIEXPORT jstring JNICALL
Java_com_theengs_bridge_BleService_decode(JNIEnv *env, jobject /*thiz*/,
                                          jbyteArray adv, jint rssi, jstring jmac) {
    jsize len = env->GetArrayLength(adv);
    uint8_t *p = (uint8_t *)env->GetByteArrayElements(adv, nullptr);
    const char *mac = env->GetStringUTFChars(jmac, nullptr);

    // 解码（返回 optional<JsonObject>）
    std::optional<JsonObject> json = theengs::decodeBLE(
        std::string((char *)p, len), mac, rssi);

    env->ReleaseByteArrayElements(adv, (jbyte *)p, JNI_ABORT);
    env->ReleaseStringUTFChars(jmac, mac);

    if (!json) return nullptr;

    // 序列化返回
    std::string jsonStr;
    serializeJson(*json, jsonStr);
    return env->NewStringUTF(jsonStr.c_str());
}
