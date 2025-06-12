#version 450

struct CubeData {
    mat4 model;
};

layout(set = 1, binding = 1) readonly buffer CubesBuffer {
    CubeData cubes[];
};

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragWorldPos;
layout(location = 2) in vec3 fragWorldNormal;

layout(location = 0) out vec4 outColor;

const vec3 lightDir = vec3(-0.96, -2.31, 1.68);
const int MAX_MODELS = 1024;

//bool rayCubeIntersection(vec3 rayOrig, vec3 rayDir, vec3 cubeMin, vec3 cubeMax, out float t) {
bool rayCubeIntersection(vec3 rayOrig, vec3 rayDir, vec3 cubeMin, vec3 cubeMax) {
    vec3 invDir = 1.0 / rayDir;
    vec3 t0 = (cubeMin - rayOrig) * invDir;
    vec3 t1 = (cubeMax - rayOrig) * invDir;
    vec3 tmin = min(t0, t1);
    vec3 tmax = max(t0, t1);

    float tNear = max(max(tmin.x, tmin.y), tmin.z);
    float tFar = min(min(tmax.x, tmax.y), tmax.z);
    
    if (tNear <= tFar && tFar > 0) {
        //t = tNear;
        return true;
    }

    return false;
}

void main() {
    vec3 rayOrig = fragWorldPos;
    vec3 rayDir = -normalize(lightDir);

    float shadow = 0.0;
    for (int i = 0; i < MAX_MODELS; i++) {
        if (cubes[i].model == mat4(0.0)) break;

        vec3 cubeMin = (cubes[i].model * vec4(-0.5, -0.5, -0.5, 1.0)).xyz;
        vec3 cubeMax = (cubes[i].model * vec4(0.5, 0.5, 0.5, 1.0)).xyz;
        bool intersect = rayCubeIntersection(rayOrig, rayDir, cubeMin, cubeMax);

        if (intersect) {
            shadow = 1.0;
            break;
        }
    }

    float diff = max(0, dot(normalize(fragWorldNormal), -normalize(lightDir)));

    float shadingPower = 0.15;
    float shadowPower = 0.2;
    float shading = clamp((1.0 - diff) * shadingPower + shadow * shadowPower, 0.0, 1.0);
    vec3 color = fragColor * (1.0 - shading);

	outColor = vec4(color, 1.0);
}