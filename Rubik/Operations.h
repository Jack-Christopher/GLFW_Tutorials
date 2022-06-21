#include <math.h>
#include <vector>

#define PI 3.1415926535

enum class rotation_axis : unsigned short { X = 0, Y = 1, Z = 2 };

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

    //color face_color;
    std::string color_name = "gray";
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


    void rotate(std::vector<std::vector<float>>& vertices, float angle, rotation_axis axis)
    {
        std::vector<std::vector<float>> rotation_matrix;
        rotation_matrix = get_rotation_matrix(angle, axis);

        for (int i = 0; i < 3; i++)
        {
            vertices[i].push_back(1.0f);
            multiply(rotation_matrix, vertices[i]);
            vertices[i].pop_back();
        }
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


}