#include <math.h>
#include <vector>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"

#define PI 3.1415926535

// camera system
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// rotation
float xRot = 0.0f ;
float yRot = 0.0f ;
float zRot = 0.0f ;

enum class rotation_axis : unsigned short { X = 0, Y = 1, Z = 2 };
enum class rotation_type {
    TOP, BOTTOM,
    RIGHT, LEFT,
    FRONT, BACK,
    CENTER_X, CENTER_Y, CENTER_Z
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

std::map<std::string, glm::vec3> colors = {
    { "red",     glm::vec3(1.0f, 0.0f, 0.0f) },
    { "yellow",  glm::vec3(1.0f, 1.0f, 0.0f) },
    { "green",   glm::vec3(0.0f, 1.0f, 0.0f) },
    { "blue",    glm::vec3(0.0f, 0.0f, 1.0f) },
    { "white",   glm::vec3(1.0f, 1.0f, 1.0f) },
    { "orange",  glm::vec3(1.0f, 0.5f, 0.0f) },
    { "gray",    glm::vec3(0.5f, 0.5f, 0.5f) },
};


namespace op
{
    rotation_axis get_axis(face f)
    {
        // array of all vertices
        std::vector<vertex> vertices= { f.a1, f.a2, f.a3,   f.b1, f.b2, f.b3 };

        float x_min = (*std::min_element(vertices.begin(), vertices.end(), [](vertex a, vertex b) { return a.x < b.x; })).x;
        float x_max = (*std::max_element(vertices.begin(), vertices.end(), [](vertex a, vertex b) { return a.x < b.x; })).x;
        float y_min = (*std::min_element(vertices.begin(), vertices.end(), [](vertex a, vertex b) { return a.y < b.y; })).y;
        float y_max = (*std::max_element(vertices.begin(), vertices.end(), [](vertex a, vertex b) { return a.y < b.y; })).y;
        float z_min = (*std::min_element(vertices.begin(), vertices.end(), [](vertex a, vertex b) { return a.z < b.z; })).z;
        float z_max = (*std::max_element(vertices.begin(), vertices.end(), [](vertex a, vertex b) { return a.z < b.z; })).z;

        if (abs(x_max - x_min) < 0.02f)
            return rotation_axis::X;
        else if (abs(y_max - y_min) < 0.02f)
            return rotation_axis::Y;
        else if (abs(z_max - z_min) < 0.02f)
            return rotation_axis::Z;
    }


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

    void rotate(std::vector<vertex>& vertices, float angle, rotation_axis axis)
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


    void prepare_VBO_VAO(unsigned int& VBO, unsigned int& VAO, float vertices[], int size_of_vertices)
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



    void draw(Shader &shader, unsigned int& VAO, glm::mat4 proj, glm::vec3 &color)
    {
        shader.use();

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        glm::mat4 model = glm::mat4(1.0f);

        model = glm::rotate(model, xRot, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, yRot, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, zRot, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 MVP = proj * view * model;

        shader.setMat4("view", view);
        shader.setMat4("u_MVP", MVP);

        // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glBindVertexArray(VAO);

        shader.setVec3("u_faceColor", color);
        
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0); // no need to unbind it every time 
    }
}