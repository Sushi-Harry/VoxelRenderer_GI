#include "chunk_manager.hpp"
#include "greedy_meshing_handler.hpp"

constexpr int CHUNK_SHIFT = 5;
constexpr int CHUNK_MASK = CHUNK_SIZE - 1;

// Procedural heightmap generator
static int getTerrainHeight(int wx, int wz){
    float x = static_cast<float>(wx) * 0.05F;
    float z = static_cast<float>(wz) * 0.05F;

    float h = std::sin(x) * std::cos(z) * 4.0F + std::sin(x * 0.3F + z * 0.3F) * 8.0F;

    return static_cast<int>(h) + 10;
}

void ChunkManager::loadRegion(int minX, int maxX, int minZ, int maxZ){
    for(int cx = minX; cx < maxX; cx++){
        for(int cz = minZ; cz < maxZ; cz++){

            loadChunk(cx, 0, cz);

            auto it = _chunks.find(glm::ivec3(cx, 0, cz));
            if(it == _chunks.end()) continue;

            Chunk& chunk = it->second->_chunk;
            for(int z = 0; z < CHUNK_SIZE; z++){
                for(int x = 0; x < CHUNK_SIZE; x++){
                    int worldX = cx * CHUNK_SIZE + x;
                    int worldZ = cz * CHUNK_SIZE + z;
                    int height = getTerrainHeight(worldX, worldZ);

                    for(int y = 0; y < CHUNK_SIZE; y++){
                        if(y > height){
                            chunk.setVoxel(x, y, z, 0);
                        }else if (y == height) {
                            chunk.setVoxel(x, y, z, 1);
                        }else if (y >= height - 3) {
                            chunk.setVoxel(x, y, z, 2);
                        }else {
                            chunk.setVoxel(x, y, z, 3);
                        }
                    }
                }
            }
        }
    }
}

void ChunkManager::update(){
    for(auto &[pos, node] : _chunks){
        if(node->_chunk.isDirty()){
            MeshData mesh = GreedyMeshingHandler::generateMesh(node->_chunk);
            // std::cout << "Generated mesh for chunk with " << mesh.vertices.size() << " vertices.\n";
            node->_mesh.upload(mesh);
            node->_chunk.setClean();
        }
    }
}

void ChunkManager::loadChunk(int cx, int cy, int cz){
    glm::ivec3 pos(cx, cy, cz);

    if(_chunks.find(pos) != _chunks.end()){
        return;
    }

    _chunks[pos] = std::make_unique<ChunkNode>(pos);

    glm::ivec3 neighbours[6] = {
        pos + glm::ivec3(1, 0, 0), pos + glm::ivec3(-1, 0, 0),
        pos + glm::ivec3(0, 1, 0), pos + glm::ivec3(0, -1, 0),
        pos + glm::ivec3(0, 0, 1), pos + glm::ivec3(0, 0, -1)
    };

    for(const auto& nPos : neighbours){
        auto it = _chunks.find(nPos);
        if(it != _chunks.end()){
            it->second->_chunk.setDirty();
        }
    }
}

void ChunkManager::unloadChunk(int cx, int cy, int cz){
    glm::ivec3 pos(cx, cy, cz);

    _chunks.erase(pos);
}

bool ChunkManager::hasChunk(glm::ivec3 chunkPos) const{
    return _chunks.find(chunkPos) != _chunks.end();
}

void ChunkManager::render(const Renderer& renderer, const Shader& shader) const {
    for(const auto& [pos, node] : _chunks){
        glm::vec3 worldPos = glm::vec3(pos * CHUNK_SIZE);
        glm::mat4 model = glm::translate(glm::mat4(1.0F), worldPos);
        shader.setMat4("u_Model", model);
        node->_mesh.draw();
    }
}

glm::ivec3 ChunkManager::worldToChunkPos(int wx, int wy, int wz){
    return glm::ivec3(
        wx >> CHUNK_SHIFT,
        wy >> CHUNK_SHIFT,
        wz >> CHUNK_SHIFT
    );
}

glm::ivec3 ChunkManager::worldToLocalPos(int wx, int wy, int wz){
    return glm::ivec3(
        wx & CHUNK_MASK,
        wy & CHUNK_MASK,
        wz & CHUNK_MASK
    );
}

VoxelID ChunkManager::getVoxel_world(int wx, int wy, int wz) const {
    glm::ivec3 cPos = worldToChunkPos(wx, wy, wz);
    glm::ivec3 lPos = worldToLocalPos(wx, wy, wz);

    auto it = _chunks.find(cPos);
    if(it == _chunks.end()){
        return 0;
    }

    return it->second->_chunk.getVoxel(lPos.x, lPos.y, lPos.z);
}

void ChunkManager::setVoxel_world(int wx, int wy, int wz, VoxelID id){
    glm::ivec3 cPos = worldToChunkPos(wx, wy, wz);
    glm::ivec3 lPos = worldToLocalPos(wx, wy, wz);

    auto it = _chunks.find(cPos);
    if(it != _chunks.end()){
        it->second->_chunk.setVoxel(lPos.x, lPos.y, lPos.z, id);

        if(lPos.x == 0){
            auto n = _chunks.find(cPos + glm::ivec3(-1, 0, 0));
            if(n != _chunks.end())
                n->second->_chunk.setVoxel(CHUNK_SIZE, lPos.y, lPos.z, n->second->_chunk.getVoxel(CHUNK_SIZE - 1, lPos.y, lPos.z));
        }
        if(lPos.x == CHUNK_SIZE - 1){
            auto n = _chunks.find(cPos + glm::ivec3(1, 0, 0));
            if(n != _chunks.end())
                n->second->_chunk.setVoxel(-1, lPos.y, lPos.z, n->second->_chunk.getVoxel(0, lPos.y, lPos.z));
        }

        if(lPos.y == 0){
            auto n = _chunks.find(cPos + glm::ivec3(0, -1, 0));
            if(n != _chunks.end())
                n->second->_chunk.setVoxel(lPos.x, CHUNK_SIZE, lPos.z, n->second->_chunk.getVoxel(lPos.x, CHUNK_SIZE - 1, lPos.z));
        }
        if(lPos.y == CHUNK_SIZE - 1){
            auto n = _chunks.find(cPos + glm::ivec3(0, 1, 0));
            if(n != _chunks.end())
                n->second->_chunk.setVoxel(lPos.x, -1, lPos.z, n->second->_chunk.getVoxel(lPos.x, 0, lPos.z));
        }

        if(lPos.z == 0){
            auto n = _chunks.find(cPos + glm::ivec3(0, 0, -1));
            if(n != _chunks.end())
                n->second->_chunk.setVoxel(lPos.x, lPos.y, CHUNK_SIZE, n->second->_chunk.getVoxel(lPos.x, lPos.y, CHUNK_SIZE - 1));
        }
        if(lPos.z == CHUNK_SIZE - 1){
            auto n = _chunks.find(cPos + glm::ivec3(0, 0, 1));
            if(n != _chunks.end())
                n->second->_chunk.setVoxel(lPos.x, lPos.y, -1, n->second->_chunk.getVoxel(lPos.x, lPos.y, 0));
        }
    }
}