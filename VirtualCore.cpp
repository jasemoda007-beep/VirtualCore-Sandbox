#include <jni.h>
#include <string>
#include <map>
#include <unistd.h>
#include <android/log.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <sys/stat.h>

// ⚠️ تأكد من إضافة مكتبة Dobby في مشروعك ليعمل هذا الهيدر
// #include "dobby.h" 

/**
 * ☠️ W-MASTER ULTIMATE V-CORE (V8.0)
 * نظام البصرة للاختطاف العميق (Dobby) والحقن الديناميكي (Frida)
 */

#define LOG_TAG "W_MASTER_PRO"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

std::map<std::string, std::string> vdisk_map;

// ==========================================
// 🛡️ قسم الاختطاف (Dobby Hooking) 🛡️
// ==========================================

// تعريف شكل دوال النظام الأصلية
typedef int (*orig_open_t)(const char *pathname, int flags, mode_t mode);
typedef int (*orig_openat_t)(int dirfd, const char *pathname, int flags, mode_t mode);

orig_open_t orig_open = nullptr;
orig_openat_t orig_openat = nullptr;

// 🎯 الدالة الفخ: كلما حاولت اللعبة فتح ملف، ستمر من هنا أولاً!
int my_open(const char *pathname, int flags, mode_t mode) {
    if (pathname != nullptr) {
        std::string path(pathname);
        
        // إذا اللعبة حاولت تفتح ملفات النظام أو الداتا الأصلية، نحولها!
        for (auto const& [original, sandbox] : vdisk_map) {
            if (path.find(original) == 0) {
                std::string new_path = sandbox + path.substr(original.length());
                LOGI("[DOBBY-HOOK] HIJACKED: %s -> %s", path.c_str(), new_path.c_str());
                return orig_open(new_path.c_str(), flags, mode);
            }
        }
    }
    return orig_open(pathname, flags, mode);
}

// ==========================================
// 💉 قسم الحقن الديناميكي (Frida Gadget) 💉
// ==========================================

void InjectFrida() {
    LOGI("[FRIDA] Attempting to load Frida Gadget...");
    // محاولة تحميل فريدا من مسار التطبيق
    void* handle = dlopen("libfrida-gadget.so", RTLD_NOW);
    if (handle) {
        LOGI("[FRIDA] Gadget Loaded Successfully! Listening for connections... 🚀");
    } else {
        LOGI("[FRIDA] Failed to load Gadget: %s", dlerror());
    }
}

namespace WMaster {
    namespace Core {

        void InitVirtualDisk(JNIEnv* env, jclass clazz, jstring javaPkg, jstring javaPath) {
            const char* pkg = env->GetStringUTFChars(javaPkg, nullptr);
            const char* path = env->GetStringUTFChars(javaPath, nullptr);

            std::string original = "/data/data/" + std::string(pkg);
            std::string sandbox = std::string(path);
            
            vdisk_map[original] = sandbox;
            mkdir(sandbox.c_str(), 0777); // إنشاء المجلد الوهمي
            
            LOGI("[V-DISK] Shield Active for: %s", pkg);

            env->ReleaseStringUTFChars(javaPkg, pkg);
            env->ReleaseStringUTFChars(javaPath, path);
        }

        void StartSandbox(JNIEnv* env, jclass clazz, jstring pkgName) {
            LOGI("[KERNEL] Igniting Dobby Hooks & Frida Engine...");
            
            // تشغيل فريدا
            InjectFrida();

            // تفعيل Dobby (اختطاف دوال النظام)
            // ⚠️ قم بإزالة التعليقات عندما تضيف ملفات Dobby للمشروع ⚠️
            /*
            void* open_ptr = dlsym(RTLD_DEFAULT, "open");
            DobbyHook(open_ptr, (void *)my_open, (void **)&orig_open);
            LOGI("[DOBBY] 'open' syscall hijacked!");
            */
            
            LOGI("[KERNEL] STEALTH MODE: MAXIMUM ☠️");
        }

        jstring GetKernelStatus(JNIEnv* env, jclass clazz) {
            return env->NewStringUTF("W-MASTER KERNEL: DOBBY & FRIDA ARMED ☢️");
        }
    }
}

static const JNINativeMethod gMethods[] = {
    { "getKernelStatus", "()Ljava/lang/String;", (void*)WMaster::Core::GetKernelStatus },
    { "startSandbox", "(Ljava/lang/String;)V", (void*)WMaster::Core::StartSandbox },
    { "initVirtualDisk", "(Ljava/lang/String;Ljava/lang/String;)V", (void*)WMaster::Core::InitVirtualDisk }
};

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env;
    if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) return JNI_ERR;

    jclass clazz = env->FindClass("com/wmaster/vcloner/NativeEngine");
    if (clazz == nullptr) return JNI_ERR;

    if (env->RegisterNatives(clazz, gMethods, sizeof(gMethods) / sizeof(gMethods[0])) != JNI_OK) return JNI_ERR;

    LOGI("[OK] W-MASTER WEAPONS LOADED! 🚀");
    return JNI_VERSION_1_6;
}
