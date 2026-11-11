#version 460 core

layout (binding = 0, rgba8) uniform writeonly image3D u_VoxelVolume;

in vec3 WorldPos;
flat in uint MaterialID;

vec3 getMaterialColor(uint id) {
    switch(id) {
        case 1u: return vec3(0.2, 0.8, 0.2); // Grass
        case 2u: return vec3(0.5, 0.35, 0.25); // Dirt
        case 3u: return vec3(0.5, 0.5, 0.5); // Stone
        default: return vec3(0.0);
    }
}

void main() {
    // Assuming your GI volume sits at world origin (0,0,0) and is 128x128x128
    ivec3 voxelCoord = ivec3(WorldPos);
    
    // Bounds check
    if (voxelCoord.x >= 0 && voxelCoord.x < 128 &&
        voxelCoord.y >= 0 && voxelCoord.y < 128 &&
        voxelCoord.z >= 0 && voxelCoord.z < 128) {
        vec3 color = getMaterialColor(MaterialID);
        imageStore(u_VoxelVolume, voxelCoord, vec4(color, 1.0));
    }
}