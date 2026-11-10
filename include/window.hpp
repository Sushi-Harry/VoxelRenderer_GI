#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <string>
#include <cstdint>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <functional>

using namespace std;

class Window{
public:
    Window(const char* name = "WINDOW_TITLE", uint32_t w = 1600, uint32_t h = 900);
    ~Window();

    // This is to prevent copying
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    float calculateDeltaTime();
    void pollEvents() { glfwPollEvents(); }
    void swapBuffers() { glfwSwapBuffers(_window); }
    
    float getDeltaTime() const { return _deltaTime; }
    bool shouldClose() const { return glfwWindowShouldClose(_window); }
    uint32_t getWidth() const { return _window_width; }
    uint32_t getHeight() const { return _window_height; }
    GLFWwindow* getGLFWwindow() const { return _window; }

    std::function<void(float xpos, float ypos)> onMouseMove;

private:
    GLFWwindow* _window = nullptr;
    uint32_t _window_width;
    uint32_t _window_height;

    float _deltaTime = 0.0F;
    float _lastFrameTime = 0.0F;

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
};

#endif