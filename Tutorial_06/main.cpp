#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderCreator()
{
    const char* vertexShader = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
    return vertexShader;
}

const char* fragmentShaderCreator(float R, float G, float B)
{
    // concat float to const char
    std::string temp = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(" + std::to_string(R) + "f, " + std::to_string(G) + "f, " + std::to_string(B) + "f, 1.0f);\n"
        "}\0";
    const char* fragmentShader = new char[temp.length() + 1];
    strcpy(const_cast<char*>(fragmentShader), temp.c_str());
    return fragmentShader;
}

const char* vertexShaderSource = vertexShaderCreator();
const char* fragmentShaderSource = fragmentShaderCreator(0.0f, 0.0f, 0.32f);
const char* vertexShaderSource1 = vertexShaderCreator();
const char* fragmentShaderSource1 = fragmentShaderCreator(0.16f, 0.21f, 0.58f);
const char* vertexShaderSource2 = vertexShaderCreator();
const char* fragmentShaderSource2 = fragmentShaderCreator(0.19f, 0.25f, 0.62f);
const char* vertexShaderSource3 = vertexShaderCreator();
const char* fragmentShaderSource3 = fragmentShaderCreator(0.22f, 0.29f, 0.67f);
const char* vertexShaderSource4 = vertexShaderCreator();
const char* fragmentShaderSource4 = fragmentShaderCreator(0.25f, 0.32f, 0.71f);
const char* vertexShaderSource5 = vertexShaderCreator();
const char* fragmentShaderSource5 = fragmentShaderCreator(0.36f, 0.42f, 0.75f);
const char* vertexShaderSource6 = vertexShaderCreator();
const char* fragmentShaderSource6 = fragmentShaderCreator(0.47f, 0.53f, 0.80f);

std::vector<const char*> vertexShaderSourceList = { vertexShaderSource, vertexShaderSource1, vertexShaderSource2, vertexShaderSource3, vertexShaderSource4, vertexShaderSource5, vertexShaderSource6 };
std::vector<const char*> fragmentShaderSourceList = { fragmentShaderSource, fragmentShaderSource1, fragmentShaderSource2, fragmentShaderSource3, fragmentShaderSource4, fragmentShaderSource5, fragmentShaderSource6 };


void prepare_shader(unsigned int& vertexShader, unsigned int& fragmentShader, const char*& vertexShaderSource, const char*& fragmentShaderSource, unsigned int& shaderProgram)
{
    // vertex shader	
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void prepare_VB0_VAO_triangle(unsigned int& VBO, unsigned int& VAO, float vertices[], int size_of_vertices)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, size_of_vertices, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
}



void get_rectangle_coords(float x1, float y1, float x2, float y2, float* vertices1, float* vertices2)
{
    vertices1[0] = x1; 	vertices1[1] = y1; 	vertices1[2] = 0.0f;
    vertices1[3] = x2; 	vertices1[4] = y1; 	vertices1[5] = 0.0f;
    vertices1[6] = x1; 	vertices1[7] = y2; 	vertices1[8] = 0.0f;


    vertices2[0] = x1; 	vertices2[1] = y2; 	vertices2[2] = 0.0f;
    vertices2[3] = x2; 	vertices2[4] = y2; 	vertices2[5] = 0.0f;
    vertices2[6] = x2; 	vertices2[7] = y1; 	vertices2[8] = 0.0f;
}

void draw_triangle(unsigned int& shaderProgram, unsigned int& VAO)
{
    // draw our first triangle
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // glBindVertexArray(0); // no need to unbind it every time 
}

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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
    const int n_triangles = 14;
    const int n_shaders = 7;
	
    unsigned int vertexShader[n_shaders], fragmentShader[n_shaders], shaderProgramList[n_shaders];
    for (int i = 0; i < n_shaders; i++)
    {
        prepare_shader(vertexShader[i], fragmentShader[i], vertexShaderSourceList[i], fragmentShaderSourceList[i], shaderProgramList[i]);
    }
	

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[9], vertices1[9], vertices2[9], vertices3[9], vertices4[9], vertices5[9], vertices6[9], vertices7[9], vertices8[9], vertices9[9], vertices10[9], vertices11[9], vertices12[9], vertices13[9];
	
    get_rectangle_coords(-1.0f, -1.0f, -0.71f, 1.0f, vertices, vertices1);
    get_rectangle_coords(-0.71f, -1.0f, -0.43f, 1.0f, vertices2, vertices3);
    get_rectangle_coords(-0.43f, -1.0f, -0.14f, 1.0f, vertices4, vertices5);
    get_rectangle_coords(-0.14f, -1.0f, 0.14f, 1.0f, vertices6, vertices7);
    get_rectangle_coords(0.14f, -1.0f, 0.43f, 1.0f, vertices8, vertices9);
    get_rectangle_coords(0.43f, -1.0f, 0.71f, 1.0f, vertices10, vertices11);
    get_rectangle_coords(0.71f, -1.0f, 1.0f, 1.0f, vertices12, vertices13);

    std::vector<float*> vertexList = { vertices, vertices1, vertices2, vertices3, vertices4, vertices5, vertices6, vertices7, vertices8, vertices9, vertices10, vertices11, vertices12, vertices13 };    

	
    unsigned int VBO[n_triangles], VAO[n_triangles];
    int size_of_vertices = sizeof(vertices);
	
    for (int i = 0; i < n_triangles; i++)
    {
        prepare_VB0_VAO_triangle(VBO[i], VAO[i], vertexList[i], size_of_vertices);
    }

    // counter for chaging colors
    int counter = 0;
    int idx = 0;

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for (int i = 0; i < n_triangles; i += 2)
        {
            draw_triangle(shaderProgramList[(idx + i/2) % n_shaders], VAO[i]);
            draw_triangle(shaderProgramList[(idx + i/2) % n_shaders], VAO[i+1]);
        }

        counter++;
        idx = counter / 666;

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------

    for (int i = 0; i < n_triangles; i++)
    {
        glDeleteVertexArrays(1, &VAO[i]);
        glDeleteBuffers(1, &VBO[i]);
    }
	
    for (int i = 0; i < n_shaders; i++)
    {
        glDeleteProgram(shaderProgramList[i]);
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();

    //  free memory
    delete fragmentShaderSource;
    delete fragmentShaderSource1;
    delete fragmentShaderSource2;
    delete fragmentShaderSource3;
    delete fragmentShaderSource4;
    delete fragmentShaderSource5;
    delete fragmentShaderSource6;

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