//2.5.shader.fs, 搭配2.4.shader.vs使用就行
#version 330 core
in vec3 vertexColor;
out vec4 FragColor;

// 伪随机函数（Hash函数）
float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}

void main()
{
    // 基础参数
    vec2 coord = gl_FragCoord.xy / vec2(320.0, 568.0);
    vec3 centerColor = vec3(0.612, 0.737, 0.820);
    
    // 生成三个随机渐变层
    vec3 finalColor = vec3(0.0);
    for(int i=0; i<3; i++){
        // 随机中心点偏移
        // (noise - 0.5)*0.3  // 控制中心点随机范围
        // random()*1.5+0.5    // 控制缩放系数范围
        // smoothstep(0.2,1.0) // 控制渐变过渡区间
        vec2 noise = vec2(random(vec2(i,0)), random(vec2(i,1)));
        vec2 center = vec2(0.5, 0.382) + (noise - 0.5)*0.3;
        
        // 随机缩放系数
        float scale = random(vec2(i,2)) * 1.5 + 0.5;
        
        // 计算距离并混合
        float dist = distance(coord, center) * scale;
        finalColor += mix(centerColor, vertexColor, smoothstep(0.2, 1.0, dist));
    }
    finalColor /= 3.0;

    FragColor = vec4(finalColor, 1.0);
}