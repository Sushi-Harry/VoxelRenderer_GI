#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <array>
#include <vector>
#include <cstdint>
#include <glm/glm.hpp>

constexpr int CHUNK_SIZE = 32;
constexpr int CHUNK_VOL = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;
using VoxelID = uint8_t; // Saving space for each chunk by using an 8 bit integer. Allows for 256 possible voxel types

class Chunk{
public:
    Chunk(glm::ivec3& chunk_pos) : _chunkPosition(chunk_pos) { _voxels.fill(0); }
    ~Chunk() = default;

    static inline constexpr int getIndex(int x, int y, int z) { return x + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_SIZE); }
    VoxelID getVoxel(int x, int y, int z) const {
        if(x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_SIZE || z < 0 || z >= CHUNK_SIZE){
            return 0;
        }
        return _voxels[getIndex(x, y, z)];
    }

    void setVoxel(int x, int y, int z, VoxelID id){
        if (x >= 0 && x < CHUNK_SIZE && y >= 0 && y < CHUNK_SIZE && z >= 0 && z < CHUNK_SIZE) {
            _voxels[getIndex(x, y, z)] = id;
            _isDirty = true;
        }
    }

    bool isDirty() const { return _isDirty; }
    void setClean() { _isDirty = false; }
    void setDirty() { _isDirty = true; }
    glm::ivec3 getPosition() const { return _chunkPosition; }
    const std::array<VoxelID, CHUNK_VOL>& getVoxelData() const { return _voxels; }

private:
    glm::ivec3 _chunkPosition;
    std::array<VoxelID, CHUNK_VOL> _voxels;
    bool _isDirty = true;
};

#endif