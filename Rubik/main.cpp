#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <string>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const double PI = 3.1415926535;
int key_press_counter = 0;

enum drawType { Point = 0, Line = 1, Triangle = 2 };
enum axisType { X = 0, Y = 1, Z = 2 };


const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* fragmentShaderCreator(float R, float G, float B)
{
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

void get_rectangle_coords(float x1, float y1, float z1, float x2, float y2, float z2, float* v1, float* v2, const axisType & axisType)
{
     switch (axisType)
     {
         case Z:
             v1[0] = x1; v1[1] = y1; v1[2] = z1;       v1[3] = x2; v1[4] = y1; v1[5] = z1;       v1[6] = x1; v1[7] = y2; v1[8] = z1;
             v2[0] = x1; v2[1] = y2; v2[2] = z2;       v2[3] = x2; v2[4] = y2; v2[5] = z2;       v2[6] = x2; v2[7] = y1; v2[8] = z2;
             break;
         case Y:
             v1[0] = x1; v1[1] = y1; v1[2] = z1;       v1[3] = x2; v1[4] = y1; v1[5] = z1;       v1[6] = x1; v1[7] = y1; v1[8] = z2;
             v2[0] = x2; v2[1] = y2; v2[2] = z1;       v2[3] = x2; v2[4] = y2; v2[5] = z2;       v2[6] = x1; v2[7] = y2; v2[8] = z2;
             break;
         case X:
             v1[0] = x1; v1[1] = y1; v1[2] = z1;       v1[3] = x1; v1[4] = y2; v1[5] = z1;       v1[6] = x2; v1[7] = y1; v1[8] = z2;
             v2[0] = x2; v2[1] = y1; v2[2] = z2;       v2[3] = x2; v2[4] = y2; v2[5] = z2;       v2[6] = x2; v2[7] = y2; v2[8] = z1;
             break;
     }
    //v1[0] = x1; v1[1] = y1; v1[2] = z1;       v1[3] = x1; v1[4] = y2; v1[5] = z1;       v1[6] = x2; v1[7] = y1; v1[8] = z2;
	//v2[0] = x2; v2[1] = y2; v2[2] = z2;       v2[3] = x2; v2[4] = y1; v2[5] = z2;       v2[6] = x1; v2[7] = y2; v2[8] = z1;
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



void prepare_rotation_matrix(float vertices[4][4], float angle, const axisType &axisType)
{
	// Default values for rotation matrix
    vertices[0][0] = 1.0f; 	vertices[0][1] = 0.0f; 	vertices[0][2] = 0.0f;  vertices[0][3] = 0.0f;
	vertices[1][0] = 0.0f; 	vertices[1][1] = 1.0f; 	vertices[1][2] = 0.0f;  vertices[1][3] = 0.0f;
    vertices[2][0] = 0.0f; 	vertices[2][1] = 0.0f; 	vertices[2][2] = 1.0f;  vertices[2][3] = 0.0f;
	vertices[3][0] = 0.0f; 	vertices[3][1] = 0.0f; 	vertices[3][2] = 0.0f;  vertices[3][3] = 1.0f;
	
    float sine = sin(angle * ( PI/180 ));
    float cosine = cos(angle * (PI / 180));
	
    switch (axisType)
    {
		case X:
            vertices[1][1] = cosine;    vertices[1][2] = -sine;
			vertices[2][1] = sine;      vertices[2][2] = cosine;
            break;
		case Y:
			vertices[0][0] = cosine;    vertices[0][2] = sine;
			vertices[2][0] = -sine;     vertices[2][2] = cosine;
			break;
		case Z:
			vertices[0][0] = cosine;    vertices[0][1] = -sine;
			vertices[1][0] = sine;      vertices[1][1] = cosine;
			break;
    }	
}


// Math operations
void rotate(float* vertices, float angle, const axisType &axisType)
{
    float matrix[4][4];
    prepare_rotation_matrix(matrix, angle, axisType);
	
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

void prepare_cube_faces(float vertices[][9], int& index, const axisType& axisType, bool side)
{
    float x_coord, y_coord, z_coord;
	
    switch (axisType)
    {
    case Z:
        // Face in the X-Y plane
        z_coord = (side)? (- 0.6f) : (0.6f);
        y_coord = -0.6f;

        for (int i = 1; i < 4; i++)
        {
            x_coord = -0.6f;
            for (int j = 1; j < 4; j++)
            {
                get_rectangle_coords(x_coord, y_coord, z_coord, x_coord + 0.4, y_coord + 0.4, z_coord, vertices[index], vertices[index + 1], axisType::Z);
                x_coord += 0.4f;
                index += 2;
            }
            y_coord += 0.4f;
        }
        break;
    case Y:
        // Face in the X-Z plane
        y_coord = (side)? (- 0.6f) : (0.6f);
        z_coord = -0.6f;

        for (int i = 1; i < 4; i++)
        {
            x_coord = -0.6f;
            for (int j = 1; j < 4; j++)
            {
                get_rectangle_coords(x_coord, y_coord, z_coord, x_coord + 0.4, y_coord, z_coord + 0.4, vertices[index], vertices[index + 1], axisType::Y);
                x_coord += 0.4f;
                index += 2;
            }
            z_coord += 0.4f;
        }
        break;
    case X:
        // Face in the Y-Z plane
        x_coord = (side)? (- 0.6f) : (0.6f);
        z_coord = -0.6f;

        for (int i = 1; i < 4; i++)
        {
            y_coord = -0.6f;
            for (int j = 1; j < 4; j++)
            {
                get_rectangle_coords(x_coord, y_coord, z_coord, x_coord, y_coord + 0.4, z_coord + 0.4, vertices[index], vertices[index + 1], axisType::X);
                y_coord += 0.4f;
                index += 2;
            }
            z_coord += 0.4f;
        }
        break;
    }
	//std::cout<< x_coord<< ", "<< y_coord << ", " <<z_coord<<"\n";
}



// Shaders and programs
const char* fragmentShaderSourceRed     = fragmentShaderCreator(1.0f, 0.0f, 0.0f);    // red
const char* fragmentShaderSourceGreen   = fragmentShaderCreator(0.0f, 1.0f, 0.0f);    // green
const char* fragmentShaderSourceBlue    = fragmentShaderCreator(0.0f, 0.0f, 1.0f);    // blue
const char* fragmentShaderSourceOrange  = fragmentShaderCreator(1.0f, 0.5f, 0.0f);    // orange
const char* fragmentShaderSourceWhite   = fragmentShaderCreator(1.0f, 1.0f, 1.0f);    // white
const char* fragmentShaderSourceYellow  = fragmentShaderCreator(1.0f, 1.0f, 0.0f);    // yellow

std::vector<const char* > fragmentShaderSourceList = { fragmentShaderSourceRed, fragmentShaderSourceGreen, 
    fragmentShaderSourceBlue, fragmentShaderSourceOrange, fragmentShaderSourceWhite, fragmentShaderSourceYellow };

// Important variables

const int n_shaders = 6;
const int n_rectangles = 54;
// const int n_rectangles = 9;
const int n_triangles = n_rectangles * 2;
const float angle = 90.0f;

unsigned int VBO_triangles[n_triangles];
unsigned int VAO_triangles[n_triangles];

float vertices[n_triangles][9];




// Must be declared after the global variables
void move_cube(int& key_press_counter, const axisType& axisType)
{
    key_press_counter++;
    if (key_press_counter > 120)
    {
        key_press_counter = 0;
        for (int i = 0; i < n_triangles; i++)
            rotate(vertices[i], angle, axisType);

        for (int i = 0; i < n_triangles; i++)
        {
            prepare_VB0_VAO(VBO_triangles[i], VAO_triangles[i], vertices[i], sizeof(vertices[i]), drawType::Triangle);
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
    unsigned int vertexShader[n_shaders], fragmentShader[n_shaders], shaderProgram[n_shaders];
    for (int i = 0; i < n_shaders; i++)
	{
		prepare_shader(vertexShader[i], fragmentShader[i], vertexShaderSource, fragmentShaderSourceList[i], shaderProgram[i]);
	}
	

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    int index = 0;
	prepare_cube_faces(vertices, index, X, false);
	prepare_cube_faces(vertices, index, Y, false);
	prepare_cube_faces(vertices, index, Z, false);
	prepare_cube_faces(vertices, index, X, true);
	prepare_cube_faces(vertices, index, Y, true);
	prepare_cube_faces(vertices, index, Z, true);    
    
    for (int i = 0; i < n_triangles; i++)
    {
		prepare_VB0_VAO(VBO_triangles[i], VAO_triangles[i], vertices[i], sizeof(vertices[i]), drawType::Triangle);
    }
    
	// print vertices
    // for (int i = 0; i < n_triangles; i++)
    // {
    	// std::cout << "Triangle " << i << ": ";
        // for (int j = 0; j < 9; j++)
        // {
            // std::cout << vertices[i][j] << " ";
        // }
        // std::cout << std::endl;
    // }
		
	
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
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw figures
        // ------------
        // 
        for (int i = 0; i < n_triangles; i += 2)
        {
            draw(shaderProgram[(i/2) % 6], VAO_triangles[i], drawType::Triangle);
            draw(shaderProgram[(i/2) % 6], VAO_triangles[i + 1], drawType::Triangle);
        }
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    for (int i = 0; i < n_triangles; i++)
    {
		glDeleteVertexArrays(1, &VAO_triangles[i]);
		glDeleteBuffers(1, &VBO_triangles[i]);
    }
    for (int i = 0; i < n_shaders; i++)
    {
        glDeleteProgram(shaderProgram[i]);
        delete fragmentShaderSourceList[i];
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

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        move_cube(key_press_counter, X);
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        move_cube(key_press_counter, Y);
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        move_cube(key_press_counter, Z);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}