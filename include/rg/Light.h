//
// Created by snakefoot on 16.9.21..
//

#ifndef PROJECT_BASE_LIGHT_H
#define PROJECT_BASE_LIGHT_H

#include <glm/vec3.hpp>

class DirectionalLight{
public:
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    DirectionalLight(
            const glm::vec3 &direction,
            const glm::vec3 &ambient,
            const glm::vec3 &diffuse,
            const glm::vec3 &specular
            )
            : direction(direction)
            , ambient(ambient)
            , diffuse(diffuse)
            , specular(specular) {}
};

class SpotLight{
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float constant, linear, quadratic;
    float cutOff, outerCutOff;

    SpotLight(
            const glm::vec3 &ambient,
            const glm::vec3 &diffuse,
            const glm::vec3 &specular,
            float constant, float linear, float quadratic,
            float cutOff, float outerCutOff
            )
            : ambient(ambient)
            , diffuse(diffuse)
            , specular(specular)
            , constant(constant), linear(linear), quadratic(quadratic)
            , cutOff(cutOff)
            , outerCutOff(outerCutOff) {}
};

#endif //PROJECT_BASE_LIGHT_H
