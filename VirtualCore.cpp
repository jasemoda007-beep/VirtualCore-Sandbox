#include <jni.h>
#include <string>
#include <android/log.h>

#define LOG_TAG "W_MASTER_KERN"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

extern "C" {

    // 1. الدالة التي جربتها ونجحت
    JNIEXPORT jstring JNICALL
    Java_com_my_newnas_NativeEngine_getKernelStatus(JNIEnv* env, jclass clazz) {
        return env->NewStringUTF("W-MASTER KERNEL v16.0: ACTIVE ✅");
    }

    // 2. دالة تزييف موديل الجهاز (Spoofing)
    JNIEXPORT jstring JNICALL
    Java_com_my_newnas_NativeEngine_spoofDeviceModel(JNIEnv* env, jclass clazz, jint modelIndex) {
        const char* models[] = {"ROG PHONE 8 PRO", "iPAD PRO M4", "RED MAGIC 9S", "GALAXY S24 ULTRA"};
        
        LOGD(">> Spoofing Device Identity to: %s", models[modelIndex]);
        
        // هنا مستقبلاً سنقوم بحقن هذا الاسم في ذاكرة التطبيق المنسوخ
        return env->NewStringUTF(models[modelIndex]);
    }

    // 3. دالة تشغيل الساندبوكس (تجهيز المسارات)
    JNIEXPORT void JNICALL
    Java_com_my_newnas_NativeEngine_startSandbox(JNIEnv* env, jclass clazz, jstring pkgName) {
        const char *nativePkgName = env->GetStringUTFChars(pkgName, 0);
        LOGD(">> Sandbox Shield Activated for: %s", nativePkgName);
        env->ReleaseStringUTFChars(pkgName, nativePkgName);
    }
}
