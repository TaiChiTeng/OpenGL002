//2.4.shader.vs
#version 330 core
layout (location = 0) in vec3 aPos;   // 顶点位置
layout (location = 1) in vec3 aColor; // 顶点颜色

out vec3 vertexColor;  // 传递给片段着色器的颜色

void main()
{
    gl_Position = vec4(aPos, 1.0);
    vertexColor = aColor;  // 直接传递顶点颜色
}