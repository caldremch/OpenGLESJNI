package com.caldremch.opengl.jni

/**
 *
 * @author Caldremch
 *
 * @date 2019-03-19 16:59
 *
 * @email caldremch@163.com
 *
 * @describe
 *
 **/

 object OpenGLMainJNI{

    init {
        System.loadLibrary("native-lib")
    }


    //初始化本地 GLES
    external fun init():Boolean

    external fun stringFromJNI(): String

    external fun setup()

    external fun resize(width:Int, height:Int)

}