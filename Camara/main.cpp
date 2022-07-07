#include "shader.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
float getDeltaTime();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float xRot{ 0.0f };
float yRot{ 0.0f };

float vertices[] =
{
    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
};

GLuint indices[] =
{
    // down
    0, 3, 2,
    0, 2, 1,

    // up
    7, 4, 5,
    7, 5, 6,

    // left
    7, 3, 0,
    7, 0, 4,

    // right
    5, 1, 2,
    5, 2, 6,

    // back
    6, 2, 3,
    6, 3, 7,

    // front
    4, 0, 1,
    4, 1, 5
};

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef APPLE
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Tutorial 9", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader shader{ "shader" };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 4.0f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::vec3 colors[] =
    {
        glm::vec3(1.0f, 1.0f, 0.0f), // YELLOW
        glm::vec3(1.0f, 1.0f, 1.0f), // WHITE
        glm::vec3(0.0f, 1.0f, 0.0f), // GREEN
        glm::vec3(0.0f, 0.0f, 1.0f), // BLUE
        glm::vec3(1.0f, 0.5f, 0.0f), // ORANGE
        glm::vec3(1.0f, 0.0f, 0.0f)  // RED
    };

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, xRot, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, yRot, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 MVP = proj * view * model;

        shader.use();
        shader.setMat4("u_MVP", MVP);

        glBindVertexArray(VAO);
        for (int i = 0; i < 6; i++)
        {
            shader.setVec3("u_faceColor", colors[i]);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(i * 6 * sizeof(GLuint)));
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    float deltaTime{ getDeltaTime() };
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        xRot -= deltaTime;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        xRot += deltaTime;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        yRot -= deltaTime;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        yRot += deltaTime;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        xRot -= deltaTime;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        xRot += deltaTime;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        yRot -= deltaTime;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        yRot += deltaTime;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

float getDeltaTime()
{
    static float lastTime{ 0.0f };
    float currentTime{ static_cast<float>(glfwGetTime()) };
    float deltaTime{ currentTime - lastTime };
    lastTime = currentTime;
    return deltaTime;
}
