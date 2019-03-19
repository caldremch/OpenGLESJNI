//指定版本号
#version 300 es

//设置输入变量的的位置值
layout (location = 0) in vec3 aPos;

//out输出字段, 传递给片元着色器的颜色
out vec4 color;

void main(){

    //gl_Position(内置函数) 赋值位置
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);


    color = vec4(0.5, 0.0, 0.0, 1.0);

}