#version 330 core

in vec3 vColor;
in vec3 vNormal;
in vec3 vWorldPos;
in vec2 vUV;

out vec4 FragColor;

uniform vec3 uViewPos;
uniform float uTime;

void main()
{
    vec3 normal = normalize(vNormal);
    vec3 viewDir = normalize(uViewPos - vWorldPos);

    float rim = pow(1.0 - max(dot(normal, viewDir), 0.0), 2.5);

    float pulse = 0.5 + 0.5 * sin(uTime * 4.0);

    float radius = length(vUV - vec2(0.5)) * 2.0;
    float rings = smoothstep(0.8, 1.0, 0.5 + 0.5 * sin(radius * 9.0 - uTime * 6.0));

    float sweep = smoothstep(0.9, 1.0, 0.5 + 0.5 * sin((vUV.x + vUV.y) * 8.0 - uTime * 5.0));

    vec3 hot = mix(vColor, vec3(1.0), 0.65);

    vec3 color = vColor * (0.6 + 0.4 * pulse);
    color += hot * (rim * 1.3 + rings * 0.35 + sweep * 0.5);

    FragColor = vec4(color, 1.0);
}