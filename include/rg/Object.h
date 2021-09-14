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
public:
    Object(std::string path)
        :object(path){
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
        object.Draw(*objectShader);
    }
};
unsigned Object::object_count = 0u;
Shader* Object::objectShader = nullptr;

#endif //PROJECT_BASE_OBJECT_H
