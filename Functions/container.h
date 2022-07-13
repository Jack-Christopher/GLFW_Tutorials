#include "triangle.h"


class Container
{
public:
	std::vector<Triangle> triangles;

    Container(std::vector<Triangle> triangles);
    void prepare_VBO_VAO();

    void rotate(float angle, rotation_axis axis);
    void translate(float x, float y, float z);
    void scale(float x, float y, float z);
	
    void setColor(std::string color);
    void draw(Shader &shader, glm::mat4 proj);
    void free();
};

Container::Container(std::vector<Triangle> triangles)
{
    this->triangles = triangles;
    prepare_VBO_VAO();
}

void Container::prepare_VBO_VAO()
{
    for (int i = 0; i < triangles.size(); i++)
    {
        triangles[i].prepare_VBO_VAO();
    }
}

void Container::rotate(float angle, rotation_axis axis)
{
    for (int i = 0; i < triangles.size(); i++)
    {
        triangles[i].rotate(angle, axis);
    }
}

void Container::translate(float x, float y, float z)
{
    for (int i = 0; i < triangles.size(); i++)
    {
        triangles[i].translate(x, y, z);
    }
}

void Container::scale(float x, float y, float z)
{
    for (int i = 0; i < triangles.size(); i++)
    {
        triangles[i].scale(x, y, z);
    }
}

void Container::setColor(std::string color)
{
    for (Triangle& triangle : triangles)
    {
        triangle.color_name = color;
    }
}

void Container::draw(Shader &shader, glm::mat4 proj)
{
    for (Triangle& triangle : triangles)
    {
        triangle.draw(shader, proj);
    }
}

void Container::free()
{
    for (Triangle& triangle : triangles)
    {
        triangle.free();
    }
}