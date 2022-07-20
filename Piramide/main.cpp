#include <iostream>
#include <string>

#include "shader.hpp"
#include "opengl.h"
#include "triangle.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void processInput(GLFWwindow* window);

// ajustes
const unsigned int SCR_WIDTH = 625;
const unsigned int SCR_HEIGHT = 625;
const char *title = "Piramide";

// vector de triangulos
std::vector<Triangle> piramide;

int main()
{
    opengl::init();
    GLFWwindow* window = opengl::createWindow(SCR_WIDTH, SCR_HEIGHT, title);
    opengl::loadGlad();

    // creacion de trianglos
    Triangle triangle1({ -0.5f, 0.0f, 0.5f }, { 0.5f, 0.0f, 0.5f }, { 0.0f, 0.0f, -0.5f }, "blue");
    Triangle triangle2({ -0.5f, 0.0f, 0.5f }, { 0.5f, 0.0f, 0.5f }, { 0.0f, 0.5f, 0.0f }, "green");
    Triangle triangle3({ 0.5f, 0.0f, 0.5f }, { 0.0f, 0.0f, -0.5f }, { 0.0f, 0.5f, 0.0f }, "red");
    Triangle triangle4({ -0.5f, 0.0f, 0.5f }, { 0.0f, 0.0f, -0.5f }, { 0.0f, 0.5f, 0.0f }, "yellow");

    piramide = { triangle1, triangle2, triangle3, triangle4 };

    Shader shader = "shader";
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    shader.setMat4("projection", proj);
    
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);


    std::cout << "MENU:\n";
    std::cout << "Traslacion sobre eje X: letras 'Q' y 'W' (para ir a la izquierda y derecha )\n";
    std::cout << "Traslacion sobre eje Y: letras 'A' y 'S' (para ir hacia atras y adelante )\n";
    std::cout << "Rotacion sobre eje X: letras 'O' y 'P' (para ir en sentido horario y antihorario )\n";
    std::cout << "Rotacion sobre eje Y: letras 'K' y 'L' (para ir en sentido horario y antihorario )\n";
    std::cout << "Rotacion de la camara sobre eje Y: letras 'Z' y 'X' (para ir en sentido horario y antihorario )\n";
    
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (int i = 0; i < 4; i++)
            piramide[i].draw(shader, proj);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // liberacion de recursos
    for (int i = 0; i < 4; i++)
        piramide[i].free();

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    float deltaTime = opengl::getDeltaTime();

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // TRASLACION SOBRE EJE X
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        for (int i = 0; i < 4; i++)
        {
            piramide[i].translate(-0.001f, 0.0f, 0.0f);
        }
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        for (int i = 0; i < 4; i++)
        {
            piramide[i].translate(0.001f, 0.0f, 0.0f);
        }
    }

    // TRASLACION SOBRE EJE Z
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        for (int i = 0; i < 4; i++)
        {
            piramide[i].translate(0.0f, 0.0f, -0.001f);
        }
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        for (int i = 0; i < 4; i++)
        {
            piramide[i].translate(0.0f, 0.0f, 0.001f);
        }
    }


    // ROTACION SOBRE EJE X
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        for (int i = 0; i < 4; i++)
        {
            piramide[i].rotate(0.05f, rotation_axis::X);
        }
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        for (int i = 0; i < 4; i++)
        {
            piramide[i].rotate(-0.05f, rotation_axis::X);
        }
    }

    // ROTACION SOBRE EJE Y
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        for (int i = 0; i < 4; i++)
        {
            piramide[i].rotate(0.05f, rotation_axis::Y);
        }
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        for (int i = 0; i < 4; i++)
        {
            piramide[i].rotate(-0.05f, rotation_axis::Y);
        }
    }


     // ROTACION DE CAMARA
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        yRot -= deltaTime;
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        yRot += deltaTime;
 
}

