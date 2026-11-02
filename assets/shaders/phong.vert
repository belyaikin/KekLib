#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec2 aUV;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
out vec3 vColor;
out vec3 vNormal;
out vec3 vWorldPos;
out vec2 vUV;
void main()
{
    vec4 worldPos = uModel * vec4(aPosition, 1.0);
    vWorldPos = worldPos.xyz;
    // inverse-transpose keeps normals correct under non-uniform scale (like a 0.3 x 1 x 0.3 paddle)
    vNormal = mat3(transpose(inverse(uModel))) * aNormal;
    vColor = aColor;
    vUV = aUV;
    gl_Position = uProjection * uView * worldPos;
}