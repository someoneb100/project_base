//
// Created by miwica on 14.9.21..
//

#ifndef PROJECT_BASE_OBJECT_H
#define PROJECT_BASE_OBJECT_H

#include <rg/model.h>
#include <rg/Shader.h>


class Object {
    static Shader* objectShader;
    static unsigned object_count;
    Model object;
    float shininess;
public:
    Object(std::string path, float shininess = 32.0f)
        : object(path), shininess(shininess){
        object.SetTextureNamePrefix("material.");
        if(object_count == 0u) objectShader = new Shader("resources/shaders/model/1.model_loading.vs", "resources/shaders/model/1.model_loading.fs");
        ++object_count;
    }

    ~Object(){
        --object_count;
        if(object_count == 0u){
            objectShader->deleteProgram();
            delete objectShader;
        }
    }

    void setView(const glm::mat4& view){
        objectShader->use();
        objectShader->setMat4("view", view);
    }
    void setProjection(const glm::mat4& projection){
        objectShader->use();
        objectShader->setMat4("projection", projection);
    }
    void setProjectionView(const glm::mat4& projection, const glm::mat4& view){
        objectShader->use();
        objectShader->setMat4("view", view);
        objectShader->setMat4("projection", projection);
    }
    void render(const glm::mat4& model){
        objectShader->use();
        objectShader->setMat4("model", model);
        objectShader->setFloat("material.shininess", shininess);
        object.Draw(*objectShader);
    }

    void setViewPos(const glm::vec3& viewPos){
        objectShader->use();
        objectShader->setVec3("viewPos", viewPos);
    }

    void setDirectionalLight(const glm::vec3& direction, const glm::vec3& ambient,
                             const glm::vec3& diffuse, const glm::vec3& specular){
        std::string prefix("dirLight.");
        objectShader->use();
        objectShader->setVec3(prefix + "direction", direction);
        objectShader->setVec3(prefix + "ambient", ambient);
        objectShader->setVec3(prefix + "diffuse", diffuse);
        objectShader->setVec3(prefix + "specular", specular);
    }

    void setPointLight(unsigned int num, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
                       float constant, float linear, float quadratic) {
        std::string prefix = "pointLights[" + std::to_string(num) + "].";
        objectShader->use();
        objectShader->setVec3(prefix + "ambient", ambient);
        objectShader->setVec3(prefix + "diffuse", diffuse);
        objectShader->setVec3(prefix + "specular", specular);
        objectShader->setFloat(prefix + "constant", constant);
        objectShader->setFloat(prefix + "linear", linear);
        objectShader->setFloat(prefix + "quadratic", quadratic);
    }

    void setPointLight(unsigned int num, const LightCube& light){
        setPointLight(num, light.getLightColor() * light.getAmbient()
                , light.getLightColor() * light.getDiffuse()
                , light.getLightColor() * light.getSpecular()
                , light.getConstant(), light.getLinear(), light.getQuadratic());
    }

    void setPointLightPosition(unsigned int num, const glm::vec3& position){
        std::string prefix = "pointLights[" + std::to_string(num) + "].";
        objectShader->use();
        objectShader->setVec3(prefix + "position", position);
    }

    void setSpotLight(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
                      float constant, float linear, float quadratic,
                      float cutOff, float outerCutOff){
        std::string prefix("spotLight.");
        objectShader->use();
        objectShader->setVec3(prefix + "ambient", ambient);
        objectShader->setVec3(prefix + "diffuse", diffuse);
        objectShader->setVec3(prefix + "specular", specular);
        objectShader->setFloat(prefix + "constant", constant);
        objectShader->setFloat(prefix + "linear", linear);
        objectShader->setFloat(prefix + "quadratic", quadratic);
        objectShader->setFloat(prefix + "cutOff", cutOff);
        objectShader->setFloat(prefix + "outerCutOff", outerCutOff);
    }

    void setSpotLightPosition(const glm::vec3& position, const glm::vec3& direction){
        std::string prefix("spotLight.");
        objectShader->use();
        objectShader->setVec3(prefix + "position", position);
        objectShader->setVec3(prefix + "direction", direction);
    }


};
unsigned Object::object_count = 0u;
Shader* Object::objectShader = nullptr;

#endif //PROJECT_BASE_OBJECT_H
