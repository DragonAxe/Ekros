#include "DisplayManager.hpp"

/**
 * Creates a new window with a GL context that is ready for rendering.
 *
 * @throws std::runtime_error if window initialization failed.
 * @return The GLFWwindow context.
 */
DisplayManager::DisplayManager()
{
    this->window = DisplayManager::initializeGLFWwindow();

    DisplayManager::initializeGLAD();

    DisplayManager::enableGLDebugging();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

/**
 * Initializes a GLFWwindow that is ready for rendering.
 *
 * @throws std::runtime_error if GLFW was unable to open a window.
 * @return A pointer to the newly opened GLFW window.
 */
GLFWwindow* DisplayManager::initializeGLFWwindow()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH,
                                          SCR_HEIGHT,
                                          WINDOW_TITLE,
                                          NULL,  // monitor
                                          NULL); // share
    if(window == nullptr){
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window,
                                   DisplayManager::framebufferSizeCallback);

    return window;
}

/**
 * Initializes GLAD, a GL extensions loading library.
 *
 * @throws std::runtime_error if GLAD failed to initialize.
 */
void DisplayManager::initializeGLAD()
{
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        throw std::runtime_error("Failed to initialize GLAD");
    }
}

/**
 * Registers an error event handler for OpenGL errors.
 */
void DisplayManager::enableGLDebugging()
{
    glEnable              ( GL_DEBUG_OUTPUT );
    glDebugMessageCallback( (GLDEBUGPROC) DisplayManager::MessageCallback, 0 );
}

/**
 * An OpenGL error handling callback function for printing OpenGL errors as
 * they happen. Parameters provide information about the OpenGL error.
 *
 * @param source
 * @param type
 * @param id
 * @param severity
 * @param length
 * @param message
 * @param userParam
 */
void DisplayManager::MessageCallback(GLenum source __attribute__((unused)),
                      GLenum type,
                      GLuint id __attribute__((unused)),
                      GLenum severity,
                      GLsizei length __attribute__((unused)),
                      const GLchar* message,
                      const void* userParam __attribute__((unused)))
{
    fprintf(stderr,
            "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
            type, severity, message);
}

/**
 * Window resized callback function. Whenever the window is resized
 * (by the OS or by the User), this callback function executes.
 *
 * @param window A pointer to the GLFWwindow that was resized.
 * @param width The new width of the window.
 * @param height The new height of the window.
 */
void DisplayManager::framebufferSizeCallback(
        GLFWwindow *window __attribute__((unused)),
        int width, int height)
{
    // make sure the viewport matches the new window dimensions;
    // Note that width and height will be significantly larger than specified
    // on retina displays.
    glViewport(0, 0, width, height);
}

/**
 * Returns the initialized window, or throws an error if uninitialized.
 *
 * @throws std::runtime_error if window is uninitialized.
 * @return A pointer to the initialized window.
 */
GLFWwindow* DisplayManager::getWindow()
{
    if (this->window == nullptr) {
        throw std::runtime_error("Window is not initialized");
    }
    return this->window;
}



DisplayManager::~DisplayManager()
{
    // glfw: terminate, clearing all previously allocated GLFW resources
    // -----------------------------------------------------------------
    glfwTerminate();
}
