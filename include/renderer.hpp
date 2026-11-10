#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

class Renderer{
public:
    Renderer() = default;

    void init();
    void setClearColor(const glm::vec4& color);
    void clear() const;

    void enableDepthTest(bool enable = true) const;
    void enableCullFace(bool enable = true) const;

    void drawIndexed(unsigned int vao, unsigned int indexCount) const;
    void drawArrays(unsigned int vao, unsigned int vertexCount) const;
};

#endif