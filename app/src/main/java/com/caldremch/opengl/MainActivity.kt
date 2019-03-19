package com.caldremch.opengl

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import com.caldremch.opengl.jni.OpenGLMainJNI
import com.caldremch.opengl.view.TriangleView
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {


    lateinit var triangleView: TriangleView;

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
//        setContentView(R.layout.activity_main)

        // Example of a call to a native method
//        sample_text.text = OpenGLMainJNI.stringFromJNI();

//        var isSucc = OpenGLMainJNI.init()
//        Log.d("caldremch--->","是否已经初始化成功$isSucc ")


        triangleView = TriangleView(this);
        setContentView(triangleView)

    }



}
