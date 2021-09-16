#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    ROLL_LEFT,
    ROLL_RIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float ROLL        = 0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    // euler Angles
    float Yaw;
    float Pitch;
    float Roll;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH, float roll = ROLL) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        Yaw = yaw;
        Pitch = pitch;
        Roll = roll;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch, float roll) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        Yaw = yaw;
        Pitch = pitch;
        Roll = roll;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    inline glm::mat4 GetViewMatrix(){return glm::lookAt(Position, Position + Front, Up);}

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
        velocity *= 35.0f;
        if (direction == ROLL_LEFT) {
            const auto r = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(-velocity), Front));
            Right = r * Right;
            Up = r * Up;
        }
        if (direction == ROLL_RIGHT) {
            const auto r = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(velocity), Front));
            Right = r * Right;
            Up = r * Up;
        }
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        {
            auto r = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(-xoffset), Up));
            Right = r * Right;
            Front = r * Front;
        }

        {
            auto r = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(yoffset), Right));
            Up = r * Up;
            Front = r * Front;
        }
    }


    inline void ProcessMouseScroll(float yoffset){Zoom = std::max(std::min(Zoom - (float)yoffset, 1.0f), 45.0f);}

private:
     //calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        glm::mat4 r = glm::mat4(1.0f);
        r = glm::rotate(r, glm::radians(Roll), glm::vec3(0.0f, 1.0f, 0.0f));
        r = glm::rotate(r, glm::radians(Pitch), glm::vec3(1.0f, 0.0f, 0.0f));
        r = glm::rotate(r, glm::radians(Yaw), glm::vec3(0.0f, 0.0f, 1.0f));

        const auto rp = glm::mat3(r);

        const auto front =  rp * glm::vec3(1.0f, 0.0f, 0.0f);
        Front = glm::normalize(glm::vec3(front.x, front.z, front.y));

        const auto right = rp * glm::vec3(0.0f, 1.0f, 0.0f);
        Right = glm::normalize(glm::vec3(right.x, right.z, right.y));

        const auto up = rp * glm::vec3(0.0f, 0.0f, 1.0f);
        Up = glm::normalize(glm::vec3(up.x, up.z, up.y));
    }
};
#endif