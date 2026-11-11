#include "window.hpp"
#include "renderer.hpp"
#include <iostream>
#include "camera.hpp"
#include <chunk_manager.hpp>

int main(){
    try{
        Window window("Voxel Renderer - GI", 1600, 900);
        Renderer renderer;
        renderer.init();
        renderer.setClearColor({0.0F, 0.2F, 0.3F, 1.0F});

        // Camera3D camera(glm::vec3(0.0F, 15.0F, 30.0F));
        Camera3D camera(glm::vec3(0.0F, 80.0F, 80.0F), glm::vec3(0.0F, 1.0F, 0.0F), -90.0F, -35.0F);
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

        Shader shader("shaders/voxel.vert", "shaders/voxel.frag");
        ChunkManager world;
        
        world.loadRegion(-1, 1, -1, 1);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        while(!window.shouldClose()){
            float dt = window.calculateDeltaTime();
            window.pollEvents();

            // INPUT SETUP
            GLFWwindow* nativeWin = window.getGLFWwindow();
            if (glfwGetKey(nativeWin, GLFW_KEY_W) == GLFW_PRESS)
                camera.processKeyboard(camera_movement::FORWARD, dt);
            if (glfwGetKey(nativeWin, GLFW_KEY_S) == GLFW_PRESS)
                camera.processKeyboard(camera_movement::BACKWARD, dt);
            if (glfwGetKey(nativeWin, GLFW_KEY_A) == GLFW_PRESS)
                camera.processKeyboard(camera_movement::LEFT, dt);
            if (glfwGetKey(nativeWin, GLFW_KEY_D) == GLFW_PRESS)
                camera.processKeyboard(camera_movement::RIGHT, dt);
            if (glfwGetKey(nativeWin, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(nativeWin, true);

            world.update();
            renderer.clear();

            shader.use();
            shader.setMat4("u_ViewProjection", camera.getViewProjMatrix());
            world.render(renderer, shader);

            window.swapBuffers();
        }
    }catch(const std::exception &e){
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}