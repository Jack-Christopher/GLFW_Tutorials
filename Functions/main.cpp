#include <iostream>
#include <string>

#include "shader.hpp"
#include "opengl.h"
//#include "triangle.h"
#include "container.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 625;
const unsigned int SCR_HEIGHT = 625;
const char *title = "Testing";


// objects creation
// Triangle triangle(  { 0.0f, 0.0f, 0.0f },   { 0.75f, 0.0f, 0.0f },   { 0.0f, 0.75f, 0.0f },   "red"   );

std::vector<Triangle> triangles = {
    { { 0.0f, 0.0f, 0.0f }, { 0.5f, -0.5f, 0.0f }, { -0.5f, -0.5f, 0.0f }, "green" },
    { { 0.0f, 0.0f, 0.0f }, { 0.5f, 0.5f, 0.0f }, { 0.5f, -0.5f, 0.0f }, "blue" },
    { { 0.0f, 0.0f, 0.0f }, { 0.5f, 0.0f, 0.0f }, { 0.0f, 0.5f, 0.0f }, "red" }
};
Container bloque(triangles);

// Shaders
Shader shader = "shader";
glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

int main()
{
    opengl::init();
    GLFWwindow* window = opengl::createWindow(SCR_WIDTH, SCR_HEIGHT, title);
    opengl::loadGlad();

    shader.setMat4("projection", proj);
    
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);

    
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // triangle.draw(shader, proj);

        // triangle.scale(0.9998f, 0.9998f, 0.9998f);
        // triangle.translate(0.0003f, 0.0f, 0.0f);
        // triangle.rotate(0.05f, rotation_axis::Z);
        bloque.draw(shader, proj);
        bloque.rotate(0.05f, rotation_axis::Z);
        //bloque.translate(0.0003f, 0.0f, 0.0f);
        //bloque.scale(0.9998f, 0.9998f, 0.9998f);
        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //triangle.free();

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    float deltaTime = opengl::getDeltaTime();

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Camera movements
    const float cameraSpeed = 0.01f; // adjust accordingly

    // Camera front movement
    if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    // Camera back movement
    if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    // Camera left movement
    if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    // Camera right movement
    if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    // Camera up movement
    if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraUp;
    // Camera down movement
    if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraUp;

    // Camera rotations
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        xRot -= deltaTime;
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        xRot += deltaTime;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        yRot -= deltaTime;
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
        yRot += deltaTime;
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
        zRot += deltaTime;
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
        zRot -= deltaTime;
}

