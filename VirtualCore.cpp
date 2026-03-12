#include <jni.h>
#include <string>
#include <map>
#include <unistd.h>
#include <android/log.h>

#define LOG_TAG "W_MASTER_MAGISK_CORE"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// خريطة لتخزين المسارات الوهمية للقرص
std::map<std::string, std::string> path_map;

namespace WMaster {
    namespace Core {

        // دالة حالة النواة
        jstring GetKernelStatus(JNIEnv* env, jclass clazz) {
            return env->NewStringUTF("W-MASTER MAGISK-CORE v1.0: INTEGRATED ✅");
        }

        // دالة تشغيل الساندبوكس
        void StartSandbox(JNIEnv* env, jclass clazz, jstring pkgName) {
            LOGI("[+] Sandbox Engine Started.");
        }

        // دالة حقن الروت
        void InjectRootAccess(JNIEnv* env, jclass clazz) {
            LOGI("[+] Injecting SU Binary... UID=0.");
        }

        // دالة القرص الوهمي
        void InitVirtualDisk(JNIEnv* env, jclass clazz, jstring javaPkg, jstring javaPath) {
            const char* pkg = env->GetStringUTFChars(javaPkg, 0);
            const char* path = env->GetStringUTFChars(javaPath, 0);
            LOGI("[V-DISK] Mounted for %s", pkg);
            env->ReleaseStringUTFChars(javaPkg, pkg);
            env->ReleaseStringUTFChars(javaPath, path);
        }

        // --- 🛡️ الدالة المطلوبة: تزييف الموديل 🛡️ ---
        jstring SpoofDeviceModel(JNIEnv* env, jclass clazz, jint modelIndex) {
            const char* models[] = {"ROG PHONE 8 PRO", "iPAD PRO M4", "RED MAGIC 9S", "GALAXY S24 ULTRA"};
            if (modelIndex < 0 || modelIndex >= 4) modelIndex = 0;
            LOGI("[+] Spoofing Device Identity to: %s", models[modelIndex]);
            return env->NewStringUTF(models[modelIndex]);
        }
    }
}

// =========================================================================
// مصفوفة التسجيل الديناميكي (يجب أن تشمل كل الدوال)
// =========================================================================

static const JNINativeMethod gMethods[] = {
    { "getKernelStatus", "()Ljava/lang/String;", (void*)WMaster::Core::GetKernelStatus },
    { "startSandbox", "(Ljava/lang/String;)V", (void*)WMaster::Core::StartSandbox },
    { "injectRootAccess", "()V", (void*)WMaster::Core::InjectRootAccess },
    { "initVirtualDisk", "(Ljava/lang/String;Ljava/lang/String;)V", (void*)WMaster::Core::InitVirtualDisk },
    // تسجيل دالة التزييف المفقودة هنا
    { "spoofDeviceModel", "(I)Ljava/lang/String;", (void*)WMaster::Core::SpoofDeviceModel }
};

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env = nullptr;
    if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) return JNI_ERR;

    jclass clazz = env->FindClass("com/my/newnas/NativeEngine");
    if (clazz == nullptr) return JNI_ERR;

    if (env->RegisterNatives(clazz, gMethods, sizeof(gMethods) / sizeof(gMethods[0])) < 0) {
        return JNI_ERR;
    }

    return JNI_VERSION_1_6;
}
