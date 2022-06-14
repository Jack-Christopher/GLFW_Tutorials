

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float rValue = 0.0f, gValue = 0.0f, bValue = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Color en RGB", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    float cColor = 0.0f, cColorInc = 0.0001f;


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(rValue, gValue, bValue, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        cColor = cColor + cColorInc;

        if (cColor > 1.f)
            cColorInc *= -1.f;

        if (cColor < 0.f)
            cColorInc *= -1.f;
    }

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
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    std::string title = "Color en RGB (" + std::to_string(rValue) + ", " + std::to_string(gValue) + ", " + std::to_string(bValue) + ")";
    glfwSetWindowTitle(window, title.c_str());
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        if (rValue == 0)
        {
            rValue = 1;
            //glfwSetWindowTitle(window, "Red");
        }
        else
        {
            rValue = 0;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        if (gValue == 0)
        {
            gValue = 1;
            //glfwSetWindowTitle(window, "Green");
        }
        else
        {
            gValue = 0;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        if (bValue == 0)
        {
            bValue = 1;
            //glfwSetWindowTitle(window, "Blue");
        }
        else
        {
            bValue = 0;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        rValue = 1;
        gValue = 1;
        bValue = 1;
        //glfwSetWindowTitle(window, "White");
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        rValue = 0;
        gValue = 0;
        bValue = 0;
        //glfwSetWindowTitle(window, "Black");
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
