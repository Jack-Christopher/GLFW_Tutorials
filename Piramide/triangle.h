#include <math.h>
#include <vector>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "operations.h"


class Triangle
{
public:
	unsigned int VAO;
	unsigned int VBO;
	std::vector<vertex> vertices;
    std::vector<float> coordinates;
    std::string color_name = "gray";

    Triangle(vertex a, vertex b, vertex c, std::string color);
    void rotate(float angle, rotation_axis axis);
    void translate(float x, float y, float z);
    void scale(float x, float y, float z);
    void update_coordinates();
    void set_vertices(vertex a, vertex b, vertex c);
    void prepare_VBO_VAO();
    void draw(Shader &shader, glm::mat4 proj);
    void free();
};


Triangle::Triangle(vertex a, vertex b, vertex c, std::string color)
{
    vertices.resize(3);
    coordinates.resize(9);
    set_vertices(a, b, c);
    prepare_VBO_VAO();
    color_name = color;    
}

void Triangle::update_coordinates()
{
    coordinates = { 
        vertices[0].x, vertices[0].y, vertices[0].z, 
        vertices[1].x, vertices[1].y, vertices[1].z, 
        vertices[2].x, vertices[2].y, vertices[2].z 
    };
}

void Triangle::set_vertices(vertex a, vertex b, vertex c)
{
    vertices[0] = a;
    vertices[1] = b;
    vertices[2] = c;
    update_coordinates();
}

void Triangle::rotate(float angle, rotation_axis axis)
{
    std::vector<vertex> old_vertices = vertices;
	op::rotate(vertices, angle, axis);
    update_coordinates();
    prepare_VBO_VAO();
}

void Triangle::translate(float x, float y, float z)
{
    op::translate(vertices, x, y, z);
    update_coordinates();
    prepare_VBO_VAO();
}

void Triangle::scale(float x, float y, float z)
{
    op::scale(vertices, x, y, z);
    update_coordinates();
    prepare_VBO_VAO();
}

void Triangle::prepare_VBO_VAO()
{
    opengl::prepare_VBO_VAO(VBO, VAO, &coordinates[0], 9 * sizeof(float));
}

void Triangle::draw(Shader &shader, glm::mat4 proj)
{
    opengl::draw(shader, VAO, proj, colors[color_name]);
}

void Triangle::free()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}
