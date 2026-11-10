#ifndef VOXEL_DATA_HPP
#define VOXEL_DATA_HPP

#include <vector>
#include <glm/glm.hpp>
#include <cstdint>

struct VoxelVertex{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;

    uint32_t materialID;
};

struct MeshData{
    std::vector<VoxelVertex> vertices;
    std::vector<uint32_t> indices;
};

#endif