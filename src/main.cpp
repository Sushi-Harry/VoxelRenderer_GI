#include "window.hpp"
#include "renderer.hpp"
#include <iostream>
#include "camera.hpp"

int main(){
    try{
        Window window("Voxel Renderer - GI", 1600, 900);
        Renderer renderer;
        renderer.init();
        renderer.setClearColor({0.0F, 0.2F, 0.3F, 1.0F});

        Camera3D camera(glm::vec3(0.0F, 15.0F, 30.0F));
        bool firstMouse = true;
        float lastX = window.getWidth() / 2.0F, lastY = window.getHeight() / 2.0F;
        window.onMouseMove = [&](float xpos, float ypos){
            if(firstMouse){
                lastX = xpos;
                lastY = ypos;
                firstMouse = false;
            }
            float xoffset = xpos - lastX;
            float yoffset = -(ypos - lastY);
            lastX = xpos;
            lastY = ypos;

            camera.processMouseMovement(xoffset, yoffset);
        };

        while(!window.shouldClose()){
            float dt = window.calculateDeltaTime();
            window.pollEvents();

            renderer.clear();
            window.swapBuffers();
        }
    }catch(const std::exception &e){
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}