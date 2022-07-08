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

// camera system
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// timing
//float deltaTime = 0.0f;	// time between current frame and last frame
//float lastFrame = 0.0f;

float xRot = 0.0f ;
float yRot = 0.0f ;
float zRot = 0.0f ;

unsigned int VBO_array[12];
unsigned int VAO_array[12];

float vertices_[12][9] = {
    // bottom
    {-0.5f, -0.5f, 0.5f,      0.5f, -0.5f, 0.5f,    0.5f, -0.5f, -0.5f},
    {0.5f, -0.5f, -0.5f,    -0.5f, -0.5f, -0.5f,    -0.5f, -0.5f, 0.5f},
    // top
    {-0.5f, 0.5f, 0.5f,      0.5f, 0.5f, 0.5f,    0.5f, 0.5f, -0.5f},
    {0.5f, 0.5f, -0.5f,      -0.5f, 0.5f, -0.5f,    -0.5f, 0.5f, 0.5f},
    // left
    {-0.5f, -0.5f, -0.5f,      -0.5f, 0.5f, -0.5f,    -0.5f, 0.5f, 0.5f},
    {-0.5f, 0.5f, 0.5f,      -0.5f, -0.5f, 0.5f,    -0.5f, -0.5f, -0.5f},
    // right
    {0.5f, -0.5f, -0.5f,      0.5f, 0.5f, -0.5f,    0.5f, -0.5f, 0.5f},
    {0.5f, 0.5f, 0.5f,      0.5f, -0.5f, 0.5f,    0.5f, 0.5f, -0.5f},
    // back
    {-0.5f, -0.5f, -0.5f,      0.5f, -0.5f, -0.5f,    -0.5f, 0.5f, -0.5f},
    {0.5f, -0.5f, -0.5f,      0.5f, 0.5f, -0.5f,    -0.5f, 0.5f, -0.5f},
    // front
    {-0.5f, -0.5f, 0.5f,      0.5f, -0.5f, 0.5f,    0.5f, 0.5f, 0.5f},
    {-0.5f, -0.5f, 0.5f,      0.5f, 0.5f, 0.5f,    -0.5f, 0.5f, 0.5f}
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

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Rubik + Camera", NULL, NULL);
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

    // unsigned int VBO, VAO, EBO;
    for (int i = 0; i < 12; i++)
    {
        glGenVertexArrays(1, &VAO_array[i]);
        glGenBuffers(1, &VBO_array[i]);
        // glGenBuffers(1, &EBO);
        glBindVertexArray(VAO_array[i]);

        glBindBuffer(GL_ARRAY_BUFFER, VBO_array[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_[i]), vertices_[i], GL_STATIC_DRAW);

        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        //glEnableVertexAttribArray(0);

        //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        //glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    shader.setMat4("projection", proj);


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
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);


    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, xRot, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, yRot, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, zRot, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 MVP = proj * view * model;
        
        shader.use();
        shader.setMat4("view", view);
        shader.setMat4("u_MVP", MVP);

        for (int i = 0; i < 12; i += 2)
        {
            shader.setVec3("u_faceColor", colors[i / 2]);
            glBindVertexArray(VAO_array[i]);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            shader.setVec3("u_faceColor", colors[i / 2]);
            glBindVertexArray(VAO_array[i+1]);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
        //for (int i = 0; i < 6; i++)
        //{
         //   shader.setVec3("u_faceColor", colors[i]);
            //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(i * 6 * sizeof(GLuint)));
        //}



        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    for (int i = 0; i < 12; i++)
    {
        glDeleteVertexArrays(1, &VAO_array[i]);
        glDeleteBuffers(1, &VBO_array[i]);
        // glDeleteBuffers(1, &EBO);
    }
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    float deltaTime{ getDeltaTime() };
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    // Cube Rotations
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        xRot -= deltaTime;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        xRot += deltaTime;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        yRot -= deltaTime;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        yRot += deltaTime;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        zRot += deltaTime;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        zRot -= deltaTime;

    // Camera movements
    const float cameraSpeed = 0.001f; // adjust accordingly

    // Camera front movement
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    // Camera back movement
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    // Camera left movement
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    // Camera right movement
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    // Camera up movement
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraUp;
    // Camera down movement
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraUp;
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
