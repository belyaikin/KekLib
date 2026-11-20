#version 330 core

in vec3 vColor;
in vec3 vNormal;
in vec3 vWorldPos;
in vec2 vUV;

out vec4 FragColor;

uniform vec3 uViewPos;
uniform float uTime;

const float Tau = 6.28318530718;

void main()
{
    vec2 centeredUV = (vUV - vec2(0.5)) * 2.0;
    float radius = length(centeredUV);
    float angle = atan(centeredUV.y, centeredUV.x);

    // The outer edge glows more strongly when viewed at a grazing angle.
    vec3 normal = normalize(vNormal);
    vec3 viewDir = normalize(uViewPos - vWorldPos);
    float fresnel = pow(1.0 - max(dot(normal, viewDir), 0.0), 2.0);
    float edge = smoothstep(0.62, 0.98, radius);

    // Two wave fronts travel around the disk in opposite directions.
    float ringA = 1.0 - smoothstep(0.025, 0.09,
        abs(fract(radius * 5.0 - uTime * 0.32) - 0.5));
    float ringB = 1.0 - smoothstep(0.025, 0.08,
        abs(fract(radius * 3.0 + uTime * 0.22) - 0.5));

    // A rotating set of short arcs gives the shield a readable energy pattern.
    float rotatedAngle = angle + uTime * 0.7;
    float arcBand = smoothstep(0.42, 0.58, sin(rotatedAngle * 6.0 + radius * 8.0));
    float arcWindow = smoothstep(0.28, 0.42, radius) * (1.0 - smoothstep(0.76, 0.9, radius));
    float arcs = arcBand * arcWindow;

    float pulse = 0.82 + 0.18 * sin(uTime * 3.5);
    vec3 color = vColor * (0.34 + 0.22 * pulse);
    color += vColor * (edge * (0.75 + fresnel * 1.8));
    color += vec3(0.55, 1.0, 0.75) * (ringA * 0.62 + ringB * 0.30 + arcs * 0.48);

    // Keep the center subdued so the animated outline remains easy to see.
    color *= mix(0.72, 1.0, smoothstep(0.0, 0.45, radius));
    FragColor = vec4(color, 1.0);
}
