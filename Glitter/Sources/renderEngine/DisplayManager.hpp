#pragma once

#include <iostream>
#include <exception>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

/**
 * This class manages the initialization of a window with an OpenGL context
 * that is ready to be rendered to.
 */
class DisplayManager
{
public:
    DisplayManager();

    GLFWwindow* getWindow();

    static int getWindowWidth();
    static int getWindowHeight();

    ~DisplayManager();

private:
    static const constexpr char* WINDOW_TITLE = "Mckenna_Reece Triangle";
    static const unsigned int SCR_WIDTH = 800;
    static const unsigned int SCR_HEIGHT = 600;

    static int windowWidth;
    static int windowHeight;

    GLFWwindow* window = nullptr;

    static GLFWwindow* initializeGLFWwindow();

    static void initializeGLAD();

    static void MessageCallback(GLenum source, GLenum type, GLuint id,
                                GLenum severity, GLsizei length,
                                const GLchar* message, const void* userParam);

    static void framebufferSizeCallback(GLFWwindow* window,
                                        int width, int height);

    static void enableGLDebugging();
};
