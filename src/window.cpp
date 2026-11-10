#include "window.hpp"
#include <stdexcept>

Window::Window(const char* name, uint32_t w, uint32_t h) : _window_width(w), _window_height(h) {
    if(!glfwInit()){
        throw std::runtime_error("GLFW INIT FAILED!");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _window = glfwCreateWindow(_window_width, _window_height, name, nullptr, nullptr);

    if(!_window){
        glfwTerminate();
        throw std::runtime_error("WINDOW CREATION FAILED!");
    }

    glfwMakeContextCurrent(_window);
    glfwSetWindowUserPointer(_window, this);
    glfwSetFramebufferSizeCallback(_window, framebufferSizeCallback);
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xpos, double ypos){
        auto *instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if(instance->onMouseMove){
            instance->onMouseMove(static_cast<float>(xpos), static_cast<float>(ypos));
        }
    });

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        throw std::runtime_error("GLAD INIT FAILED!");
    }

    glfwSwapInterval(1);
}

Window::~Window(){
    if(_window){
        glfwDestroyWindow(_window);
    }
    glfwTerminate();
}

float Window::calculateDeltaTime(){
    float currentFrame = static_cast<float>(glfwGetTime());
    _deltaTime = currentFrame - _lastFrameTime;
    _lastFrameTime = currentFrame;
    return _deltaTime;
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height){
    auto *instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
    instance->_window_width = width;
    instance->_window_height = height;
    glViewport(0, 0, width, height);
}