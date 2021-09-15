#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <learnopengl/camera.h>
#include <rg/Box.h>

#include <iostream>
#include <rg/Object.h>
#include <rg/Skybox.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void mouse_callback(GLFWwindow *window, double xpos, double ypos);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

void processInput(GLFWwindow *window);

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

glm::mat4 getPerspective(){
        return glm::perspective(glm::radians(camera.Zoom), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);
}
bool mouseInScreen = true;
int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    stbi_set_flip_vertically_on_load(false);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    {
        //TODO
        //backpack
        Object backpack("resources/objects/cyborg/cyborg.obj");
        backpack.setDirectionalLight(
                    glm::vec3(-0.2f, -1.0f, -0.3f),
                    glm::vec3(0.05f),
                    glm::vec3(0.4f),
                    glm::vec3(0.5f)
                );
        backpack.setSpotLight(
                glm::vec3(0.0f),
                glm::vec3(1.0f),
                glm::vec3(1.0f),
                1.0f, 0.09f, 0.032f,
                glm::cos(glm::radians(12.5f)),
                glm::cos(glm::radians(15.0f))
                );
        ComplexBox cb;
        cb.setDirectionalLight(
                glm::vec3(-0.2f, -1.0f, -0.3f),
                glm::vec3(0.05f),
                glm::vec3(0.4f),
                glm::vec3(0.5f)
        );
        cb.setSpotLight(
                glm::vec3(0.0f),
                glm::vec3(1.0f),
                glm::vec3(1.0f),
                1.0f, 0.09f, 0.032f,
                glm::cos(glm::radians(12.5f)),
                glm::cos(glm::radians(15.0f))
        );
        //lights
        glm::vec3 pointLightPositions[] = {
                glm::vec3( 0.7f,  0.2f,  2.0f),
                glm::vec3( 2.3f, -3.3f, -4.0f),
                glm::vec3(-4.0f,  2.0f, -12.0f),
                glm::vec3( 0.0f,  0.0f, -3.0f)
        };
        LightCube lc;
        for(unsigned i = 0u; i != 4u; ++i) {
            backpack.setPointLight(i, lc);
            backpack.setPointLightPosition(i, pointLightPositions[i]);
            cb.setPointLight(i, lc);
            cb.setPointLightPosition(i, pointLightPositions[i]);
        }

        // render loop
        // -----------
        while (!glfwWindowShouldClose(window)) {
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            processInput(window);

            glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


            //TODO

            //loading lights
            lc.setProjectionView(getPerspective(), camera.GetViewMatrix());
            for(unsigned i = 0u; i != 4u; ++i){
                glm::mat4 model(1.0f);
                model = glm::translate(model, pointLightPositions[i]);
                model = glm::scale(model, glm::vec3(0.2f));
                lc.render(model);
            }



            backpack.setProjectionView(getPerspective(),camera.GetViewMatrix());
            backpack.setViewPos(camera.Position);
            backpack.setSpotLightPosition(camera.Position, camera.Front);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::scale(model,glm::vec3(0.5));
            backpack.render(model);

            cb.setProjectionView(getPerspective(),camera.GetViewMatrix());
            cb.setViewPos(camera.Position);
            cb.setSpotLightPosition(camera.Position, camera.Front);
            glm::mat4 model2 = glm::mat4(1.0f);
            model2 = glm::translate(model2,glm::vec3(5.0));
            model2 = glm::scale(model2,glm::vec3(2.0));
            cb.render(model2);

            Skybox::getSkybox().render(getPerspective(),camera.GetViewMatrix());

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessRoll(ROLL_LEFT, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessRoll(ROLL_RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}



void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS){
        if(mouseInScreen) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        mouseInScreen = !mouseInScreen;
    }
}
