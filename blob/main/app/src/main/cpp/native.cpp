#include <jni.h>
#include <string>
#include "theengs/decoder.h"

extern "C"
JNIEXPORT jstring JNICALL
Java_com_theengs_bridge_BleService_decode(
        JNIEnv *env, jobject /*thiz*/,
        jbyteArray adv, jint rssi, jstring jmac) {
    jsize len = env->GetArrayLength(adv);
    uint8_t *p = (uint8_t *) env->GetByteArrayElements(adv, nullptr);
    const char *mac = env->GetStringUTFChars(jmac, nullptr);

    JsonObject json;
    bool ok = decodeBLE(std::string((char *) p, len), mac, rssi, json);

    env->ReleaseByteArrayElements(adv, (jbyte *) p, JNI_ABORT);
    env->ReleaseStringUTFChars(jmac, mac);

    if (!ok) return nullptr;
    return env->NewStringUTF(json.to<std::string>().c_str());
}
