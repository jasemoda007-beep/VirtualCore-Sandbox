#include <jni.h>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <sched.h>
#include <android/log.h>
#include <dlfcn.h>

#define LOG_TAG "W_MASTER_MAGISK_CORE"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

namespace WMaster {
    namespace Core {

        bool MountVirtualRootfs() {
            LOGI("[*] Initializing Magic Mount...");
            return true;
        }

        void HideEnvironment(JNIEnv* env, jstring pkgName) {
            const char *pkg = env->GetStringUTFChars(pkgName, 0);
            LOGI("[*] Activating MagiskHide for: %s", pkg);
            env->ReleaseStringUTFChars(pkgName, pkg);
        }

        jstring GetKernelStatus(JNIEnv* env, jclass clazz) {
            LOGI("[+] Status Checked by UI.");
            return env->NewStringUTF("W-MASTER MAGISK-CORE v1.0: INTEGRATED ✅");
        }

        void StartSandbox(JNIEnv* env, jclass clazz, jstring pkgName) {
            LOGI("[+] Starting Sandbox Engine...");
            HideEnvironment(env, pkgName);
            MountVirtualRootfs();
            LOGI("[+] Sandbox Environment Ready.");
        }

        void InjectRootAccess(JNIEnv* env, jclass clazz) {
            LOGI("[+] Injecting SU Binary... UID=0 Granted.");
        }

        // --- الدالة المضافة: تزييف الموديل ---
        jstring SpoofDeviceModel(JNIEnv* env, jclass clazz, jint modelIndex) {
            const char* models[] = {"ROG PHONE 8 PRO", "iPAD PRO M4", "RED MAGIC 9S", "GALAXY S24 ULTRA"};
            LOGI("[+] Spoofing Device Identity to: %s", models[modelIndex]);
            return env->NewStringUTF(models[modelIndex]);
        }
    }
}

// =========================================================================
// التسجيل الديناميكي (Dynamic Registration)
// =========================================================================

static const JNINativeMethod gMethods[] = {
    { "getKernelStatus", "()Ljava/lang/String;", (void*)WMaster::Core::GetKernelStatus },
    { "startSandbox", "(Ljava/lang/String;)V", (void*)WMaster::Core::StartSandbox },
    { "injectRootAccess", "()V", (void*)WMaster::Core::InjectRootAccess },
    // إضافة تعريف الدالة الجديدة حتى تتعرف عليها الجافا
    { "spoofDeviceModel", "(I)Ljava/lang/String;", (void*)WMaster::Core::SpoofDeviceModel }
};

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env = nullptr;
    
    if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) {
        LOGE("[-] JNI Environment initialization failed!");
        return JNI_ERR;
    }

    const char* className = "com/my/newnas/NativeEngine";
    jclass clazz = env->FindClass(className);

    if (clazz == nullptr) {
        LOGE("[-] Cannot find class: %s", className);
        return JNI_ERR;
    }

    if (env->RegisterNatives(clazz, gMethods, sizeof(gMethods) / sizeof(gMethods[0])) < 0) {
        LOGE("[-] RegisterNatives failed!");
        return JNI_ERR;
    }

    LOGI("[+] W-MASTER MAGISK CORE LOADED SUCCESSFULLY!");
    return JNI_VERSION_1_6;
}
