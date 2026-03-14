#include <jni.h>
#include <string>
#include <map>
#include <unistd.h>
#include <android/log.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <signal.h>

/**
 * ☠️ W-MASTER ULTIMATE V-CORE (V13 - DYNAMIC DOBBY ENGINE + ANTI-CHEAT BYPASS)
 * محرك البصرة للاختطاف العميق (نسخة منع الانتحار والحمايات)
 */

#define LOG_TAG "W_MASTER_PRO_NATIVE"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

std::string real_pkg_dir = "";
std::string fake_pkg_dir = "";

// ==========================================
// 🛡️ تعريف دوال Dobby الديناميكية (بدون ملفات Header)
// ==========================================
typedef int (*DobbyHook_t)(void *address, void *replace_call, void **origin_call);
DobbyHook_t DynamicDobbyHook = nullptr;

// مؤشرات الدوال الأصلية لملفات النظام
int (*orig_openat)(int dirfd, const char *pathname, int flags, mode_t mode) = nullptr;
int (*orig_faccessat)(int dirfd, const char *pathname, int mode, int flags) = nullptr;

// مؤشرات دوال الحماية (الانتحار)
void (*orig_exit)(int status) = nullptr;
void (*orig__exit)(int status) = nullptr;
int (*orig_kill)(pid_t pid, int sig) = nullptr;

// ==========================================
// ☠️ الجواسيس مالتنا (Hooked Functions)
// ==========================================

// 1. تزوير أمر فتح الملفات (openat)
int my_openat(int dirfd, const char *pathname, int flags, mode_t mode) {
    if (pathname != nullptr) {
        std::string path(pathname);
        if (!real_pkg_dir.empty() && path.find(real_pkg_dir) == 0) {
            std::string new_path = fake_pkg_dir + path.substr(real_pkg_dir.length());
            LOGI("[IO-REDIRECT] openat: %s -> %s", pathname, new_path.c_str());
            if(orig_openat) return orig_openat(dirfd, new_path.c_str(), flags, mode);
        }
        if (path.find("/Android/obb") != std::string::npos || path.find("/Android/data") != std::string::npos) {
            LOGI("[OBB-BYPASS] Accessing Original Game Data: %s", pathname);
        }
    }
    if(orig_openat) return orig_openat(dirfd, pathname, flags, mode);
    return -1;
}

// 2. تزوير فحص الملفات (faccessat)
int my_faccessat(int dirfd, const char *pathname, int mode, int flags) {
    if (pathname != nullptr) {
        std::string path(pathname);
        if (!real_pkg_dir.empty() && path.find(real_pkg_dir) == 0) {
            std::string new_path = fake_pkg_dir + path.substr(real_pkg_dir.length());
            LOGI("[IO-REDIRECT] faccessat: %s -> %s", pathname, new_path.c_str());
            if(orig_faccessat) return orig_faccessat(dirfd, new_path.c_str(), mode, flags);
        }
    }
    if(orig_faccessat) return orig_faccessat(dirfd, pathname, mode, flags);
    return -1;
}

// ==========================================
// 🛡️ درع W-MASTER لمنع انتحار اللعبة (Anti-Suicide)
// ==========================================

// 3. اعتراض دالة exit العادية
void my_exit(int status) {
    LOGE("💥 [ANTI-CHEAT] Blocked exit(%d) call! Tencent tried to suicide. WE SURVIVED!", status);
    // لا نستدعي orig_exit لكي لا تموت اللعبة
}

// 4. اعتراض دالة _exit الصامتة
void my__exit(int status) {
    LOGE("💥 [ANTI-CHEAT] Blocked _exit(%d) call! Target neutralized.", status);
}

// 5. اعتراض دالة kill (إجهاض محاولة قتل العملية)
int my_kill(pid_t pid, int sig) {
    if (pid == getpid()) {
        LOGE("💥 [ANTI-CHEAT] Blocked self-kill(%d, %d)! Target is caged.", pid, sig);
        return 0; // نكذب على الحماية ونخبرها أننا متنا بنجاح
    }
    if(orig_kill) return orig_kill(pid, sig);
    return -1;
}

// ==========================================
// 💉 قسم فريدا والجافا
// ==========================================
void InjectFrida() {
    void* handle = dlopen("libfrida-gadget.so", RTLD_NOW);
    if (handle) {
        LOGI("[FRIDA] Gadget Loaded! 🚀");
    } else {
        LOGE("[FRIDA] Error: Failed to load Frida gadget!");
    }
}

namespace WMaster {
    namespace Core {
        void InitVirtualDisk(JNIEnv* env, jclass clazz, jstring javaPkg, jstring javaPath) {
            const char* pkg = env->GetStringUTFChars(javaPkg, nullptr);
            const char* path = env->GetStringUTFChars(javaPath, nullptr);
            real_pkg_dir = "/data/data/" + std::string(pkg);
            fake_pkg_dir = std::string(path);
            mkdir(fake_pkg_dir.c_str(), 0777); 
            LOGI("[V-DISK] IO-Shield Active: %s", real_pkg_dir.c_str());
            env->ReleaseStringUTFChars(javaPkg, pkg);
            env->ReleaseStringUTFChars(javaPath, path);
        }
        void StartSandbox(JNIEnv* env, jclass clazz, jstring pkgName) {
            InjectFrida();
        }
        jstring GetKernelStatus(JNIEnv* env, jclass clazz) {
            return env->NewStringUTF("W-MASTER KERNEL: DYNAMIC DOBBY & ANTI-CHEAT ARMED ☢️");
        }
        jstring SpoofDeviceModel(JNIEnv* env, jclass clazz, jint index) {
            return env->NewStringUTF("ROG PHONE 8 PRO");
        }
    }
}

static const JNINativeMethod gMethods[] = {
    { "getKernelStatus", "()Ljava/lang/String;", (void*)WMaster::Core::GetKernelStatus },
    { "startSandbox", "(Ljava/lang/String;)V", (void*)WMaster::Core::StartSandbox },
    { "initVirtualDisk", "(Ljava/lang/String;Ljava/lang/String;)V", (void*)WMaster::Core::InitVirtualDisk },
    { "spoofDeviceModel", "(I)Ljava/lang/String;", (void*)WMaster::Core::SpoofDeviceModel }
};

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env;
    if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) return JNI_ERR;

    jclass clazz = env->FindClass("com/wmaster/vcloner/NativeEngine");
    if (clazz == nullptr) return JNI_ERR;
    if (env->RegisterNatives(clazz, gMethods, sizeof(gMethods) / sizeof(gMethods[0])) != JNI_OK) return JNI_ERR;

    // ☢️ السحر: تحميل Dobby من الهواء واختطاف الدوال ☢️
    void* dobby_handle = dlopen("libdobby.so", RTLD_NOW);
    if (dobby_handle) {
        DynamicDobbyHook = (DobbyHook_t)dlsym(dobby_handle, "DobbyHook");
        if (DynamicDobbyHook) {
            LOGI("[DOBBY] Engine Loaded Dynamically! Injecting Syscalls... 🥷");
            
            void* libc_handle = dlopen("libc.so", RTLD_NOW);
            if (libc_handle) {
                // دوال الملفات
                void* openat_ptr = dlsym(libc_handle, "openat");
                void* faccessat_ptr = dlsym(libc_handle, "faccessat");
                // دوال الانتحار (جديد)
                void* exit_ptr = dlsym(libc_handle, "exit");
                void* _exit_ptr = dlsym(libc_handle, "_exit");
                void* kill_ptr = dlsym(libc_handle, "kill");
                
                // زرع الهوكات
                DynamicDobbyHook(openat_ptr, (void*)my_openat, (void**)&orig_openat);
                DynamicDobbyHook(faccessat_ptr, (void*)my_faccessat, (void**)&orig_faccessat);
                DynamicDobbyHook(exit_ptr, (void*)my_exit, (void**)&orig_exit);
                DynamicDobbyHook(_exit_ptr, (void*)my__exit, (void**)&orig__exit);
                DynamicDobbyHook(kill_ptr, (void*)my_kill, (void**)&orig_kill);
                
                LOGI("[DOBBY] Syscall Routing Active! ANTI-CHEAT BLOCKED. 🎯");
            }
        }
    } else {
        LOGE("[DOBBY] Error: libdobby.so not found in jniLibs!");
    }

    return JNI_VERSION_1_6;
}
