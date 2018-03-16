#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/common.hpp>

#include <iostream>
#include <execinfo.h>
#include <zconf.h>
#include <csignal>
#include <fstream>

#include "Emodel.hpp"
#include "renderEngine/DisplayManager.hpp"
#include "shaders/StaticShader.hpp"


void handler(int sig);
void processInput(GLFWwindow* window);


int main()
{
    signal(SIGSEGV, handler); // Register our backtrace handler

    DisplayManager dm = DisplayManager();
    GLFWwindow* window = dm.getWindow();

    StaticShader shader = StaticShader();

    // Load a new model from .obj file
    Emodel* triangles = new Emodel("MuseumModels/objs/Pikachu.obj");

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    GLfloat xyzRot[] = {0.0, 0.0, 0.0};

    // render loop
    // -----------
    while(!glfwWindowShouldClose(window)){

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.start();
        {
            // Hacked on code to allow inputs to the vertex shader program
            GLint xyzRotLoc = glGetUniformLocation(shader.shaderProgramID,
                                                   "xyzrot");
            glUniform3fv(xyzRotLoc, 1, xyzRot);
            xyzRot[1] += 0.5;

            triangles->draw();
        }
        shader.stop();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete triangles;

    return 0;
}

/**
 * A system signal handler that prints a stacktrace.
 *
 * @param sig The signal id (I.e. SIGINT)
 */
void handler(int sig)
{
    void *array[10];
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
 */
void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}
