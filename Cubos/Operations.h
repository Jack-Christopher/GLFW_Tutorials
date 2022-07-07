#include <math.h>
#include <vector>

#define PI 3.1415926535

enum class rotation_axis : unsigned short { X = 0, Y = 1, Z = 2 };
enum drawType { Point = 0, Line = 1, Triangle = 2 };

/*
enum class rotation_type {
    TOP, BOTTOM,
    RIGHT, LEFT,
    FRONT, BACK,
    CENTER_X, CENTER_Y, CENTER_Z
};
*/

struct color
{
    float R;
    float G;
    float B;
};

struct vertex
{
    float x;
    float y;
    float z;
};

struct face
{
    vertex a1;
	vertex a2;
	vertex a3;
	vertex b1;
	vertex b2;
	vertex b3;	

    std::string color_name = "gray";
};

struct source
{
    const char* fragmentShaderSource;
    unsigned int vertexShader;
    unsigned int fragmentShader;
    unsigned int shaderProgram;
};

std::map<std::string, color> colors = {
    {"red", {1.0f, 0.0f, 0.0f}},
    {"yellow", {1.0f, 1.0f, 0.0f}},
    {"green", {0.0f, 1.0f, 0.0f}},
    {"blue", {0.0f, 0.0f, 1.0f}},
    {"white", {1.0f, 1.0f, 1.0f}},
    {"orange", {1.0f, 0.5f, 0.0f}},
	{"gray", {0.5f, 0.5f, 0.5f}},
};



namespace op
{

    // Math operations
    void multiply(std::vector<std::vector<float>> matrix, std::vector<float>& vertices)
    {
        // multiply matrix by vertices
        float result[4];
        for (int i = 0; i < 4; i++)
        {
            result[i] = 0;
            for (int j = 0; j < 4; j++)
            {
                result[i] += matrix[i][j] * vertices[j];
            }
        }

        // copy result to vertices, overwriting old vertices
        // only to x, y, z components
        for (int i = 0; i < 3; i++)
            vertices[i] = result[i];
    }


    std::vector<std::vector<float>> get_matrix()
    {
        std::vector<std::vector<float>> matrix = {
            {1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
        };

        return matrix;
    }

    std::vector<std::vector<float>> get_rotation_matrix(float angle, rotation_axis axis)
    {
        float sine = sin(angle * (PI / 180));
        float cosine = cos(angle * (PI / 180));
        std::vector<std::vector<float>> matrix;
        matrix = get_matrix();

        switch (axis)
        {
        case rotation_axis::X:
            matrix[1][1] = cosine;    matrix[1][2] = -sine;
            matrix[2][1] = sine;      matrix[2][2] = cosine;
            break;
        case rotation_axis::Y:
            matrix[0][0] = cosine;    matrix[0][2] = sine;
            matrix[2][0] = -sine;    matrix[2][2] = cosine;
            break;
        case rotation_axis::Z:
            matrix[0][0] = cosine;    matrix[0][1] = -sine;
            matrix[1][0] = sine;      matrix[1][1] = cosine;
            break;
        default:
            break;
        }

        return matrix;
    }

    std::vector<std::vector<float>> get_translation_matrix(float x, float y, float z)
    {
        std::vector<std::vector<float>> matrix = get_matrix();
        matrix[0][3] = x;
        matrix[1][3] = y;
        matrix[2][3] = z;
        return matrix;
    }

    void rotate(std::vector<vertex> &vertices, float angle, rotation_axis axis)
    {
        std::vector<std::vector<float>> rotation_matrix;
        rotation_matrix = get_rotation_matrix(angle, axis);

        for (int i = 0; i < vertices.size(); i++)
        {
            std::vector<float> temp = { vertices[i].x, vertices[i].y, vertices[i].z, 1.0f };
            multiply(rotation_matrix, temp);
			vertices[i].x = temp[0];
			vertices[i].y = temp[1];
			vertices[i].z = temp[2];
        }
    }

    void translate(std::vector<vertex> &vertices, float x, float y, float z)
    {
        std::vector<std::vector<float>> translation_matrix;
        translation_matrix = get_translation_matrix(x, y, z);
        
        for (int i = 0; i < vertices.size(); i++)
        {
            std::vector<float> temp = { vertices[i].x, vertices[i].y, vertices[i].z, 1.0f };
            multiply(translation_matrix, temp);
            vertices[i].x = temp[0];
            vertices[i].y = temp[1];
            vertices[i].z = temp[2];
        }
    }



    

    void get_rectangle_coords(float x1, float y1, float z1, float x2, float y2, float z2, float* v1, float* v2, rotation_axis axis)
    {
        switch (axis)
        {
        case rotation_axis::Z:
            v1[0] = x1; v1[1] = y1; v1[2] = z1;       v1[3] = x2; v1[4] = y1; v1[5] = z1;       v1[6] = x1; v1[7] = y2; v1[8] = z1;
            v2[0] = x1; v2[1] = y2; v2[2] = z2;       v2[3] = x2; v2[4] = y2; v2[5] = z2;       v2[6] = x2; v2[7] = y1; v2[8] = z2;
            break;
        case rotation_axis::Y:
            v1[0] = x1; v1[1] = y1; v1[2] = z1;       v1[3] = x2; v1[4] = y1; v1[5] = z1;       v1[6] = x1; v1[7] = y1; v1[8] = z2;
            v2[0] = x2; v2[1] = y2; v2[2] = z1;       v2[3] = x2; v2[4] = y2; v2[5] = z2;       v2[6] = x1; v2[7] = y2; v2[8] = z2;
            break;
        case rotation_axis::X:
            v1[0] = x1; v1[1] = y1; v1[2] = z1;       v1[3] = x1; v1[4] = y2; v1[5] = z1;       v1[6] = x2; v1[7] = y1; v1[8] = z2;
            v2[0] = x2; v2[1] = y1; v2[2] = z2;       v2[3] = x2; v2[4] = y2; v2[5] = z2;       v2[6] = x2; v2[7] = y2; v2[8] = z1;
            break;
        }
    }


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
}