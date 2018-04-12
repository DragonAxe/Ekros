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

        forwardPressed = (key == GLFW_KEY_W) ? pressed : forwardPressed;
        backwardPressed = (key == GLFW_KEY_S) ? pressed : backwardPressed;

        leftPressed = (key == GLFW_KEY_A) ? pressed : leftPressed;
        rightPressed = (key == GLFW_KEY_D) ? pressed : rightPressed;

        upPressed = (key == GLFW_KEY_SPACE) ? pressed : upPressed;
        downPressed = (key == GLFW_KEY_LEFT_SHIFT) ? pressed : downPressed;
    }

    void mouseCallback(GLFWwindow* window __attribute__((unused)),
                       double xpos, double ypos)
    {
        cursorXPosition = xpos;
        cursorYPosition = ypos;
    }

    /**
     * Should be called every frame.
     */
    void update()
    {
        // Calculate orientation of camera
        glm::mat4 rmx = glm::rotate(glm::mat4(1.0f),
                                    (float) (cursorXPosition / 500.0f),
                                    glm::vec3(0, 1, 0));
        glm::mat4 rmy = glm::rotate(glm::mat4(1.0f),
                                    (float) (cursorYPosition / 500.0f),
                                    glm::vec3(1, 0, 0));
        orientation = rmy * rmx;

        // Calculate orientation of movement vector
        glm::mat4 reflectmx = glm::mat4(-1.0, 0.0, 0.0, 0.0,
                                        0.0, 1.0, 0.0, 0.0,
                                        0.0, 0.0, 1.0, 0.0,
                                        0.0, 0.0, 0.0, 1.0);
        glm::mat4 reflectmy = glm::mat4(1.0, 0.0, 0.0, 0.0,
                                        0.0, -1.0, 0.0, 0.0,
                                        0.0, 0.0, 1.0, 0.0,
                                        0.0, 0.0, 0.0, 1.0);
        glm::mat4 reflectedOrientation = (reflectmx * rmx) * (reflectmy * rmy);

        // Calculate axis aligned movement vector from key presses
        glm::vec3 moveVec = glm::vec3();
        moveVec.z += (forwardPressed) ? 1.0f : (backwardPressed) ? -1.0f : 0;
        moveVec.x += (rightPressed) ? 1.0f : (leftPressed) ? -1.0f : 0;
        moveVec.y += (upPressed) ? 1.0f : (downPressed) ? -1.0f : 0;
        moveVec *= 0.1f;

        // Move camera by oriented movement vector
        position += (glm::vec3) // Why I have to cast I don't know. C++ is stupid
                (reflectedOrientation * glm::vec4(moveVec, 1.0f));
    }

    glm::mat4 getCameraMatrix()
    {
        glm::mat4 posM = glm::translate(glm::mat4(1.0f), position);

        glm::mat4 perspectiveM = glm::perspective(
                glm::radians(90.0f), // Fov in degrees
                // Aspect ratio
                (float) DisplayManager::getWindowWidth() /
                (float) DisplayManager::getWindowHeight(),
                0.01f,   // Near fov
                1000.f); // Far fov

        return perspectiveM * orientation * posM;
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
};

