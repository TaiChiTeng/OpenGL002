//2.4.shader.fs
#version 330 core
in vec3 vertexColor;  // 接收插值后的顶点颜色
out vec4 FragColor;

void main()
{
    // 计算当前片段到中心的距离（归一化到 [0,1]）
    vec2 coord = gl_FragCoord.xy / vec2(320, 568); // 标准化坐标
    vec2 center = vec2(0.5, 0.382); // 中心点
    float dist = distance(coord, center); // 距离 [0, 0.707]
    vec3 centerColor = vec3(0.612, 0.737, 0.820); // 中心色

    // 混合中心颜色和顶点插值颜色
    FragColor = vec4(mix(centerColor, vertexColor, dist * 1.414), 1.0);
}