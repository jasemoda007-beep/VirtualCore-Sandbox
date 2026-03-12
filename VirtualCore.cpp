#include <jni.h>
#include <string>
#include <map>
#include <unistd.h>
#include <android/log.h>

#define LOG_TAG "W_MASTER_STORAGE_CORE"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// خريطة لتخزين المسارات الوهمية
std::map<std::string, std::string> path_map;

namespace WMaster {
    namespace Storage {

        // دالة تحويل المسار (The Redirection Engine)
        std::string redirect_path(const char* original_path) {
            std::string path(original_path);
            for (auto const& [src, dst] : path_map) {
                if (path.find(src) == 0) {
                    std::string redirected = dst + path.substr(src.length());
                    LOGI("[V-DISK] Redirecting: %s -> %s", original_path, redirected.c_str());
                    return redirected;
                }
            }
            return path;
        }

        // إعداد القرص الوهمي لتطبيق معين
        void InitVirtualDisk(JNIEnv* env, jclass clazz, jstring javaPkg, jstring javaSandboxPath) {
            const char* pkg = env->GetStringUTFChars(javaPkg, 0);
            const char* sandbox = env->GetStringUTFChars(javaSandboxPath, 0);

            // تحويل مسارات البيانات الحقيقية إلى الساندبوكس
            std::string original_data = "/data/data/" + std::string(pkg);
            std::string original_user_data = "/data/user/0/" + std::string(pkg);
            
            path_map[original_data] = std::string(sandbox);
            path_map[original_user_data] = std::string(sandbox);

            LOGI("[V-DISK] Disk Mounted for %s at %s", pkg, sandbox);

            env->ReleaseStringUTFChars(javaPkg, pkg);
            env->ReleaseStringUTFChars(javaSandboxPath, sandbox);
        }

        jstring GetKernelStatus(JNIEnv* env, jclass clazz) {
            return env->NewStringUTF("W-MASTER KERNEL: V-DISK ACTIVE ✅");
        }
    }
}

static const JNINativeMethod gMethods[] = {
    { "getKernelStatus", "()Ljava/lang/String;", (void*)WMaster::Storage::GetKernelStatus },
    { "initVirtualDisk", "(Ljava/lang/String;Ljava/lang/String;)V", (void*)WMaster::Storage::InitVirtualDisk }
};

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env;
    if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) return JNI_ERR;

    jclass clazz = env->FindClass("com/my/newnas/NativeEngine");
    env->RegisterNatives(clazz, gMethods, sizeof(gMethods) / sizeof(gMethods[0]));

    return JNI_VERSION_1_6;
}
