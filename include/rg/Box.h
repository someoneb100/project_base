//
// Created by snakefoot on 11.9.21..
//

#ifndef PROJECT_BASE_BOX_H
#define PROJECT_BASE_BOX_H

#include <glad/glad.h>
#include <stb_image.h>
#include <rg/Error.h>
#include <rg/Shader.h>
#include <rg/Texture2D.h>

class Cube{
protected:
    unsigned VAO;

    static unsigned VBO;
    static unsigned cube_count;

    Cube() {
        if(cube_count == 0u){
            float vertices[] = {
                    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
                    0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
                    0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
                    0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
                    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
                    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,

                    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f,  1.0f,
                    0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f,  1.0f,
                    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f,  1.0f,
                    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f,  1.0f,
                    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f,  1.0f,
                    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f,  1.0f,

                    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
                    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
                    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
                    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
                    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
                    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

                    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
                    0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
                    0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
                    0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
                    0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
                    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,

                    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  -1.0f,  0.0f,
                    0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  -1.0f,  0.0f,
                    0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  -1.0f,  0.0f,
                    0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  -1.0f,  0.0f,
                    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  -1.0f,  0.0f,
                    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  -1.0f,  0.0f,

                    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
                    0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
                    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
                    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
                    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
                    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f
            };
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        }
        ++cube_count;
    }

    ~Cube(){
        --cube_count;
        if(cube_count == 0u){
            glDeleteBuffers(1, &VBO);
        }
    }

public:
    virtual void setView(const glm::mat4& view) = 0;
    virtual void setProjection(const glm::mat4& projection) = 0;
    virtual void setProjectionView(const glm::mat4& projection, const glm::mat4& view) = 0;
    virtual void render(const glm::mat4& model) = 0;

protected:
    void setView(Shader* shader, const glm::mat4& view){
        shader->use();
        shader->setMat4("view", view);
    }
    inline void setProjection(Shader* shader, const glm::mat4& projection){
        shader->use();
        shader->setMat4("projection", projection);
    }
    void setProjectionView(Shader* shader, const glm::mat4& projection, const glm::mat4& view){
        shader->use();
        shader->setMat4("view", view);
        shader->setMat4("projection", projection);
    }
};
unsigned Cube::cube_count = 0u;
unsigned Cube::VBO = 0u;






class Light : public Cube {
protected:
    static unsigned VAO;
    static Shader* shader;
private:
    static unsigned cube_count;

public:
    Light(const glm::vec3& lightColor = glm::vec3(1.0f)) {
        if(shader == nullptr) cube_count = 0u;

        if(cube_count == 0u){
            shader = new Shader("resources/shaders/box/light_cube.vs",
                                "resources/shaders/box/light_cube.fs");
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
        }
        ++cube_count;
    }

    ~Light(){
        --cube_count;
        if(cube_count == 0u){
            shader->deleteProgram();
            delete shader;
            glDeleteVertexArrays(1, &VAO);
        }
    }

    inline void setView(const glm::mat4& view) final {Cube::setView(shader, view);}
    inline void setProjection(const glm::mat4& projection) final {Cube::setProjection(shader, projection);}
    inline void setProjectionView(const glm::mat4& projection, const glm::mat4& view) final {Cube::setProjectionView(shader, projection, view);}
};
unsigned Light::cube_count = 0u;
unsigned Light::VAO = 0u;
Shader* Light::shader = nullptr;



class LightCube : public Light{
private:
    glm::vec3 lc;
    float ambient, diffuse, specular;
    float constant, linear, quadratic;

public:
    LightCube(const glm::vec3& lightColor = glm::vec3(1.0f))
    : lc(lightColor)
    , ambient(0.05f), diffuse(0.8f), specular(1.0f)
    , constant(1.0f), linear(0.09f), quadratic(0.032f){}

    void render(const glm::mat4& model) final{
        shader->use();
        shader->setVec3("lc", lc);
        shader->setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    inline glm::vec3 getLightColor() const {return lc;}
    inline void setLightColor(const glm::vec3 &lightColor) {lc = lightColor;}

    inline float getAmbient() const {return ambient;}
    inline float getDiffuse() const {return diffuse;}
    inline float getSpecular() const {return specular;}
    inline float getConstant() const {return constant;}
    inline float getLinear() const {return linear;}
    inline float getQuadratic() const {return quadratic;}

    void setADS(float ambient, float diffuse, float specular){
        this->ambient  = ambient;
        this->diffuse  = diffuse;
        this->specular = specular;
    }

    void setK(float constant, float linear, float quadratic){
        this->constant  = constant;
        this->linear    = linear;
        this->quadratic = quadratic;
    }
};






class Box : public Cube{
protected:
    static unsigned cube_count;
    static unsigned VAO;
    static Shader* shader;

    Box(){
        if(shader == nullptr) cube_count = 0u;

        if(cube_count == 0u){
            shader = new Shader("resources/shaders/box/box.vs", "resources/shaders/box/box.fs");
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            // texture coord attribute
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
            glEnableVertexAttribArray(2);
        }
        ++cube_count;
    }

    ~Box(){
        --cube_count;
        if(cube_count == 0u) {
            shader->deleteProgram();
            delete shader;
            glDeleteVertexArrays(1, &VAO);
        }
    }

public:

    inline void setView(const glm::mat4& view) final {Cube::setView(shader, view);}
    inline void setProjection(const glm::mat4& projection) final {Cube::setProjection(shader, projection);}
    inline void setProjectionView(const glm::mat4& projection, const glm::mat4& view) final {Cube::setProjectionView(shader, projection, view);}


    void setViewPos(const glm::vec3& viewPos){
        shader->use();
        shader->setVec3("viewPos", viewPos);
    }

    void setDirectionalLight(const glm::vec3& direction, const glm::vec3& ambient,
                             const glm::vec3& diffuse, const glm::vec3& specular){
        std::string prefix("dirLight.");
        shader->use();
        shader->setVec3(prefix + "direction", direction);
        shader->setVec3(prefix + "ambient", ambient);
        shader->setVec3(prefix + "diffuse", diffuse);
        shader->setVec3(prefix + "specular", specular);
    }

    void setPointLight(unsigned int num, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
                       float constant, float linear, float quadratic) {
        std::string prefix = "pointLights[" + std::to_string(num) + "].";
        shader->use();
        shader->setVec3(prefix + "ambient", ambient);
        shader->setVec3(prefix + "diffuse", diffuse);
        shader->setVec3(prefix + "specular", specular);
        shader->setFloat(prefix + "constant", constant);
        shader->setFloat(prefix + "linear", linear);
        shader->setFloat(prefix + "quadratic", quadratic);
    }

    void setPointLight(unsigned int num, const LightCube& light){
        setPointLight(num, light.getLightColor() * light.getAmbient()
                         , light.getLightColor() * light.getDiffuse()
                         , light.getLightColor() * light.getSpecular()
                         , light.getConstant(), light.getLinear(), light.getQuadratic());
    }

    void setPointLightPosition(unsigned int num, const glm::vec3& position){
        std::string prefix = "pointLights[" + std::to_string(num) + "].";
        shader->use();
        shader->setVec3(prefix + "position", position);
    }

    void setSpotLight(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
                      float constant, float linear, float quadratic,
                      float cutOff, float outerCutOff){
        std::string prefix("spotLight.");
        shader->use();
        shader->setVec3(prefix + "ambient", ambient);
        shader->setVec3(prefix + "diffuse", diffuse);
        shader->setVec3(prefix + "specular", specular);
        shader->setFloat(prefix + "constant", constant);
        shader->setFloat(prefix + "linear", linear);
        shader->setFloat(prefix + "quadratic", quadratic);
        shader->setFloat(prefix + "cutOff", cutOff);
        shader->setFloat(prefix + "outerCutOff", outerCutOff);
    }

    void setSpotLightPosition(const glm::vec3& position, const glm::vec3& direction){
        std::string prefix("spotLight.");
        shader->use();
        shader->setVec3(prefix + "position", position);
        shader->setVec3(prefix + "direction", direction);
    }

};
unsigned Box::cube_count = 0u;
unsigned Box::VAO = 0u;
Shader* Box::shader = nullptr;




class SimpleBox : public Box{
private:
    Textrure2D texture;

public:
    SimpleBox() : texture("resources/textures/box/container.jpg", GL_REPEAT, GL_LINEAR){}

    void render(const glm::mat4& model){
        shader->use();
        glBindVertexArray(VAO);
        texture.activate(GL_TEXTURE0);
        shader->setInt("material.diffuse", 0);
        shader->setInt("material.specular", -1);
        shader->setFloat("material.shininess", 128.0f);
        shader->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

};

class ComplexBox : public Box{
private:
    Textrure2D texture;
    Textrure2D outline;
public:
    ComplexBox() : texture("resources/textures/box/container2.png", GL_REPEAT, GL_LINEAR),
                   outline("resources/textures/box/container2_specular.png", GL_REPEAT, GL_LINEAR){}


    void render(const glm::mat4& model){
        shader->use();
        glBindVertexArray(VAO);
        texture.activate(GL_TEXTURE0);
        outline.activate(GL_TEXTURE1);
        shader->setInt("material.diffuse", 0);
        shader->setInt("material.specular", 1);
        shader->setFloat("material.shininess", 32.0f);
        shader->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
};



#endif //PROJECT_BASE_BOX_H
