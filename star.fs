#version 330 core
out vec4 FragColor;
// in float vPhase;
in float vFrequency;
uniform float time;  // 接收CPU传递的时间

void main() {
    // 圆形裁剪
    vec2 coord = gl_PointCoord * 2.0 - 1.0;
    float dist = length(coord);
    if (dist > 1.0) discard;

    // 计算闪烁强度（正弦波动）
    // float intensity = 0.5 + 0.5 * sin(time * vFrequency + vPhase); 
    float intensity = 0.5 + 0.5 * sin(time * vFrequency); 
    intensity *= exp(-dist * dist * 4.0); // 结合光晕效果

    FragColor = vec4(1.0, 1.0, 1.0, intensity);
}