#ifndef GREEDY_MESHING_HANDLER_HPP
#define GREEDY_MESHING_HANDLER_HPP

#include "chunk.hpp"
#include "voxel_data.hpp"

class GreedyMeshingHandler{
public:
    static MeshData generateMesh(const Chunk& chunk){
        MeshData mesh;

        for(int d = 0; d < 3; d++){
            int u = (d+1) %3;
            int v = (d+2) %3;

            glm::ivec3 x = { 0, 0, 0 };
            glm::ivec3 dir = { 0, 0, 0 };
            dir[d] = 1;
            /*
                d = 0 -> X
                d = 1 -> Y
                d = 2 -> Z
            */

            glm::vec3 uDir = { 0.0F, 0.0F, 0.0F }; uDir[u] = 1.0F;
            glm::vec3 vDir = { 0.0F, 0.0F, 0.0F }; vDir[v] = 1.0F;

            // 2D MAsk for storing visible face materials for the current mesh slice
            std::array<VoxelID, CHUNK_SIZE * CHUNK_SIZE> mask{};

            for(int backFace = 0; backFace <= 1; backFace++){
                glm::vec3 faceNormal = backFace ? -glm::vec3(dir) : glm::vec3(dir);

                for(x[d] = 0; x[d] < CHUNK_SIZE; x[d]++){
                    int maskIdx = 0;
                    for(x[v] = 0; x[v] < CHUNK_SIZE; x[v]++){
                        for(x[u] = 0; x[u] < CHUNK_SIZE; x[u]++){
                            VoxelID current = chunk.getVoxel(x.x, x.y, x.z);
                            VoxelID neighbour = chunk.getVoxel(
                                x.x + dir.x * (backFace ? -1 : 1),
                                x.y + dir.y * (backFace ? -1 : 1),
                                x.z + dir.z * (backFace ? -1 : 1)
                            );

                            bool isVisible = (current != 0) && (neighbour == 0);
                            mask[maskIdx++] = isVisible ? current : 0;
                        }
                    }

                    maskIdx = 0;
                    for(int j = 0; j < CHUNK_SIZE; j++){
                        for(int i = 0; i < CHUNK_SIZE;){
                            VoxelID type = mask[j * CHUNK_SIZE + i];
                            if(type == 0){
                                i++;
                                continue;
                            }

                            int width;
                            for(width = 1; i + width < CHUNK_SIZE && mask[j * CHUNK_SIZE + (i+width)] == type; width++) {}

                            int height;
                            bool canExtend = true;
                            for(height = 1; j + height < CHUNK_SIZE; height++){
                                for(int k = 0; k < width; k++){
                                    if(mask[(j + height) * CHUNK_SIZE + (i + k)] != type){
                                        canExtend = false;
                                        break;
                                    }
                                }
                                if(!canExtend) break;
                            }
                            
                            x[u] = i;
                            x[v] = j;

                            glm::vec3 basePos = glm::vec3(x);
                            if(!backFace)
                                basePos[d] += 1.0F;
                            
                            float wFloat = static_cast<float>(width);
                            float hFloat = static_cast<float>(height);

                            glm::vec3 p0, p1, p2, p3;
                            if(!backFace){
                                p0 = basePos;
                                p1 = basePos + uDir * wFloat;
                                p2 = basePos + uDir * wFloat + vDir * hFloat;
                                p3 = basePos + vDir * hFloat;
                            }else{
                                p0 = basePos;
                                p1 = basePos + vDir * hFloat;
                                p2 = basePos + uDir * wFloat + vDir * hFloat;
                                p3 = basePos + uDir * wFloat;
                            }
                            
                            uint32_t startIndex = static_cast<uint32_t>(mesh.vertices.size());
                            mesh.vertices.push_back({ p0, faceNormal, glm::vec2(0.0f,   0.0f  ), type });
                            mesh.vertices.push_back({ p1, faceNormal, glm::vec2(wFloat, 0.0f  ), type });
                            mesh.vertices.push_back({ p2, faceNormal, glm::vec2(wFloat, hFloat), type });
                            mesh.vertices.push_back({ p3, faceNormal, glm::vec2(0.0f,   hFloat), type });

                            mesh.indices.push_back(startIndex + 0);
                            mesh.indices.push_back(startIndex + 1);
                            mesh.indices.push_back(startIndex + 2);
                            mesh.indices.push_back(startIndex + 0);
                            mesh.indices.push_back(startIndex + 2);
                            mesh.indices.push_back(startIndex + 3);

                            for (int ny = 0; ny < height; ++ny) {
                                for (int nx = 0; nx < width; ++nx) {
                                    mask[(j + ny) * CHUNK_SIZE + (i + nx)] = 0;
                                }
                            }

                            i+= width;
                        }
                    }
                }
            }
        }
        return mesh;
    }
private:

};

#endif