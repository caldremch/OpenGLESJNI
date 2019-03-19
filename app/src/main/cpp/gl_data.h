//
// Created by Administrator on 2019/3/19.
//

#ifndef OPENGLESJNI_GL_DATA_H
#define OPENGLESJNI_GL_DATA_H


#include <GLES3/gl3.h>

static const char VERTEX_SHADER[] =
        "#version 300 es\n"
        "layout(location = 0) in vec4 vPosition;\n"
        "void main(){\n"
        "gl_Position = vPosition;\n"
        "}\n"
        ;

static const char FRAGMENT_SHADER[] =
        "#version 300 es\n"
        "precision mediump float;\n"
        "out vec4 fragColor;\n"
        "void main(){\n"
        "fragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n"
        ;

static const GLfloat VERTEX[] = {
        0.0f,0.5f,0.0f,
        -0.5f,-0.5f,0.0f,
        0.5f,-0.5f,0.0f

};


#endif //OPENGLESJNI_GL_DATA_H
