#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "Rubik.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
const char* fragmentShaderCreator(color c);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int key_press_counter = 0;
const int iteration_times = 25;

enum drawType { Point = 0, Line = 1, Triangle = 2 };


std::map<std::string, const char*> fragmentShaderSources = {
    {"blue", fragmentShaderCreator(colors["blue"])},
    {"green", fragmentShaderCreator(colors["green"])},
    {"orange", fragmentShaderCreator(colors["orange"])},
    {"red", fragmentShaderCreator(colors["red"])},
    {"white", fragmentShaderCreator(colors["white"])},
    {"yellow", fragmentShaderCreator(colors["yellow"])},
	{"gray", fragmentShaderCreator(colors["gray"])},
};

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

std::map<std::string, unsigned int> shaderPrograms = {
    {"blue", 0},
    {"green", 0},
    {"orange", 0},
    {"red", 0},
    {"white", 0},
    {"yellow", 0},
	{"gray", 0},
};



const char* fragmentShaderCreator(color c)
{
    std::string temp = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(" + std::to_string(c.R) + "f, " + std::to_string(c.G) + "f, " + std::to_string(c.B) + "f, 1.0f);\n"
        "}\0";
    const char* fragmentShader = new char[temp.length() + 1];
    strcpy(const_cast<char*>(fragmentShader), temp.c_str());
    return fragmentShader;
}


void prepare_shader(unsigned int &vertexShader, unsigned int &fragmentShader, const char* &vertexShaderSource, const char* &fragmentShaderSource,  unsigned int &shaderProgram)
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


void prepare_VB0_VAO(unsigned int& VBO, unsigned int& VAO, float vertices[], int size_of_vertices, const drawType &drawType)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, size_of_vertices, vertices, GL_STATIC_DRAW);

    switch (drawType)
    {
		case Point:
		    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
		    break;
		case Line:
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            break;
        case Triangle:
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            break;		
    }
	
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
}



void draw(unsigned int& shaderProgram, unsigned int& VAO, const drawType &drawType, float pointSize = 10.0f)
{
    glUseProgram(shaderProgram);
    // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glBindVertexArray(VAO); 
	
    switch (drawType)
    {
    case Point:
        glDrawArrays(GL_POINTS, 0, 1);
        glPointSize(pointSize);
        break;
    case Line:
        glDrawArrays(GL_LINES, 0, 2);
        break;
    case Triangle:
        glDrawArrays(GL_TRIANGLES, 0, 3);
        break;
    }
    // glBindVertexArray(0); // no need to unbind it every time 
}


void multiply(float vertices1[4][4], float vertices2[4][1])
{
	// multiply 4x4 matrix vertice1 and 4x1 matrix vertices2
    float result[4];
	for (int i = 0; i < 4; i++)
	{
		result[i] = 0;
		for (int j = 0; j < 4; j++)
		{
			result[i] += vertices1[i][j] * vertices2[j][0];
		}
	}
	// copy result to vertices2
	for (int i = 0; i < 4; i++)
	{
		vertices2[i][0] = result[i];
	}
}



void prepare_rotation_matrix(float vertices[4][4], float angle, rotation_axis axis)
{
	// Default values for rotation matrix
    vertices[0][0] = 1.0f; 	vertices[0][1] = 0.0f; 	vertices[0][2] = 0.0f;  vertices[0][3] = 0.0f;
	vertices[1][0] = 0.0f; 	vertices[1][1] = 1.0f; 	vertices[1][2] = 0.0f;  vertices[1][3] = 0.0f;
    vertices[2][0] = 0.0f; 	vertices[2][1] = 0.0f; 	vertices[2][2] = 1.0f;  vertices[2][3] = 0.0f;
	vertices[3][0] = 0.0f; 	vertices[3][1] = 0.0f; 	vertices[3][2] = 0.0f;  vertices[3][3] = 1.0f;
	
    float sine = sin(angle * ( PI/180 ));
    float cosine = cos(angle * (PI / 180));
	
    switch (axis)
    {
		case rotation_axis::X:
            vertices[1][1] = cosine;    vertices[1][2] = -sine;
			vertices[2][1] = sine;      vertices[2][2] = cosine;
            break;
		case rotation_axis::Y:
			vertices[0][0] = cosine;    vertices[0][2] = sine;
			vertices[2][0] = -sine;     vertices[2][2] = cosine;
			break;
		case rotation_axis::Z:
			vertices[0][0] = cosine;    vertices[0][1] = -sine;
			vertices[1][0] = sine;      vertices[1][1] = cosine;
			break;
    }	
}


// Math operations
void rotate(float* vertices, float angle, rotation_axis axis)
{
    float matrix[4][4];
    prepare_rotation_matrix(matrix, angle, axis);
	
    float data1[4][1] = { {vertices[0]}, {vertices[1]}, {vertices[2]}, {1.0f} };
	float data2[4][1] = { {vertices[3]}, {vertices[4]}, {vertices[5]}, {1.0f} };
	float data3[4][1] = { {vertices[6]}, {vertices[7]}, {vertices[8]}, {1.0f} };
	
    multiply(matrix, data1);
	multiply(matrix, data2);
	multiply(matrix, data3);
	
	vertices[0] = data1[0][0];  vertices[1] = data1[1][0];  vertices[2] = data1[2][0];
	vertices[3] = data2[0][0];  vertices[4] = data2[1][0];  vertices[5] = data2[2][0];
	vertices[6] = data3[0][0];  vertices[7] = data3[1][0];  vertices[8] = data3[2][0];
}


// Important variables

const int n_shaders = 7;
// const float angle = 1.0f;

Rubik rubik;


void move_cube(int& key_press_counter, float angle, rotation_axis axis)
{
    key_press_counter++;
    if (key_press_counter > iteration_times)
    {
        key_press_counter = 0;
        // Cube loops
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    Cube &cube = rubik.matrix[i][j][k];
                    cube.rotate(angle, axis);
                    cube.set_faces();

                    // faces loop
                    for (int v = 0; v < 6; v++)
                    {
                        prepare_VB0_VAO(cube.VBO[v * 2], cube.VAO[v * 2], &cube.data[v * 2][0], 9 * sizeof(float), drawType::Triangle);
                        prepare_VB0_VAO(cube.VBO[v * 2 + 1], cube.VAO[v * 2 + 1], &cube.data[v * 2 + 1][0], 9 * sizeof(float), drawType::Triangle);
                    }
                }
            }
        }
    }
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
    std::vector<unsigned int> vertexShaders(fragmentShaderSources.size());
    std::vector<unsigned int> fragmentShaders(fragmentShaderSources.size());

    int idx = 0;
    for (auto& f : fragmentShaderSources)
    {
        prepare_shader(vertexShaders[idx], fragmentShaders[idx], vertexShaderSource, f.second, shaderPrograms[f.first]);
        idx++;
    }


    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

	// Cube loops
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                Cube &cube = rubik.matrix[i][j][k];
                // faces loop
                for (int v = 0; v < 6; v++)
                {
                    prepare_VB0_VAO(cube.VBO[v * 2], cube.VAO[v * 2], &cube.data[v * 2][0], 9 * sizeof(float), drawType::Triangle);
                    prepare_VB0_VAO(cube.VBO[v * 2 + 1], cube.VAO[v * 2 + 1], &cube.data[v * 2 + 1][0], 9 * sizeof(float), drawType::Triangle);
                }
            }
        }
    }
    
    // Habilidad el test de profundidad
    glEnable(GL_DEPTH_TEST);
    // Aceptar el fragmento si está más cerca de la cámara que el fragmento anterior
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
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw figures
        // ------------

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    Cube& cube = rubik.matrix[i][j][k];
                    // faces loop
                    for (int v = 0; v < 6; v++)
                    {
                        //std::cout << cube.faces[v].color_name << " ";
                        draw(shaderPrograms[cube.faces[v].color_name], cube.VAO[v * 2], drawType::Triangle);
                        draw(shaderPrograms[cube.faces[v].color_name], cube.VAO[v * 2 + 1], drawType::Triangle);
                    }
                }
            }
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------

    for (int i = 0; i < 3; i++)
    {
       for (int j = 0; j < 3; j++)
       {
            for (int k = 0; k < 3; k++)
            {
                for (int v = 0; v < 6; v++)
                {
                    Cube& cube = rubik.matrix[i][j][k];
                    glDeleteVertexArrays(1, &cube.VAO[v]);
                    glDeleteBuffers(1, &cube.VBO[v]);
                }
            }
        }
    }
	

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
        move_cube(key_press_counter, 1.0f, rotation_axis::X);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		move_cube(key_press_counter, -1.0f, rotation_axis::X);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        move_cube(key_press_counter, 1.0f, rotation_axis::Y);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		move_cube(key_press_counter, -1.0f, rotation_axis::Y);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        move_cube(key_press_counter, 1.0f, rotation_axis::Z);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		move_cube(key_press_counter, -1.0f, rotation_axis::Z);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}