#version 330 core
out vec4 FragColor;

void main() {
    // 创建圆形点
    vec2 coord = gl_PointCoord * 2.0 - 1.0;
    float dist = length(coord);
    if (dist > 1.0) discard;
    
    // 带光晕效果
    float intensity = exp(-dist * dist * 4.0);
    FragColor = vec4(1.0, 1.0, 1.0, intensity);
}