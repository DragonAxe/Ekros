#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/common.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <execinfo.h>
#include <zconf.h>
#include <csignal>
#include <fstream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "model/Emodel.hpp"
#include "renderEngine/DisplayManager.hpp"
#include "shaders/StaticShader.hpp"
#include "Camera.cpp"

// stb_image.h must be included at least once in a .cpp file. See:
// https://stackoverflow.com/questions/43348798/double-inclusion-and-headers-only-library-stbi-image
#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

void handler(int sig);

void
key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

int modelCount = 0;
int state = 0;
Camera camera;

int main()
{
    signal(SIGSEGV, handler); // Register our backtrace handler

    DisplayManager dm = DisplayManager();
    GLFWwindow* window = dm.getWindow();

    StaticShader shader = StaticShader();

    // Load a new model from .obj file
    Emodel textured = Emodel("MuseumModels/objs/CubeTextured.obj",
                             &shader,
                             glm::vec3(3, 0, -3));
    Emodel museum = Emodel("MuseumModels/museum/museum.obj",
                           &shader,
                           glm::vec3(0, -1.5, 1));
    Emodel pikachu = Emodel("MuseumModels/objs/Pikachu.obj",
                            &shader,
                            glm::vec3(1, 0, -3));
    Emodel tree = Emodel("MuseumModels/objs/Tree.obj",
                         &shader,
                         glm::vec3(-1, 0, -3));
    Emodel monkey = Emodel("MuseumModels/objs/Suzanne.obj",
                           &shader,
                           glm::vec3(-3, 0, -3));
    Emodel isoTextured = Emodel("MuseumModels/objs/ISOTextured.obj",
                                &shader,
                                glm::vec3(-5, 0, -3));

    camera = Camera();


    // uncomment this call to draw in wireframe polygons
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //Handle key presses via callback
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {

        // render
        // ------
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        textured.draw(camera);
        museum.draw(camera);
        pikachu.draw(camera);
        tree.draw(camera);
        monkey.draw(camera);
        isoTextured.draw(camera);

        // glfw: swap buffers and poll IO events
        // (keys pressed/released, mouse moved etc.)
        // -----------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        camera.update();
    }

    return 0;
}

/**
 * A system signal handler that prints a stacktrace.
 *
 * @param sig The signal id (I.e. SIGINT)
 */
void handler(int sig)
{
    void* array[10];
    int size;

    // get void*'s for all entries on the stack
    size = backtrace(array, 10);

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}

/**
 * Process all keyboard input. This is a GLFW callback function.
 */
void key_callback(GLFWwindow* window __attribute__((unused)), int key,
                  int scancode __attribute__((unused)), int action,
                  int mods __attribute__((unused)))
{
    camera.keyCallback(window, key, scancode, action, mods);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        exit(EXIT_SUCCESS);
}

/**
 * Process all mouse motion input. This is a GLFW callback function.
 */
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    camera.mouseCallback(window, xpos, ypos);
}
