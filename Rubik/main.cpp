#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "Rubik.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int key_press_counter = 0;
const int n_shaders = 7;
Rubik rubik;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

std::map<std::string, const char*> fragmentShaderSources;
std::map<std::string, unsigned int> shaderPrograms;


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Rubik's Cube", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    for (auto& c : colors)
    {
        fragmentShaderSources[c.first] = op::fragmentShaderCreator(c.second);
        shaderPrograms[c.first] = 0;
    }

    std::vector<unsigned int> vertexShaders(fragmentShaderSources.size());
    std::vector<unsigned int> fragmentShaders(fragmentShaderSources.size());

    int idx = 0;
    for (auto& f : fragmentShaderSources)
    {
        op::prepare_shader(vertexShaders[idx], fragmentShaders[idx], vertexShaderSource, f.second, shaderPrograms[f.first]);
        idx++;
    }

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    rubik.prepare_VB0_VAO();

    // Improve rendering based on the z-buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw figures
        // ------------
        rubik.draw(shaderPrograms);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    rubik.free();
	
    for (auto& shader : shaderPrograms)
        glDeleteProgram(shader.second);
    for (auto& fragmentShader : fragmentShaderSources)
        delete fragmentShader.second;
	
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
	
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        rubik.move(key_press_counter, 1.0f, rotation_axis::X);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        rubik.move(key_press_counter, -1.0f, rotation_axis::X);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        rubik.move(key_press_counter, 1.0f, rotation_axis::Y);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        rubik.move(key_press_counter, -1.0f, rotation_axis::Y);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        rubik.move(key_press_counter, 1.0f, rotation_axis::Z);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        rubik.move(key_press_counter, -1.0f, rotation_axis::Z);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}