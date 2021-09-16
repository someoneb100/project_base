#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <iostream>
#include <rg/Camera.h>
#include <rg/Box.h>
#include <rg/Object.h>
#include <rg/Skybox.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void mouse_callback(GLFWwindow *window, double xpos, double ypos);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

void processInput(GLFWwindow *window);

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f,2.0f,5.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

inline glm::mat4 getPerspective(){
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
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Our Project", NULL, NULL);
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
        //utility things
        const auto x_axis = glm::vec3(1.0f, 0.0f, 0.0f);
        const auto y_axis = glm::vec3(0.0f, 1.0f, 0.0f);
        const auto z_axis = glm::vec3(0.0f, 0.0f, 1.0f);
        const auto identity = glm::mat4(1.0f);


        const std::vector<glm::vec3> boxPositions({
            glm::vec3(4.0f, 0.0f, 4.0f),
            glm::vec3(4.0f, 0.0f, -4.0f),
            glm::vec3(-4.0f, 0.0f, 4.0f),
            glm::vec3(-4.0f, 0.0f, -4.0f)
        });

        const std::vector<glm::vec3> pointLightPositions({
                glm::vec3( 2.0f,  2.0f,  2.0f),
                glm::vec3( 2.3f, -3.3f, -4.0f),
                glm::vec3(-4.0f,  2.0f, -6.0f),
                glm::vec3( 0.0f,  0.0f, -3.0f)
        });

        const auto rcPosition = glm::vec3(0.0f,3.0f,3.0f);

        //lights
        const DirectionalLight directionalLight(
                glm::vec3(-0.2f, -5.0f, -0.3f),
                glm::vec3(0.05f),
                glm::vec3(0.4f),
                glm::vec3(0.5f)
                );

        const SpotLight spotLight(
                glm::vec3(0.0f),
                glm::vec3(1.0f),
                glm::vec3(1.0f),
                1.0f, 0.09f, 0.032f,
                glm::cos(glm::radians(12.5f)),
                glm::cos(glm::radians(15.0f))
                );


        Object cyborg("resources/objects/cyborg/cyborg.obj");
        Object duck("resources/objects/duck/12248_Bird_v1_L2.obj");
        Object watertower("resources/objects/watertower/old_water_tower_OBJ.obj");
        Object::setDirectionalLight(directionalLight);
        Object::setSpotLight(spotLight);

        SimpleBox sb;
        sb.setDirectionalLight(directionalLight);
        sb.setSpotLight(spotLight);

        ComplexBox cb;
        cb.setDirectionalLight(directionalLight);
        cb.setSpotLight(spotLight);
        //lights

        LightCube lc;
        for(unsigned i = 0u; i != pointLightPositions.size(); ++i) {
            Object::setPointLight(i, lc);
            cb.setPointLight(i, lc);
            sb.setPointLight(i, lc);
        }

        LightCube lc_red(glm::vec3(1.0f, 0.0f, 0.0f));
        lc_red.setK(1.0f, 0.007f, 0.0002f);
        Object::setPointLight(pointLightPositions.size(), lc_red);
        cb.setPointLight(pointLightPositions.size(), lc_red);
        sb.setPointLight(pointLightPositions.size(), lc_red);


        // render loop
        // -----------
        while (!glfwWindowShouldClose(window)) {
            const float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            processInput(window);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            const auto perspective = getPerspective();
            const auto view = camera.GetViewMatrix();

            //basic lights
            lc.setProjectionView(perspective, view);
            for(unsigned i = 0u; i != pointLightPositions.size(); ++i){
                auto model = glm::rotate(identity, currentFrame, y_axis);
                model = glm::translate(model, pointLightPositions[i]);
                model = glm::scale(model, glm::vec3(0.2f));
                lc.render(model);
                auto newPosition = glm::vec3(model*glm::vec4(pointLightPositions[i],1.0f));
                cb.setPointLightPosition(i, newPosition);
                sb.setPointLightPosition(i, newPosition);
                Object::setPointLightPosition(i, newPosition);
            }


            {
                //red light
                lc_red.setProjectionView(perspective, view);

                auto model = glm::rotate(identity, currentFrame, x_axis);
                model = glm::translate(model, rcPosition);
                model = glm::scale(model, glm::vec3(0.05f));
                lc_red.render(model);
                const auto newPosition = glm::vec3(model * glm::vec4(rcPosition, 1.0f));
                cb.setPointLightPosition(4, newPosition);
                sb.setPointLightPosition(4, newPosition);
                Object::setPointLightPosition(4, newPosition);
            }


            //all models
            Object::setProjectionView(perspective, view);
            Object::setViewPos(camera.Position);
            Object::setSpotLightPosition(camera.Position, camera.Front);

            //cyborg
            cyborg.render(glm::scale(identity, glm::vec3(0.8f)));


            {
                //duck
                auto model = glm::rotate(identity, currentFrame / 2.13f, y_axis);
                model = glm::translate(model, glm::vec3(3.0f, 0.0f, 3.0f));
                model = glm::rotate(model, glm::radians(-90.0f), x_axis);
                model = glm::rotate(model, glm::radians(130.0f), z_axis);
                model = glm::scale(model, glm::vec3(0.02f));
                duck.render(model);
            }

            {
                //watertower
                auto model = glm::translate(identity, glm::vec3(-4.0f, 5.0f, -20.0f));
                model = glm::scale(model, glm::vec3(0.5f));
                watertower.render(model);
            }


            //boxes
            cb.setProjectionView(perspective, view);
            cb.setViewPos(camera.Position);
            cb.setSpotLightPosition(camera.Position, camera.Front);
            sb.setProjectionView(perspective, view);
            sb.setViewPos(camera.Position);
            sb.setSpotLightPosition(camera.Position, camera.Front);
            for(auto& box : boxPositions) {
                auto model = glm::translate(identity, box);
                sb.render(model);
                model = glm::translate(model, y_axis * 4.0f);
                cb.render(model);
            }


            //skybox
            Skybox::getSkybox().render(perspective, view);

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
        camera.ProcessKeyboard(ROLL_LEFT, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(ROLL_RIGHT, deltaTime);
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
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
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
