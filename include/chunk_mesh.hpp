#ifndef CHUNK_MESH_HPP
#define CHUNK_MESH_HPP

#include <glad/glad.h>
#include "voxel_data.hpp"

class ChunkMesh{
public:
    ChunkMesh() = default;
    ~ChunkMesh() { cleanUp(); }

    ChunkMesh(const ChunkMesh&) = delete;
    ChunkMesh& operator=(const ChunkMesh&) = delete;

    void upload(const MeshData& data){
        if(_vao == 0){
            glGenVertexArrays(1, &_vao);
            glGenBuffers(1, &_vbo);
            glGenBuffers(1, &_ebo);
        }

        _indexCount = static_cast<unsigned int>(data.indices.size());
        if(_indexCount == 0) return;

        glBindVertexArray(_vao);

            glBindBuffer(GL_ARRAY_BUFFER, _vbo);
            glBufferData(GL_ARRAY_BUFFER, data.vertices.size() * sizeof(VoxelVertex), data.vertices.data(), GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
            glBufferData(GL_ARRAY_BUFFER, data.indices.size() * sizeof(uint32_t), data.indices.data(), GL_STATIC_DRAW);

            // Position
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VoxelVertex), (void*)offsetof(VoxelVertex, position));
            // Normal
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VoxelVertex), (void*)offsetof(VoxelVertex, normal));
            // UV
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VoxelVertex), (void*)offsetof(VoxelVertex, uv));
            // Material ID
            glEnableVertexAttribArray(3);
            glVertexAttribIPointer(3, 1, GL_UNSIGNED_INT, sizeof(VoxelVertex), (void*)offsetof(VoxelVertex, materialID));
        glBindVertexArray(0);
    }

    void draw() const {
        if(_vao != 0 && _indexCount > 0) {
            glBindVertexArray(_vao);
            glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);
        }
    }

    void cleanUp(){
        if(_vao != 0){
            glDeleteVertexArrays(1, &_vao);
            glDeleteBuffers(1, &_vbo);
            glDeleteBuffers(1, &_ebo);
            _vao = 0; _vbo = 0; _ebo = 0;
            _indexCount = 0;
        }
    }

private:
    unsigned int _vao = 0, _vbo = 0, _ebo = 0;
    unsigned int _indexCount = 0;
};

#endif