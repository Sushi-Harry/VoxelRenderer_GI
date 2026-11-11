#ifndef CHUNK_MANAGER_HPP
#define CHUNK_MANAGER_HPP

#include <unordered_map>
#include <memory>
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "chunk.hpp"
#include "chunk_mesh.hpp"
#include "shader.hpp"
#include "renderer.hpp"

class ChunkNode{
public:
    Chunk _chunk;
    ChunkMesh _mesh;

    explicit ChunkNode(glm::ivec3 pos) : _chunk(pos) {}
};

class ChunkManager{
public:
    ChunkManager() = default;
    ~ChunkManager() = default;
    ChunkManager(const ChunkManager&) = delete;
    ChunkManager& operator=(const ChunkManager&) = delete;

    VoxelID getVoxel_world(int wx, int wy, int wz) const;
    void setVoxel_world(int wx, int wy, int wz, VoxelID id);

    void loadChunk(int cx, int cy, int cz);
    void unloadChunk(int cx, int cy, int cz);
    bool hasChunk(glm::ivec3 chunkPos) const;

    void loadRegion(int minX, int maxX, int minZ, int maxZ);

    static glm::ivec3 worldToChunkPos(int wx, int wy, int wz);
    static glm::ivec3 worldToLocalPos(int wx, int wy, int wz);

    void update();
    void render(const Renderer& renderer, const Shader& shader) const;
    void upadteChunkStreaming(const glm::vec3& cameraPos, int renderDistance);

private:
    std::unordered_map<glm::ivec3, std::unique_ptr<ChunkNode>> _chunks;

    glm::ivec3 _lastCamChunk = glm::ivec3(-999, -999, -999);
};

#endif