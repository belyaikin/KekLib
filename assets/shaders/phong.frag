#version 330 core
in vec3 vColor;
in vec3 vNormal;
in vec3 vWorldPos;
in vec2 vUV;
out vec4 FragColor;
uniform bool uLightingEnabled;
uniform vec3 uLightPos;
uniform vec3 uLightColor;
uniform vec3 uViewPos;
uniform bool uHasTexture;
uniform sampler2D uTexture;
const float AmbientStrength = 0.15;
const float SpecularStrength = 0.5;
const float Shininess = 32.0;
void main()
{
    vec3 baseColor = vColor;
    if (uHasTexture)
    baseColor *= texture(uTexture, vUV).rgb;
    // No light in the scene: plain colors (keeps the old samples working).
    if (!uLightingEnabled)
    {
        FragColor = vec4(baseColor, 1.0);
        return;
    }
    vec3 normal = normalize(vNormal);
    vec3 lightDir = normalize(uLightPos - vWorldPos);
    vec3 viewDir = normalize(uViewPos - vWorldPos);
    vec3 ambient = AmbientStrength * uLightColor;
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * uLightColor;
    vec3 specular = vec3(0.0);
    if (diff > 0.0)
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), Shininess);
        specular = SpecularStrength * spec * uLightColor;
    }
    FragColor = vec4((ambient + diffuse + specular) * baseColor, 1.0);
}
