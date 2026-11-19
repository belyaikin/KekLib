#version 330 core

in vec3 vColor;
in vec3 vNormal;
in vec3 vWorldPos;
in vec2 vUV;

out vec4 FragColor;

uniform mat4 uModel;
uniform bool uLightingEnabled;
uniform vec3 uLightPos;
uniform vec3 uLightColor;
uniform vec3 uViewPos;
uniform float uTime;

float Hash21(vec2 p)
{
    p = fract(p * vec2(233.34, 851.73));
    p += dot(p, p + 23.45);
    return fract(p.x * p.y);
}

void main()
{
    vec3 normal = normalize(vNormal);
    vec3 absNorm = abs(normal);

    vec3 bCenter = uModel[3].xyz;
    vec3 bScale = vec3(length(uModel[0].xyz), length(uModel[1].xyz), length(uModel[2].xyz));
    bScale = max(bScale, vec3(0.01));

    vec3 localPos = vWorldPos - bCenter;

    vec2 seedData = vec2(
        bScale.x * 17.31 + bCenter.x * 5.43 + vColor.r * 11.0,
        bScale.y * 29.77 + vColor.g * 13.0 + vColor.b * 7.0
    );
    float buildingSeed = Hash21(seedData);

    vec3 baseColor = vColor;
    vec3 emission = vec3(0.0);
    float shininess = 16.0;
    float specStrength = 0.2;

    if (normal.y > 0.6)
    {
        // roof
        float edgeDistX = bScale.x * 0.5 - abs(localPos.x);
        float edgeDistZ = bScale.z * 0.5 - abs(localPos.z);
        float edgeDist = min(edgeDistX, edgeDistZ);

        if (edgeDist < 0.22)
        {
            baseColor = vColor * 0.85;
            shininess = 20.0;
            specStrength = 0.25;
        }
        else
        {
            baseColor = vec3(0.12, 0.13, 0.16);

            vec2 acCoord = localPos.xz;
            vec2 acCell = floor(acCoord / 1.6);
            vec2 acFract = abs(fract(acCoord / 1.6) - 0.5);
            float acHash = Hash21(acCell + vec2(buildingSeed * 10.0));

            if (acHash > 0.60 && max(acFract.x, acFract.y) < 0.32)
            {
                float grill = 0.5 + 0.5 * sin(localPos.x * 40.0);
                baseColor = mix(vec3(0.35, 0.38, 0.42), vec3(0.10, 0.11, 0.13), grill * 0.7);
                shininess = 32.0;
                specStrength = 0.4;
            }

            if (bScale.y > 9.5)
            {
                float beaconDist = length(localPos.xz);
                if (beaconDist < 0.22)
                {
                    float beaconFlash = step(0.4, sin(uTime * 4.5));
                    vec3 redGlow = vec3(1.2, 0.08, 0.05);
                    baseColor = mix(baseColor, redGlow, beaconFlash * smoothstep(0.22, 0.04, beaconDist));
                    emission = redGlow * beaconFlash * smoothstep(0.22, 0.02, beaconDist);
                }
            }
        }
    }
    else if (normal.y < -0.6)
    {
        baseColor = vColor * 0.5;
    }
    else
    {
        // walls
        float uCoord = (absNorm.x > absNorm.z) ? localPos.z : localPos.x;
        float wallHalfWidth = 0.5 * ((absNorm.x > absNorm.z) ? bScale.z : bScale.x);
        float distFromEdge = wallHalfWidth - abs(uCoord);

        float heightFromBottom = localPos.y + bScale.y * 0.5;
        float heightFromTop = bScale.y * 0.5 - localPos.y;

        baseColor = vColor;

        float groundAo = smoothstep(0.0, 3.5, heightFromBottom);
        baseColor *= (0.75 + 0.25 * groundAo);

        // top cornice
        if (heightFromTop < 0.32)
        {
            baseColor = vColor * 0.88;
            if (abs(heightFromTop - 0.20) < 0.035)
                baseColor *= 0.65;
        }
        // foundation
        else if (heightFromBottom < 1.3)
        {
            float foundationSplit = 1.3 - heightFromBottom;
            if (foundationSplit < 0.08)
            {
                baseColor = vec3(0.08, 0.08, 0.10);
            }
            else if (heightFromBottom > 0.25 && distFromEdge > 0.3)
            {
                float lobbyCol = fract((uCoord + wallHalfWidth) / 1.4);
                if (lobbyCol < 0.08 || lobbyCol > 0.92)
                {
                    baseColor = vec3(0.12, 0.13, 0.15);
                }
                else
                {
                    vec3 lobbyLight = vec3(1.0, 0.88, 0.65);
                    baseColor = lobbyLight * 0.7;
                    emission = lobbyLight * 0.6;
                    shininess = 64.0;
                    specStrength = 0.8;
                }
            }
            else
            {
                baseColor = mix(vColor * 0.5, vec3(0.14, 0.15, 0.18), 0.6);
            }
        }
        // corners of the building
        else if (distFromEdge < 0.28)
        {
            baseColor = vColor * 0.92;
            if (abs(distFromEdge - 0.28) < 0.03)
                baseColor *= 0.75;
        }
        // windows grid on upper floors
        else
        {
            float facadeY = heightFromBottom - 1.3;
            float floorH = 1.10;
            float colW = 0.88;

            int floorIdx = int(floor(facadeY / floorH));
            int colIdx = int(floor((uCoord + wallHalfWidth) / colW));

            float cellU = fract((uCoord + wallHalfWidth) / colW);
            float cellV = fract(facadeY / floorH);

            // window frame margins inside cell
            float winU0 = 0.18;
            float winU1 = 0.82;
            float winV0 = 0.20;
            float winV1 = 0.82;

            if (cellU >= winU0 && cellU <= winU1 && cellV >= winV0 && cellV <= winV1)
            {
                // inside window area
                float localWinU = (cellU - winU0) / (winU1 - winU0);
                float localWinV = (cellV - winV0) / (winV1 - winV0);

                float frameBorder = 0.06;
                if (localWinU < frameBorder || localWinU > (1.0 - frameBorder) ||
                    localWinV < frameBorder || localWinV > (1.0 - frameBorder) ||
                    abs(localWinU - 0.5) < 0.035)
                {
                    baseColor = vec3(0.08, 0.09, 0.11);
                    shininess = 32.0;
                    specStrength = 0.3;
                }
                else
                {
                    shininess = 64.0;
                    specStrength = 0.9;

                    vec2 winCoord = vec2(float(floorIdx) + buildingSeed * 47.0, float(colIdx) + buildingSeed * 91.0);
                    float winHash = Hash21(winCoord);
                    float winPalette = Hash21(winCoord * 1.618);

                    if (winHash < 0.42)
                    {
                        vec3 litColor;
                        if (winPalette < 0.35)
                            litColor = vec3(1.0, 0.82, 0.45);
                        else if (winPalette < 0.65)
                            litColor = vec3(1.0, 0.92, 0.70);
                        else if (winPalette < 0.85)
                            litColor = vec3(0.70, 0.88, 1.0);
                        else
                            litColor = vec3(1.0, 0.68, 0.35);

                        if (winPalette > 0.40)
                        {
                            float blindHeight = 0.35 + 0.45 * Hash21(winCoord * 3.14);
                            if (localWinV > (1.0 - blindHeight))
                            {
                                float slat = 0.65 + 0.35 * sin(localWinV * 65.0);
                                litColor *= slat * 0.45;
                            }
                        }

                        baseColor = litColor;
                        emission = litColor * 0.85;
                    }
                    else
                    {
                        vec3 darkGlass = vec3(0.05, 0.08, 0.13);
                        float interiorGrad = 0.85 + 0.3 * localWinV;
                        baseColor = darkGlass * interiorGrad;
                    }
                }
            }
            else
            {
                if (cellV < 0.06)
                    baseColor *= 0.72;
                else if (cellV < 0.18)
                    baseColor *= 0.85;

                if (cellU >= (winU0 - 0.03) && cellU <= (winU1 + 0.03) && cellV < winV0 && cellV > (winV0 - 0.05))
                    baseColor *= 0.70;
            }
        }
    }

    // lighting calculations
    vec3 finalColor;

    if (!uLightingEnabled)
    {
        float fakeLight = 0.72 + 0.28 * max(dot(normal, normalize(vec3(0.4, 0.8, 0.5))), 0.0);
        finalColor = baseColor * fakeLight + emission;
    }
    else
    {
        // Blinn-Phong lighting
        vec3 lightDir = normalize(uLightPos - vWorldPos);
        vec3 viewDir = normalize(uViewPos - vWorldPos);

        float ambientStrength = 0.22;
        vec3 ambient = ambientStrength * uLightColor;

        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = diff * uLightColor;

        vec3 specular = vec3(0.0);
        if (diff > 0.0)
        {
            vec3 halfDir = normalize(lightDir + viewDir);
            float spec = pow(max(dot(normal, halfDir), 0.0), shininess);
            specular = specStrength * spec * uLightColor;
        }

        finalColor = (ambient + diffuse) * baseColor + specular + emission;
    }

    // distance fog
    float dist = max(0.0, -vWorldPos.z - 15.0);
    float fog = clamp(dist / 95.0, 0.0, 0.88);
    vec3 skyFogColor = vec3(0.10, 0.20, 0.40);
    finalColor = mix(finalColor, skyFogColor, fog);

    FragColor = vec4(finalColor, 1.0);
}
