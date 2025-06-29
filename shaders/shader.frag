#version 450

struct CubeData {
    mat4 model;
    vec4 color;
};

layout(set = 1, binding = 1) readonly buffer CubesBuffer {
    CubeData cubes[];
};

layout(set = 2, binding = 2, rgba32f) uniform image2D prevFrame;
layout(set = 3, binding = 3) uniform sampler2D depthSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragWorldPos;
layout(location = 2) in vec3 fragWorldNormal;
layout(location = 3) in vec2 fragProjPos;
layout(location = 4) in float time;
layout(location = 5) in float frameCount;

layout(location = 0) out vec4 outColor;

const vec3 lightDir = vec3(-0.96, -2.31, 1.68);
const int MAX_MODELS = 1024;
const int MAX_DEPTH = 10;
const int SAMPLES = 2;

#define PI 3.1415926535

float random(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

vec2 random2D(vec4 co) {
    return vec2(
        random(co.xy), 
        random(co.zw));
}

vec3 random3D(vec4 co) {
    return vec3(
        random(co.xy), 
        random(co.yz), 
        random(co.zw));
}

vec3 randomHemispherePoint(vec2 rand) {
    float cosTheta = sqrt(1.0 - rand.x);
    float sinTheta = sqrt(rand.x);
    float phi = 2.0 * PI * rand.y;
    return vec3(
        cos(phi) * sinTheta,
        sin(phi) * sinTheta,
        cosTheta
    );
}

vec3 normalOrientedHemispherePoint(vec2 rand, vec3 n) {
    return randomHemispherePoint(rand);
}

vec3 sampleDiffuseDirection(vec3 point, vec3 normal, float add) {
    vec3 hemisphereDistributedDirection = normalOrientedHemispherePoint(random2D(vec4(point, normal.x) + add), normal);

    vec3 randomVec = normalize(2.0 * random3D(vec4(point, normal.x) + add) - 1.0);

    vec3 tangent = cross(randomVec, normal);
    vec3 bitangent = cross(normal, tangent);
    mat3 transform = mat3(tangent, bitangent, normal);

    return transform * hemisphereDistributedDirection;
}

vec3 computeCubeNormal(vec3 hitPoint, vec3 cubeMin, vec3 cubeMax) {
    vec3 center = (cubeMin + cubeMax) * 0.5;
    vec3 p = hitPoint - center;
    vec3 size = cubeMax - cubeMin;

    vec3 absDist = abs(p) - size * 0.5;
    float minDist = min(min(absDist.x, absDist.y), absDist.z);

    vec3 normal = vec3(0.0);
    if (minDist == absDist.x) normal = vec3(sign(p.x), 0.0, 0.0);
    else if (minDist == absDist.y) normal = vec3(0.0, sign(p.y), 0.0);
    else normal = vec3(0.0, 0.0, sign(p.z));

    return normalize(normal);
}

bool rayCubeIntersection(vec3 rayOrig, vec3 rayDir, vec3 cubeMin, vec3 cubeMax, out float t) {
    vec3 invDir = 1.0 / rayDir;
    vec3 t0 = (cubeMin - rayOrig) * invDir;
    vec3 t1 = (cubeMax - rayOrig) * invDir;
    vec3 tmin = min(t0, t1);
    vec3 tmax = max(t0, t1);

    float tNear = max(max(tmin.x, tmin.y), tmin.z);
    float tFar = min(min(tmax.x, tmax.y), tmax.z);
    
    if (tNear <= tFar && tFar > 0) {
        t = tNear;
        return true;
    }

    return false;
}

bool intersectScene(vec3 rayOrig, vec3 rayDir, out vec3 point, out vec3 normal, out vec3 color) {
    float minDist = 1000;
    bool intersect = false;
    
    for (int i = 0; i < MAX_MODELS; i++) {
        if (cubes[i].model == mat4(0.0)) break;
        
        vec3 cubeMin = (cubes[i].model * vec4(-0.5, -0.5, -0.5, 1.0)).xyz;
        vec3 cubeMax = (cubes[i].model * vec4(0.5, 0.5, 0.5, 1.0)).xyz;
        float t;

        if (rayCubeIntersection(rayOrig, rayDir, cubeMin, cubeMax, t) && t < minDist) {
            minDist = t;
            point = rayOrig + rayDir * t;
            normal = computeCubeNormal(point, cubeMin, cubeMax);
            color = cubes[i].color.rgb;
            intersect = true;
        }
    }
        
    return intersect;
}

bool intersectScene(vec3 rayOrig, vec3 rayDir) {
    vec3 p, n, c;
    bool intersect = intersectScene(rayOrig, rayDir, p, n, c);
    return intersect;
}

vec3 sampleCone(vec3 baseDir, float angle, vec2 rand) {
    float radius = tan(angle);
    
    float r = radius * sqrt(rand.x);
    float phi = 2.0 * PI * rand.y;
    vec2 diskPoint = r * vec2(cos(phi), sin(phi));
    
    vec3 tangent = normalize(cross(baseDir, vec3(0, 1, 1)));
    vec3 bitangent = cross(baseDir, tangent);
    
    return normalize(baseDir + tangent * diskPoint.x + bitangent * diskPoint.y);
}

vec3 calculateSoftDirectionalShadow(vec3 hitPoint, vec3 hitNormal, vec3 lightDir, float sunAngularRadius, int samples) {
    float shadow = 0.0;
    vec3 normal = hitNormal;
    
    for (int i = 0; i < samples; i++) {
        vec2 rand = random2D(vec4(hitPoint, i) * time);
        vec3 jitteredLightDir = sampleCone(lightDir, sunAngularRadius, rand);
        
        if (!intersectScene(hitPoint + normal * 0.001, jitteredLightDir)) {
            shadow += 1.0;
        }
    }
    
    shadow = shadow / float(samples);

    return vec3(shadow);
}

vec3 tracePath(vec3 rayOrig, vec3 rayDir) {
    vec3 result = vec3(0);
    vec3 throughput = vec3(1);

    for (int i = 0; i < MAX_DEPTH; i++) {
        vec3 hitPoint, hitNormal, hitColor;
        bool hit = intersectScene(rayOrig, rayDir, hitPoint, hitNormal, hitColor);

        if (!hit) {
            vec3 skyColor = vec3(1);
            result += throughput * skyColor;
            break;
        }


        if (i > 3) {
            float p = max(max(throughput.x, throughput.y), throughput.z);
            if (random(rayDir.xz) > p) break;
            throughput /= p;
        }

        rayOrig = hitPoint + hitNormal * 0.0001;
        rayDir = sampleDiffuseDirection(hitPoint, hitNormal, time);

        throughput *= hitColor;
    }

    return result;
}

void main() {
    ivec2 coord = ivec2(gl_FragCoord.xy);
    float storedDepth = texelFetch(depthSampler, coord, 0).r;
    float currentDepth = gl_FragCoord.z;

    if (currentDepth > storedDepth + 0.001) {
        discard;
    }

    vec3 rayOrig = fragWorldPos;
    vec3 rayDir = -normalize(lightDir);

    vec3 totalColor = vec3(0);
    for (int i = 0; i < SAMPLES; i++) {
        float sunAngularRadius = .1;
        vec3 directLight = calculateSoftDirectionalShadow(rayOrig, fragWorldNormal, -lightDir, sunAngularRadius, 1);
        directLight = mix(vec3(0.7), vec3(1.0), directLight);
        
        vec3 newRay = sampleDiffuseDirection(fragWorldPos, fragWorldNormal, i + time);
        vec3 indirectLight = tracePath(fragWorldPos + normalize(fragWorldNormal) * 0.001, newRay);
        
        totalColor += clamp(fragColor * directLight * indirectLight, vec3(0), vec3(1));
    }
    totalColor /= float(SAMPLES);
    
    ivec2 pixelCoords = ivec2(gl_FragCoord.xy);
    vec3 color = totalColor;

    if (frameCount == 0) {
        imageStore(prevFrame, pixelCoords, vec4(totalColor, 1.0));
    } else {
        vec3 accum = imageLoad(prevFrame, pixelCoords).rgb;
        vec3 newValue = mix(accum, totalColor, 1.0 / (frameCount + 1));
        imageStore(prevFrame, pixelCoords, vec4(newValue, 1.0));
        color = newValue;
    }

	outColor = vec4(color, 1.0);
}