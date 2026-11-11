#include "renderer.hpp"

void Renderer::init(){
    enableDepthTest(true);
    enableCullFace(false);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

void Renderer::setClearColor(const glm::vec4& color){
    glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::clear() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::enableDepthTest(bool enable) const {
    if(enable) glEnable(GL_DEPTH_TEST);
    else glDisable(GL_DEPTH_TEST);
}

void Renderer::enableCullFace(bool enable) const {
    if(enable) glEnable(GL_CULL_FACE);
    else glDisable(GL_CULL_FACE);
}

void Renderer::drawIndexed(unsigned int vao, unsigned int indexCount) const {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Renderer::drawArrays(unsigned int vao, unsigned int vertexCount) const {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);
}