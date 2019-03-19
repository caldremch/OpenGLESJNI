#include <jni.h>
#include <string>
#include <GLES3/gl3.h>
#include "MyLog.h"

static const char *CLZ_NAME = "com/caldremch/opengl/jni/OpenGLMainJNI";

static jstring getStringFromJNI(JNIEnv *env, jclass, jobject jObj) {

    const char *str = "来自 jni 动态注册的 str";
    jstring jStr = env->NewStringUTF(str);
    return jStr;

}


static JNINativeMethod methods[] = {
        {
                "stringFromJNI", "()Ljava/lang/String;", (void *) getStringFromJNI
        }

};




//根据函数映射表注册函数
static int regNativeMethods(JNIEnv *env,
                            const char *className,
                            const JNINativeMethod *gMethods,
                            size_t num
) {

    //获取包名类
    jclass clz;
    clz = env->FindClass(className);
    if (clz == NULL) {
        log_error("Class not found", "");
        return JNI_ERR;
    }

    //注册以下函数
    for (int i = 0; i < num; ++i) {
        log_debug("RegisterNatives method -> %s(%s) | fun-> %lld", gMethods[i].name,
                  gMethods[i].signature, (INTPTR) gMethods[i].fnPtr);
    }

    //动态注册函数
    if (env->RegisterNatives(clz, gMethods, num) < 0) {
        log_error("RegisterNatives failed", "");
        return JNI_ERR;
    }

    //删除本地引用
    env->DeleteLocalRef(clz);

    return JNI_OK;
}


JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved) {

    JNIEnv *env;

    if (jvm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return JNI_ERR;
    }

    if (regNativeMethods(env, CLZ_NAME, methods, sizeof(methods) / sizeof(methods[0])) < 0) {
        return JNI_ERR;
    }


    return JNI_VERSION_1_4;
}
