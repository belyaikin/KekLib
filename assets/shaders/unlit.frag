#version 330 core

in vec3 vColor;
in vec2 vUV;

out vec4 FragColor;

uniform bool uHasTexture;
uniform sampler2D uTexture;

void main()
{
    vec3 baseColor = vColor;
    if (uHasTexture)
        baseColor *= texture(uTexture, vUV).rgb;
    
    FragColor = vec4(baseColor, 1.0);
}