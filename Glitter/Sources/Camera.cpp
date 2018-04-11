#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "renderEngine/DisplayManager.hpp"

class Camera
{
public:

    void keyCallback(GLFWwindow* window __attribute__((unused)),
                     int key,
                     int scancode __attribute__((unused)),
                     int action __attribute__((unused)),
                     int mods __attribute__((unused)))
    {
        bool pressed;
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            pressed = true;
        } else if (action == GLFW_RELEASE) {
            pressed = false;
        }

        if (key == GLFW_KEY_W) {
            forwardPressed = pressed;
        }
        if (key == GLFW_KEY_S) {
            backwardPressed = pressed;
        }

        if (key == GLFW_KEY_A) {
            leftPressed = pressed;
        }
        if (key == GLFW_KEY_D) {
            rightPressed = pressed;
        }

        if (key == GLFW_KEY_SPACE) {
            upPressed = pressed;
        }
        if (key == GLFW_KEY_LEFT_SHIFT) {
            downPressed = pressed;
        }
    }

    void mouseCallback(GLFWwindow* window __attribute__((unused)),
                       double xpos, double ypos)
    {
        cursorXPosition = xpos;
        cursorYPosition = ypos;
    }

    void update()
    {
        glm::mat4 rmx = glm::rotate(glm::mat4(1.0f),
                                    (float) (cursorXPosition / 100.0f),
                                    glm::vec3(0, 1, 0));
        glm::mat4 rmy = glm::rotate(glm::mat4(1.0f),
                                    (float) (cursorYPosition / 100.0f),
                                    glm::vec3(1, 0, 0));
        orientation = rmy * rmx;


        glm::vec3 movementVec = glm::vec3();
        if (forwardPressed) {
            movementVec.z += 0.01f;
        }
        if (backwardPressed) {
            movementVec.z -= 0.01f;
        }
        if (leftPressed) {
            movementVec.x += 0.01f;
        }
        if (rightPressed) {
            movementVec.x -= 0.01f;
        }
        if (upPressed) {
            movementVec.y -= 0.01f;
        }
        if (downPressed) {
            movementVec.y += 0.01f;
        }
        glm::vec3 a = orientation * glm::vec4(movementVec, 1.0f);

        position += a;
    }

    glm::mat4 getCameraMatrix()
    {

        glm::mat4 vm = glm::translate(glm::mat4(1.0f), position);

        glm::mat4 proj = glm::perspective(
                glm::radians(90.0f), // Fov in degrees
                // Aspect ratio
                (float) DisplayManager::getWindowWidth() /
                (float) DisplayManager::getWindowHeight(),
                0.01f,   // Near fov
                1000.f); // Far fov

        return proj * orientation * vm;
    }

private:
    float yaw = 0;
    float pitch = 0;
    double cursorXPosition = 0;
    double cursorYPosition = 0;
    bool forwardPressed = false;
    bool backwardPressed = false;
    bool leftPressed = false;
    bool rightPressed = false;
    bool upPressed = false;
    bool downPressed = false;
    glm::vec3 position = glm::vec3();
    glm::mat4 orientation = glm::mat4();
    glm::mat4 cameraMatrix;
};

