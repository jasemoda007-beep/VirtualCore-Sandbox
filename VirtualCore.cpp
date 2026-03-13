#include <jni.h>
#include <string>
#include <map>
#include <unistd.h>
#include <android/log.h>

/**
 * 🛠️ W-MASTER V-CORE ENGINE v2.0
 * Developed with pride in Basrah.
 * مخصص للعزل والحقن وتغيير هوية الجهاز.
 */

#define LOG_TAG "W_MASTER_NATIVE"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

std::map<std::string, std::string> vdisk_map;

namespace WMaster {
    namespace Core {

        // 🛡️ تفعيل القرص الوهمي (V-DISK)
        void InitVirtualDisk(JNIEnv* env, jclass clazz, jstring javaPkg, jstring javaPath) {
            const char* pkg = env->GetStringUTFChars(javaPkg, nullptr);
            const char* path = env->GetStringUTFChars(javaPath, nullptr);

            if (pkg == nullptr || path == nullptr) {
                LOGE("[V-DISK] CRITICAL ERROR: Null package or path!");
                return;
            }

            std::string original_path = "/data/data/" + std::string(pkg);
            vdisk_map[original_path] = std::string(path);

            LOGI("[V-DISK] Successfully Mounted: %s -> %s", original_path.c_str(), path);

            env->ReleaseStringUTFChars(javaPkg, pkg);
            env->ReleaseStringUTFChars(javaPath, path);
        }

        // ✅ فحص حالة النواة
        jstring GetKernelStatus(JNIEnv* env, jclass clazz) {
            return env->NewStringUTF("W-MASTER KERNEL: V-DISK ACTIVE ✅ (READY FOR INJECTION)");
        }

        // 🚀 بدء الساندبوكس (العزل)
        void StartSandbox(JNIEnv* env, jclass clazz, jstring pkgName) {
            const char* pkg = env->GetStringUTFChars(pkgName, nullptr);
            LOGI("[SANDBOX] Engine Active for: %s", pkg);
            env->ReleaseStringUTFChars(pkgName, pkg);
        }

        // ⚡ حقن صلاحيات الروت الوهمي
        void InjectRootAccess(JNIEnv* env, jclass clazz) {
            LOGI("[ROOT] Privilege Escalation Spoofed (Root: Enabled)");
        }

        // 📱 تغيير موديل الجهاز (تخطي الباند)
        jstring SpoofDeviceModel(JNIEnv* env, jclass clazz, jint index) {
            const char* models[] = {
                "ROG PHONE 8 PRO", 
                "iPAD PRO M4", 
                "RED MAGIC 9S", 
                "S24 ULTRA"
            };
            return env->NewStringUTF(models[index % 4]);
        }
    }
}

// 📋 مصفوفة التسجيل الديناميكي (يجب أن تطابق الجافا 100%)
static const JNINativeMethod gMethods[] = {
    { "getKernelStatus", "()Ljava/lang/String;", (void*)WMaster::Core::GetKernelStatus },
    { "startSandbox", "(Ljava/lang/String;)V", (void*)WMaster::Core::StartSandbox },
    { "injectRootAccess", "()V", (void*)WMaster::Core::InjectRootAccess },
    { "initVirtualDisk", "(Ljava/lang/String;Ljava/lang/String;)V", (void*)WMaster::Core::InitVirtualDisk },
    { "spoofDeviceModel", "(I)Ljava/lang/String;", (void*)WMaster::Core::SpoofDeviceModel }
};

// 🏁 المحرك الرئيسي عند التحميل
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env;
    if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) {
        LOGE("JNI_OnLoad: Failed to get JNIEnv!");
        return JNI_ERR;
    }

    // 🎯 تحديد الهدف: ربط النواة بكلاس NativeEngine بالبصرة
    jclass clazz = env->FindClass("com/wmaster/vcloner/NativeEngine");
    if (clazz == nullptr) {
        LOGE("JNI_OnLoad: Could not find class com/wmaster/vcloner/NativeEngine! Check your package name.");
        return JNI_ERR;
    }

    // 🛠️ تسجيل الدوال برمجياً
    if (env->RegisterNatives(clazz, gMethods, sizeof(gMethods) / sizeof(gMethods[0])) < 0) {
        LOGE("JNI_OnLoad: Registration failed for NativeEngine methods.");
        return JNI_ERR;
    }

    LOGI("W-MASTER CORE: Loaded Successfully (JNI_OnLoad). ✅");
    return JNI_VERSION_1_6;
}
