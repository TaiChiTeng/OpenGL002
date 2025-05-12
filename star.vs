#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float aSize;
layout (location = 2) in float aPhase;
layout (location = 3) in float aFrequency;
//  (location = 2) in float aFrequency;

out float vPhase;
out float vFrequency;

void main() {
    gl_Position = vec4(aPos, 1.0);
    gl_PointSize = aSize;
    vPhase = aPhase;         // 传递相位
    vFrequency = aFrequency; // 传递频率
}