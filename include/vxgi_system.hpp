#ifndef VXGI_SYSTEM_HPP
#define VXGI_SYSTEM_HPP

#include "texture3d.hpp"
#include "shader.hpp"
#include "chunk_manager.hpp"
#include "camera.hpp"
#include "renderer.hpp"
#include <memory>

class GISystem{
public:
    GISystem(int volumeResolution = 128){
        _voxelVolume = std::make_unique<Texture3D>(volumeResolution);

        _voxelizationShader = std::make_unique<Shader>("shaders/voxelize.vert", "shaders/voxelize.frag");
        _conetraceShader = std::make_unique<Shader>("shaders/conetrace.vert", "shaders/conetrace.frag");
    }

    void Execute(const Renderer& baseRenderer, const ChunkManager& world, const Camera3D& camera){
        // This is the first pass - Voxelization pass
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        _voxelizationShader->use();
        _voxelizationShader->setMat4("u_ViewProjection", camera.getViewProjMatrix());

        _voxelVolume->BindForWriting(0);
        world.render(baseRenderer, *_voxelizationShader);

        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        _voxelVolume->GenerateMipMaps();

        // This is the 2nd passs - Cone tracing pass
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        baseRenderer.clear();

        _conetraceShader->use();
        _conetraceShader->setMat4("u_ViewProjection", camera.getViewProjMatrix());

        _voxelVolume->BindForReaeding(0);
        _conetraceShader->setInt("u_VoxelVolume", 0);

        world.render(baseRenderer, *_conetraceShader);
    }

private:
    std::unique_ptr<Texture3D> _voxelVolume;
    std::unique_ptr<Shader> _voxelizationShader;
    std::unique_ptr<Shader> _conetraceShader;
};

#endif