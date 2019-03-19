package com.caldremch.opengl.view

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.Log
import android.widget.Toast
import com.caldremch.opengl.jni.OpenGLMainJNI
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class TriangleView(context:Context):GLSurfaceView(context){


    init {
        setEGLConfigChooser(8,8,8,0,16,0)
        setEGLContextClientVersion(3)
        setRenderer(TriangleRender())
    }

    class TriangleRender:GLSurfaceView.Renderer{
        override fun onDrawFrame(gl: GL10?) {
            OpenGLMainJNI.setup()
        }

        override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
            OpenGLMainJNI.resize(width, height)
        }

        override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
          val isSucc =  OpenGLMainJNI.init();

            if (isSucc){
                Log.d("caldremch", "成功了")
            }else{
                Log.d("caldremch", "失败了")
            }

        }


    }

}