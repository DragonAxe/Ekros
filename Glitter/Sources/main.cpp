#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/common.hpp>

#include <iostream>
#include <execinfo.h>
#include <zconf.h>
#include <csignal>
#include <fstream>
#include <cmath>

#include "Emodel.hpp"
#include "renderEngine/DisplayManager.hpp"
#include "shaders/StaticShader.hpp"


void handler(int sig);

void
key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int modelCount = 0;
int state = 0;

int main()
{
    signal(SIGSEGV, handler); // Register our backtrace handler

    DisplayManager dm = DisplayManager();
    GLFWwindow* window = dm.getWindow();

    StaticShader shader = StaticShader();

    // Load a new model from .obj file
    Emodel* textured = new Emodel("MuseumModels/objs/ISOTextured.obj");
    Emodel* pikachu = new Emodel("MuseumModels/objs/Pikachu.obj");
    Emodel* tree = new Emodel("MuseumModels/objs/Tree.obj");
    Emodel* monkey = new Emodel("MuseumModels/objs/Suzanne.obj");
    modelCount = 4;

    Emodel* models[] = {textured, tree, pikachu, monkey};

    // uncomment this call to draw in wireframe polygons
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    GLfloat xyzRot[] = {0.0, 0.0, 0.0};

    //Handle key presses via callback
    glfwSetKeyCallback(window, key_callback);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {

        // render
        // ------
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.start();
        {
            shader.setRotation(xyzRot);
            xyzRot[1] += 0.5;

            models[state]->draw();
        }
        shader.stop();

        // glfw: swap buffers and poll IO events
        // (keys pressed/released, mouse moved etc.)
        // -----------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete pikachu;

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
 * process all input: query GLFW whether relevant keys are pressed/released
 * this frame and react accordingly
 *
 * @param window
 * @param key
 * @param scancode
 * @param action
 * @param mods
 */
void key_callback(GLFWwindow* window __attribute__((unused)), int key,
                  int scancode __attribute__((unused)), int action,
                  int mods __attribute__((unused)))
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        exit(EXIT_SUCCESS);

    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        state = (state + 1) % modelCount;
    }

    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        if (state == 0) {
            state = modelCount - 1;
        } else {
            state = (state - 1) % modelCount;
        }
    }
}
