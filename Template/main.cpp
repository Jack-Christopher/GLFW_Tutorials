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

enum drawType { Point = 0, Line = 1, Triangle = 2 };


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


void prepare_VB0_VAO(unsigned int& VBO, unsigned int& VAO, float vertices[], int size_of_vertices, const drawType& drawType)
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



void draw(unsigned int& shaderProgram, unsigned int& VAO, const drawType& drawType, float pointSize = 10.0f)
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



//void multiply(float vertices1[3][3], float vertices2[3][1])
void multiply(float vertices1[3][3], std::vector<float> &vertices2)
{
    // multiply 3x3 matrix vertice1 and 3x1 matrix vertices2
    float result[3];
    for (int i = 0; i < 3; i++)
    {
        result[i] = 0;
        for (int j = 0; j < 3; j++)
        {
            result[i] += vertices1[i][j] * vertices2[j];
        }
    }
    // copy result to vertices2
    for (int i = 0; i < 3; i++)
    {
        vertices2[i] = result[i];
    }
}


// Matrix preparations
void prepare_rotation_matrix(float matrix[3][3], float angle)
{
    float sine = sin(angle * (PI / 180));
    float cosine = cos(angle * (PI / 180));

    matrix[0][0] = cosine;    matrix[0][1] = -sine;     matrix[0][2] = 0.0f;
    matrix[1][0] = sine;      matrix[1][1] = cosine;    matrix[1][2] = 0.0f;
    matrix[2][0] = 0.0f;      matrix[2][1] = 0.0f;      matrix[2][2] = 1.0f;
}

void prepare_translation_matrix(float matrix[3][3], float x, float y)
{
	matrix[0][0] = 1.0f;    matrix[0][1] = 0.0f;     matrix[0][2] = x;
	matrix[1][0] = 0.0f;    matrix[1][1] = 1.0f;     matrix[1][2] = y;
	matrix[2][0] = 0.0f;    matrix[2][1] = 0.0f;     matrix[2][2] = 1.0f;
}

void prepare_scaling_matrix(float matrix[3][3], float x, float y)
{
	matrix[0][0] = x;    matrix[0][1] = 0.0f;     matrix[0][2] = 0.0f;
	matrix[1][0] = 0.0f;    matrix[1][1] = y;     matrix[1][2] = 0.0f;
	matrix[2][0] = 0.0f;    matrix[2][1] = 0.0f;     matrix[2][2] = 1.0f;
}


// Math operations
void rotate(float* vertices, float angle, const drawType& drawType)
{
    int n_vertices;
    if (drawType == Point)          n_vertices = 1;
    else if (drawType == Line)      n_vertices = 2;
    else if (drawType == Triangle)  n_vertices = 3;
	
    float matrix[3][3];
    prepare_rotation_matrix(matrix, angle);

    for (int i = 0; i < n_vertices; i++)
    {
        std::vector<float>vert = { vertices[i * n_vertices], vertices[i * n_vertices + 1], 1.0f };
        multiply(matrix, vert);
        vertices[i * n_vertices] = vert[0];
        vertices[i * n_vertices + 1] = vert[1];
    }

}

void translate(float* vertices, float x, float y, const drawType& drawType)
{
    int n_vertices;
	if (drawType == Point)          n_vertices = 1;
	else if (drawType == Line)      n_vertices = 2;
	else if (drawType == Triangle)  n_vertices = 3;
	
	float matrix[3][3];
	prepare_translation_matrix(matrix, x, y);
	
	for (int i = 0; i < n_vertices; i++)
	{
        std::vector<float>vert = { vertices[i * n_vertices], vertices[i * n_vertices + 1], 1.0f };
		multiply(matrix, vert);
		vertices[i * n_vertices] = vert[0];
        vertices[i * n_vertices + 1] = vert[1];
	}
}

void scale(float* vertices, float x, float y, const drawType& drawType)
{
	int n_vertices;
	if (drawType == Point)          n_vertices = 1;
	else if (drawType == Line)      n_vertices = 2;
	else if (drawType == Triangle)  n_vertices = 3;
	
	float matrix[3][3];
	prepare_scaling_matrix(matrix, x, y);
	
	for (int i = 0; i < n_vertices; i++)
	{
		std::vector<float>vert = { vertices[i * n_vertices], vertices[i * n_vertices + 1], 1.0f };
		multiply(matrix, vert);
		vertices[i * n_vertices] = vert[0];
		vertices[i * n_vertices + 1] = vert[1];
	}
}


void get_point_coords(float x, float y, float vertices[][2], size_t& index)
{
    vertices[index][0] = x; 	vertices[index][1] = y;

    index++;
}


void get_line_coords(float x1, float y1, float x2, float y2, float vertices[][4], size_t& index)
{
	vertices[index][0] = x1; 	vertices[index][1] = y1;
	vertices[index][2] = x2; 	vertices[index][3] = y2;

    index++;
}

void get_triangle_coords(float x1, float y1, float x2, float y2, float x3, float y3, float vertices[][9], size_t& index)
{
    vertices[index][0] = x1; 	vertices[index][1] = y1; 	vertices[index][2] = 0.0f;
    vertices[index][3] = x2; 	vertices[index][4] = y2; 	vertices[index][5] = 0.0f;
    vertices[index][6] = x3; 	vertices[index][7] = y3; 	vertices[index][8] = 0.0f;
	
    index++;
}

void get_rectangle_coords(float x1, float y1, float x2, float y2, float vertices[][9], size_t &index)
{
    vertices[index][0] = x1; 	vertices[index][1] = y1; 	vertices[index][2] = 0.0f;
    vertices[index][3] = x2; 	vertices[index][4] = y1; 	vertices[index][5] = 0.0f;
    vertices[index][6] = x1; 	vertices[index][7] = y2; 	vertices[index][8] = 0.0f;

    vertices[index + 1][0] = x1; 	vertices[index + 1][1] = y2; 	vertices[index + 1][2] = 0.0f;
    vertices[index + 1][3] = x2; 	vertices[index + 1][4] = y2; 	vertices[index + 1][5] = 0.0f;
    vertices[index + 1][6] = x2; 	vertices[index + 1][7] = y1; 	vertices[index + 1][8] = 0.0f;

	index += 2;
}

void get_regular_coords(int n_sides, float x, float y, float radius, float vertices[][4], size_t &index)
{
	double angle = 360.0f / n_sides;
	angle *= (PI / 180);
	
    vertices[index][0] = x + radius * cos(index * angle);
    vertices[index][1] = y + radius * sin(index * angle);

	for (int i = 0; i < n_sides; i++)
	{
        vertices[index + i][0] = x + radius * cos(i * angle);
        vertices[index + i][1] = y + radius * sin(i * angle);

        vertices[index + i][2] = x + radius * cos((i + 1) * angle);
        vertices[index + i][3] = y + radius * sin((i + 1) * angle);
	}
	index += n_sides;
}


// Shaders and programs
const char* fragmentShaderSourceRed = fragmentShaderCreator(1.0f, 0.0f, 0.0f);      // red
const char* fragmentShaderSourceGreen = fragmentShaderCreator(0.0f, 1.0f, 0.0f);    // green
const char* fragmentShaderSourceBlue = fragmentShaderCreator(0.0f, 0.0f, 1.0f);     // blue
const char* fragmentShaderSourceOrange = fragmentShaderCreator(1.0f, 0.5f, 0.0f);   // orange
const char* fragmentShaderSourceWhite = fragmentShaderCreator(1.0f, 1.0f, 1.0f);    // white
const char* fragmentShaderSourceYellow = fragmentShaderCreator(1.0f, 1.0f, 0.0f);   // yellow
const char* fragmentShaderSourceBlack = fragmentShaderCreator(0.0f, 0.0f, 0.0f);    // black
const char* fragmentShaderSourceSand = fragmentShaderCreator(0.82f, 0.66f, 0.43f);  // sand
const char* fragmentShaderSourceSkyBlue = fragmentShaderCreator(0.0f, 0.7f, 1.0f);  // sky blue

std::vector<const char* > fragmentShaderSourceList = { fragmentShaderSourceRed, fragmentShaderSourceGreen, fragmentShaderSourceBlue, 
    fragmentShaderSourceOrange, fragmentShaderSourceWhite, fragmentShaderSourceYellow, fragmentShaderSourceBlack, 
	fragmentShaderSourceSand, fragmentShaderSourceSkyBlue };


const int n_shaders = 9;
const int n_triangles = 20;
const int n_lines = 10;
const int n_points = 1;

unsigned int VBO_triangles[n_triangles];
unsigned int VAO_triangles[n_triangles];
unsigned int VBO_lines[n_lines];
unsigned int VAO_lines[n_lines];
unsigned int VBO_points[n_points];
unsigned int VAO_points[n_points];

float verticesTriangles[n_triangles][9];
float verticesLines[n_lines][4];
float verticesPoints[n_points][2];


// Must be declared after the global variables
void rotate_figure(std::vector<int> indices, float angle, const drawType& drawType, int &timer)
{
    if (timer % 200 == 0)
    {
        switch (drawType)
        {
        case Line:
            for (int i = 0; i < indices.size(); i++)
                rotate(verticesLines[indices[i]], angle, drawType::Line);
            for (int i = 0; i < indices.size(); i++)
                prepare_VB0_VAO(VBO_lines[indices[i]], VAO_lines[indices[i]], verticesLines[indices[i]], 4*sizeof(float), drawType::Line);
            break;
        case Triangle:
            for (int i = 0; i < indices.size(); i++)
                rotate(verticesTriangles[indices[i]], angle, drawType::Triangle);
            for (int i = 0; i < indices.size(); i++)
                prepare_VB0_VAO(VBO_triangles[indices[i]], VAO_triangles[indices[i]], verticesTriangles[indices[i]], 9*sizeof(float), drawType::Triangle);
            break;			
        }
        timer = 1;
    }
    else
    {
        timer++;
    }
}

void translate_figure(std::vector<int> indices, float x, float y, const drawType& drawType, int &timer)
{
	if (timer % 200 == 0)
	{
		switch (drawType)
		{
		case Line:
			for (int i = 0; i < indices.size(); i++)
				translate(verticesLines[indices[i]], x, y, drawType::Line);
			for (int i = 0; i < indices.size(); i++)
				prepare_VB0_VAO(VBO_lines[indices[i]], VAO_lines[indices[i]], verticesLines[indices[i]], 4*sizeof(float), drawType::Line);
			break;
		case Triangle:
			for (int i = 0; i < indices.size(); i++)
				translate(verticesTriangles[indices[i]], x, y, drawType::Triangle);
			for (int i = 0; i < indices.size(); i++)
				prepare_VB0_VAO(VBO_triangles[indices[i]], VAO_triangles[indices[i]], verticesTriangles[indices[i]], 9*sizeof(float), drawType::Triangle);
			break;
		}
		timer = 1;
	}
	else
	{
		timer++;
	}
}

void scale_figure(std::vector<int> indices, float x, float y, const drawType& drawType, int &timer)
{
	if (timer % 200 == 0)
	{
		switch (drawType)
		{
		case Line:
			for (int i = 0; i < indices.size(); i++)
				scale(verticesLines[indices[i]], x, y, drawType::Line);
			for (int i = 0; i < indices.size(); i++)
				prepare_VB0_VAO(VBO_lines[indices[i]], VAO_lines[indices[i]], verticesLines[indices[i]], 4*sizeof(float), drawType::Line);
			break;
		case Triangle:
			for (int i = 0; i < indices.size(); i++)
				scale(verticesTriangles[indices[i]], x, y, drawType::Triangle);
			for (int i = 0; i < indices.size(); i++)
				prepare_VB0_VAO(VBO_triangles[indices[i]], VAO_triangles[indices[i]], verticesTriangles[indices[i]], 9*sizeof(float), drawType::Triangle);
			break;
		}
		timer = 1;
	}
	else
	{
		timer++;
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Transformaciones", NULL, NULL);
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
    size_t index_triangles = 0;
    size_t index_lines = 0;
    size_t index_points = 0;

	// Arena
    get_rectangle_coords(-1.0f, -1.0f, 1.0f, -0.7f, verticesTriangles, index_triangles);
    // Mar
    get_rectangle_coords(-1.0f, -0.7f, 1.0f, -0.4f, verticesTriangles, index_triangles);
    // Cielo
    get_rectangle_coords(-1.0f, -0.4f, 1.0f,  1.0f, verticesTriangles, index_triangles);
	// Avion
    get_triangle_coords(0.5f, 0.3f, 0.45f, 0.2f, 0.55f, 0.2f, verticesTriangles, index_triangles);
	get_rectangle_coords(0.45f, -0.1f, 0.55f, 0.2f, verticesTriangles, index_triangles);
	get_triangle_coords(0.5f, 0.2f, 0.3f, 0.0f, 0.7f, 0.0f, verticesTriangles, index_triangles);
	// Barco
	get_triangle_coords(-1.2f, -0.2f, -0.8f, -0.2f, -1.0f, -0.5f, verticesTriangles, index_triangles);
	get_triangle_coords(-1.0f, -0.5f, -0.8f, -0.2f, -0.6f, -0.5f, verticesTriangles, index_triangles);
	get_triangle_coords(-0.8f, -0.2f, -0.4f, -0.2f, -0.6f, -0.5f, verticesTriangles, index_triangles);
	get_line_coords(-0.8f, -0.2f, -0.8, 0.2f, verticesLines, index_lines);
	get_rectangle_coords(-0.8f, 0.0f, -0.6f, 0.1f, verticesTriangles, index_triangles);
	// Molino
    get_triangle_coords(0.0f, 0.0f, 0.0f, 0.4f, 0.2f, 0.0f, verticesTriangles, index_triangles);
	get_triangle_coords(0.0f, 0.0f, 0.4f, 0.0f, 0.0f, -0.2f, verticesTriangles, index_triangles);
	get_triangle_coords(0.0f, 0.0f, 0.0f, -0.4f, -0.2f, 0.0f, verticesTriangles, index_triangles);
	get_triangle_coords(0.0f, 0.0f, -0.4f, 0.0f, 0.0f, 0.2f, verticesTriangles, index_triangles);
    get_line_coords(0.0f, -0.8f, 0.0f, 0.0f, verticesLines, index_lines);
    get_point_coords(0.0f, 0.0f, verticesPoints, index_points);
	
    // get_regular_coords(12, 0.5f, 0.5f, 0.3f, verticesLines, index_lines);

    for (int i = 0; i < n_triangles; i++)
        prepare_VB0_VAO(VBO_triangles[i], VAO_triangles[i], verticesTriangles[i], sizeof(verticesTriangles[i]), drawType::Triangle);
    for (int i = 0; i < n_lines; i++)
        prepare_VB0_VAO(VBO_lines[i], VAO_lines[i], verticesLines[i], sizeof(verticesLines[i]), drawType::Line);
    for (int i = 0; i < n_points; i++)
        prepare_VB0_VAO(VBO_points[i], VAO_points[i], verticesPoints[i], sizeof(verticesPoints[i]), drawType::Point);


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    int timer_mar = 1;
    int timer_barco = 1;
    int timer_barco_ = 1;
    int timer_molino = 1;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw figures
        // ------------
        
		// Arena
        draw(shaderProgram[7], VAO_triangles[0], drawType::Triangle);
        draw(shaderProgram[7], VAO_triangles[1], drawType::Triangle);
		// Mar
		draw(shaderProgram[2], VAO_triangles[2], drawType::Triangle);
		draw(shaderProgram[2], VAO_triangles[3], drawType::Triangle);
        // Cielo
        draw(shaderProgram[8], VAO_triangles[4], drawType::Triangle);
        draw(shaderProgram[8], VAO_triangles[5], drawType::Triangle);
		// Avion
        draw(shaderProgram[3], VAO_triangles[6], drawType::Triangle);
		draw(shaderProgram[3], VAO_triangles[7], drawType::Triangle);
		draw(shaderProgram[3], VAO_triangles[8], drawType::Triangle);
		draw(shaderProgram[3], VAO_triangles[9], drawType::Triangle);
        std::vector<int> mar = { 6, 7, 8, 9 };
        scale_figure(mar, 1.0f, 0.9f, drawType::Triangle, timer_mar);
		// Barco
		draw(shaderProgram[1], VAO_triangles[10], drawType::Triangle);
		draw(shaderProgram[1], VAO_triangles[11], drawType::Triangle);
		draw(shaderProgram[1], VAO_triangles[12], drawType::Triangle);
		draw(shaderProgram[6], VAO_lines[0], drawType::Line);
		draw(shaderProgram[1], VAO_triangles[13], drawType::Triangle);
		draw(shaderProgram[1], VAO_triangles[14], drawType::Triangle);
        std::vector<int> barco = { 10, 11, 12, 13, 14 };
        std::vector<int> barco_ = { 0 };
        translate_figure(barco, 0.01f, 0.0f, drawType::Triangle, timer_barco);
		translate_figure(barco_, 0.01f, 0.0f, drawType::Line, timer_barco_);
		// Molino de Viento
        draw(shaderProgram[6], VAO_lines[1], drawType::Line);
        draw(shaderProgram[0], VAO_triangles[15], drawType::Triangle);
        draw(shaderProgram[0], VAO_triangles[16], drawType::Triangle);
        draw(shaderProgram[0], VAO_triangles[17], drawType::Triangle);
        draw(shaderProgram[0], VAO_triangles[18], drawType::Triangle);
        draw(shaderProgram[3], VAO_points[0], drawType::Point, 15.0f);
        std::vector<int> molino = { 15, 16, 17, 18};
        rotate_figure(molino, 5.0f, drawType::Triangle, timer_molino);		


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
		
		glDeleteVertexArrays(1, &VAO_lines[i]);
		glDeleteBuffers(1, &VBO_lines[i]);
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
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}