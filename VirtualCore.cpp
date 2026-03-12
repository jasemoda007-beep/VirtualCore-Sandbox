#include <jni.h>
#include <string>
#include <map>
#include <unistd.h>
#include <android/log.h>

#define LOG_TAG "W_MASTER_STORAGE_CORE"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// خريطة تحويل المسارات الفيزيائية (القرص الوهمي)
std::map<std::string, std::string> vdisk_map;

namespace WMaster {
    namespace Core {

        // دالة تفعيل القرص الوهمي لتطبيق معين
        void InitVirtualDisk(JNIEnv* env, jclass clazz, jstring javaPkg, jstring javaPath) {
            const char* pkg = env->GetStringUTFChars(javaPkg, 0);
            const char* path = env->GetStringUTFChars(javaPath, 0);

            // تحويل مسارات البيانات الأصلية إلى مجلد الساندبوكس
            std::string original_path = "/data/data/" + std::string(pkg);
            std::string sandbox_path = std::string(path);

            vdisk_map[original_path] = sandbox_path;
            
            LOGI("[V-DISK] Mapping Mounted: %s -> %s", original_path.c_str(), sandbox_path.c_str());

            env->ReleaseStringUTFChars(javaPkg, pkg);
            env->ReleaseStringUTFChars(javaPath, path);
        }

        jstring GetKernelStatus(JNIEnv* env, jclass clazz) {
            return env->NewStringUTF("W-MASTER KERNEL: V-DISK ACTIVE ✅");
        }

        void StartSandbox(JNIEnv* env, jclass clazz, jstring pkgName) {
            LOGI("[+] Sandbox Engine Active.");
        }

        void InjectRootAccess(JNIEnv* env, jclass clazz) {
            LOGI("[+] Root Access Spoofed.");
        }

        jstring SpoofDeviceModel(JNIEnv* env, jclass clazz, jint index) {
            const char* models[] = {"ROG PHONE 8 PRO", "iPAD PRO M4", "RED MAGIC 9S", "S24 ULTRA"};
            return env->NewStringUTF(models[index % 4]);
        }
    }
}

// مصفوفة التسجيل الديناميكي
static const JNINativeMethod gMethods[] = {
    { "getKernelStatus", "()Ljava/lang/String;", (void*)WMaster::Core::GetKernelStatus },
    { "startSandbox", "(Ljava/lang/String;)V", (void*)WMaster::Core::StartSandbox },
    { "injectRootAccess", "()V", (void*)WMaster::Core::InjectRootAccess },
    { "initVirtualDisk", "(Ljava/lang/String;Ljava/lang/String;)V", (void*)WMaster::Core::InitVirtualDisk },
    { "spoofDeviceModel", "(I)Ljava/lang/String;", (void*)WMaster::Core::SpoofDeviceModel }
};

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env;
    if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) return JNI_ERR;
    jclass clazz = env->FindClass("com/my/newnas/NativeEngine");
    env->RegisterNatives(clazz, gMethods, sizeof(gMethods) / sizeof(gMethods[0]));
    return JNI_VERSION_1_6;
}
