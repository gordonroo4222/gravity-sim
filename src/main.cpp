#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <tuple>
#include <string>
#include <cmath>

//./build/Debug/OpenGLProject.exe

//constants
const int windowWidth = 800;
const int windowHeight = 800;

//shaders
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.0f, 1.0f, 1.0f, 1.0f);\n"
    "}\0";

//headers
void glfwStartup();
void glfwCleanup(GLFWwindow* window);
void tickEnd(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

GLuint createVertexShader(const char* vertexShaderSource);
GLuint createFragmentShader(const char* fragmentShaderSource);
GLuint createShaderProgram(GLuint vertexShader, GLuint fragmentShader);
void resetBuffers();
void setupBuffers(GLuint &EBO, GLuint &VAO, GLuint &VBO, GLenum drawMode, float vertices[], GLuint indices[], size_t verticesSize, size_t indicesSize);

GLFWwindow* setupWindow(int windowWidth, int windowHeight, std::string windowTitle);



int main() {
    //Initialise glfw and required functions
    glfwStartup();
    GLFWwindow* window = setupWindow(windowWidth, windowHeight, "gravity-sim");

    //setup the shaders and link them into a program
    GLuint vertexShader, fragmentShader, shaderProgram;
    vertexShader = createVertexShader(vertexShaderSource);
    fragmentShader = createFragmentShader(fragmentShaderSource);
    shaderProgram = createShaderProgram(vertexShader, fragmentShader);

    //sets up the vertices
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
    };
    
    GLuint indices[] = {
        0, 1, 2
    };

    //sets up the buffers
    GLuint EBO, VAO, VBO;
    setupBuffers(EBO, VAO, VBO, GL_STATIC_DRAW, vertices, indices, sizeof(vertices), sizeof(indices));
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    resetBuffers(); //reset VAO VBO EBO

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //Window mainloop
    while(!glfwWindowShouldClose(window)) {
        //start frame with process inputs
        processInput(window);

        //render commands
        //Sets background color & clear screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        
        //end frame
        tickEnd(window); //Swaps buffer and updates poll events
    }

    //Final cleanups
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    glfwCleanup(window); //terminates glfw and cleans up data
    return 0;
}

//functions definitions
void glfwStartup() {
    //initialises GLFW
    glfwInit();

    //Sets the window versions to use
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWwindow* setupWindow(int windowWidth, int windowHeight, std::string windowTitle) {
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    //Make GLFW know which window to use
    glfwMakeContextCurrent(window);

    gladLoadGL();

    glViewport(0, 0, windowWidth, windowHeight);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return window;
}

void glfwCleanup(GLFWwindow* window) {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void tickEnd(GLFWwindow* window) {
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

GLuint createVertexShader(const char* vertexShaderSource) {
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    return vertexShader;
}

GLuint createFragmentShader(const char* fragmentShaderSource) {
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    return fragmentShader;
}

GLuint createShaderProgram(GLuint vertexShader, GLuint fragmentShader) {
    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void resetBuffers() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void setupBuffers(GLuint &EBO, GLuint &VAO, GLuint &VBO, GLenum drawMode, float vertices[], GLuint indices[], size_t verticesSize, size_t indicesSize) {
    //Generate the buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //Binds and sets buffer data
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, drawMode);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, drawMode);
}


