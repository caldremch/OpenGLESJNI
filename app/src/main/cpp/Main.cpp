#include <jni.h>
#include <string>
#include <GLES3/gl3.h>
#include "MyLog.h"
#include "gl_data.h"
#include <cstdlib>

static const char *CLZ_NAME = "com/caldremch/opengl/jni/OpenGLMainJNI";

static jstring getStringFromJNI(JNIEnv *env, jclass, jobject jObj) {

    const char *str = "来自 jni 动态注册的 str";
    jstring jStr = env->NewStringUTF(str);
    return jStr;

}


/**
 *
 * @param funName 方法名
 * @return true: 错误
 */
bool checkGLError(const char *funName) {

    GLint err = glGetError();

    //GL_NO_ERROR : 当前无错误值
    if (err != GL_NO_ERROR) {
        log_error("GL error in %s(): %08x\n", funName, err);
        return true;
    }

    return false;

}


/**
 * 创建着色器
 * @param shaderType 指定要创建的着色器的类型。 只能是GL_VERTEX_SHADER或GL_FRAGMENT_SHADER。
 * @param src
 * @return
 */
GLuint createShader(GLenum shaderType, const char *src) {

    //着色器创建成功,将会返回一个非0的数值
    GLuint shader = glCreateShader(shaderType);

    if (!shader) {
        checkGLError("createShader");
        return 0;
    }


    // 从着色器对象返回源代码字符串
    glShaderSource(shader, 1, &src, NULL);
    log_debug("createShader::src--> %s", src);
    GLint compiled = GL_FALSE;
    glCompileShader(shader);
    //从着色器对象返回一个参数
    /**
     *   指定着色器对象的参数。 可接受的符号名称为GL_SHADER_TYPE，GL_DELETE_STATUS，
     *   GL_COMPILE_STATUS，GL_INFO_LOG_LENGTH，GL_SHADER_SOURCE_LENGTH
     *
     *   GL_COMPILE_STATUS:对于支持着色器编译器的实现，如果着色器上的最后一次编译操作成功
     *   compiled返回true
     *
     *   GL_INFO_LOG_LENGTH:
     *
     *   对于支持着色器编译器的实现，params返回着色器信息日志的字符数，包括空终止字符（即，存储信息日志所需的字符缓冲区的大小）。 如果着色器没有信息日志，则返回值0。
     *
     */
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {
        GLint logInfoLength = 0;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logInfoLength);
        if (logInfoLength > 0) {
            GLchar *logInfo = (GLchar *) (malloc(logInfoLength));
            //将log输出到logInfo
            glGetShaderInfoLog(shader, logInfoLength, NULL, logInfo);
            log_debug("不能compile %s shader:\n%s\n", shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment", logInfo);
            free(logInfo);
        }

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}


/**
 *  创建程序
 * @param vtxSrc 顶点
 * @param fragSrc 片元
 * @return
 */
GLuint createProgram(const char *vtxSrc, const char *fragSrc) {
    //todo 为什么一个int类型能 作为program的标识
    GLuint vtxShader = 0;
    GLuint fragShader = 0;
    GLuint program = 0;
    GLint linked = GL_FALSE;

    vtxShader = createShader(GL_VERTEX_SHADER, vtxSrc);
    if (!vtxShader) {
        goto exit;
    }

    fragShader = createShader(GL_FRAGMENT_SHADER, fragSrc);
    if (!fragShader) {
        goto exit;

    }

    program = glCreateProgram();
    if (!program) {
        checkGLError("glCreateProgram");
        goto exit;
    }

    glAttachShader(program, vtxShader);
    glAttachShader(program, fragShader);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {

        log_error("不能link program%s", "");
        GLint logInfoLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logInfoLength);
        if (logInfoLength > 0) {
            GLchar *logInfo = (GLchar *) (malloc(logInfoLength));
            //将log输出到logInfo
            glGetProgramInfoLog(program, logInfoLength, NULL, logInfo);
            log_debug("不能link program:\n%s\n", logInfo);
            free(logInfo);
        }

        /**
         * 释放内存并使与着色器指定的着色器对象关联的ID无效。 这个命令有效地撤消了对glCreateProgram的调用的影响。
         */
        glDeleteProgram(program);
        program = 0;
    }


    exit:
    glDeleteShader(vtxShader);
    glDeleteShader(fragShader);
    return program;
}

GLuint program;

jboolean J_init(JNIEnv *env, jobject obj) {

    program = createProgram(VERTEX_SHADER, FRAGMENT_SHADER);
    if (!program) {
        log_error("程序创建失败了%s", "");
        return JNI_FALSE;
    }
    glClearColor(0, 0, 0, 0);
    return JNI_TRUE;
}


void J_resize(JNIEnv *env, jobject obj, int width, int height) {
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

}

void J_setup(JNIEnv *env, jobject obj) {
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, VERTEX);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 3);

}

static JNINativeMethod methods[] = {
        {
                "stringFromJNI", "()Ljava/lang/String;", (void *) getStringFromJNI
        },
        {
                "init",          "()Z",                  (void *) J_init
        },
        {
                "setup",         "()V",                  (void *) J_setup
        },
        {
                "resize",        "(II)V",               (void *) J_resize
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
        log_error("Class not found%s", "");
        return JNI_ERR;
    }

    //注册以下函数
    for (int i = 0; i < num; ++i) {
//        log_debug("RegisterNatives method -> %s(%s) | fun-> %lld", gMethods[i].name,
//                  gMethods[i].signature, (INTPTR) gMethods[i].fnPtr);

        log_debug("RegisterNatives method -> %s(%s)", gMethods[i].name,
                  gMethods[i].signature);
    }

    //动态注册函数
    if (env->RegisterNatives(clz, gMethods, num) < 0) {
        log_error("RegisterNatives failed%s", "");
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
