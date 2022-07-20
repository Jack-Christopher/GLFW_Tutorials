#include <math.h>
#include <vector>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"

#define PI 3.1415926535


enum class rotation_axis : unsigned short { X = 0, Y = 1, Z = 2 };

struct vertex
{
    float x;
    float y;
    float z;
};

// a color for each triangle
std::map<std::string, glm::vec3> colors = {
    { "red",     glm::vec3(1.0f, 0.0f, 0.0f) },
    { "green",   glm::vec3(0.0f, 1.0f, 0.0f) },
    { "blue",    glm::vec3(0.0f, 0.0f, 1.0f) },
    { "yellow",  glm::vec3(1.0f, 1.0f, 0.0f) },
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

    void translate(std::vector<vertex>& vertices, float x, float y, float z)
    {
        std::vector<std::vector<float>> translation_matrix;
        translation_matrix = get_matrix();
        translation_matrix[0][3] = x;
        translation_matrix[1][3] = y;
        translation_matrix[2][3] = z;

        for (int i = 0; i < vertices.size(); i++)
        {
            std::vector<float> temp = { vertices[i].x, vertices[i].y, vertices[i].z, 1.0f };
            multiply(translation_matrix, temp);
            vertices[i].x = temp[0];
            vertices[i].y = temp[1];
            vertices[i].z = temp[2];
        }
    }

    void scale(std::vector<vertex>& vertices, float x, float y, float z)
    {
        std::vector<std::vector<float>> scale_matrix;
        scale_matrix = get_matrix();
        scale_matrix[0][0] = x;
        scale_matrix[1][1] = y;
        scale_matrix[2][2] = z;

        for (int i = 0; i < vertices.size(); i++)
        {
            std::vector<float> temp = { vertices[i].x, vertices[i].y, vertices[i].z, 1.0f };
            multiply(scale_matrix, temp);
            vertices[i].x = temp[0];
            vertices[i].y = temp[1];
            vertices[i].z = temp[2];
        }
    }
}