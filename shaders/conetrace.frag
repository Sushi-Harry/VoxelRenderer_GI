#version 460 core

in vec3 WorldPos;
in vec3 Normal;
flat in uint MaterialID;

out vec4 FragColor;

layout (binding = 0) uniform sampler3D u_VoxelVolume;

vec3 getMaterialColor(uint id) {
    switch(id) {
        case 1u: return vec3(0.2, 0.8, 0.2); // Grass
        case 2u: return vec3(0.5, 0.35, 0.25); // Dirt
        case 3u: return vec3(0.5, 0.5, 0.5); // Stone
        default: return vec3(0.0);
    }
}

vec4 TraceCone(vec3 origin, vec3 direction, float aperture){
    vec4 accumulatedColor = vec4(0.0);

    float dist = 1.5;
    float maxDist = 64.0;

    while(dist < maxDist && accumulatedColor.a < 1.0){
        float diameter = 2.0 * dist * tan(aperture / 2.0);
        float mipLevel = log2(max(diameter, 1.0));
        vec3 samplePos = (origin + direction * dist) / 128.0;

        if(any(lessThan(samplePos, vec3(0.0))) || any(greaterThan(samplePos, vec3(1.0)))) break;
        vec4 voxelSample = textureLod(u_VoxelVolume, samplePos, mipLevel);

        float weight = (1.0 - accumulatedColor.a);
        accumulatedColor += weight * voxelSample;

        dist += diameter * 0.5;
    }
    return accumulatedColor;
}

void main(){
    vec3 N = normalize(Normal);
    vec3 baseColor = getMaterialColor(MaterialID);

    vec3 lightDir = normalize(vec3(0.4, 0.8, 0.5));
    float directDiff = max(dot(N, lightDir), 0.0);
    vec3 directLight = baseColor * directDiff;

    vec4 indirectGI = TraceCone(WorldPos, N, 1.0);
    vec3 finalColor = directLight + (baseColor * indirectGI.rgb * 1.5); // 1.5 is a boost multiplier\
    
    FragColor = vec4(finalColor, 1.0);
}