#include <vector>
#include <map>

#include "Operations.h"


class Cube
{
public:
	std::vector<unsigned int> VAO;
	std::vector<unsigned int> VBO;
	std::vector<vertex> vertices;
	std::vector<face> faces;
	std::vector<std::vector<float>> data;
	std::string id;

	Cube();
	void update_faces();
	void set_vertices(vertex left_bottom_back, vertex right_top_front);
	void rotate(float angle, rotation_axis axis);
	std::string to_string();
	~Cube();
};


Cube::Cube()
{
	vertices.resize(8);
	faces.resize(6);
	data.resize(12);
	for (int i = 0; i < 12; i++)
		data[i].resize(9);

	VAO.resize(12);
	VBO.resize(12);
}



void Cube::update_faces()
{
	// x-z plane
	faces[0] = { vertices[0], vertices[1], vertices[2], vertices[0], vertices[2], vertices[3], faces[0].color_name };
	faces[1] = { vertices[4], vertices[5], vertices[6], vertices[4], vertices[6], vertices[7], faces[1].color_name };
	// y-x plane
	faces[2] = { vertices[0], vertices[1], vertices[5], vertices[0], vertices[5], vertices[4], faces[2].color_name };
	faces[3] = { vertices[2], vertices[3], vertices[7], vertices[2], vertices[7], vertices[6], faces[3].color_name };
	// y-z plane
	faces[4] = { vertices[1], vertices[2], vertices[6], vertices[1], vertices[6], vertices[5], faces[4].color_name };
	faces[5] = { vertices[3], vertices[0], vertices[4], vertices[3], vertices[4], vertices[7], faces[5].color_name };

	for (int i = 0; i < 6; i++)
	{
		data[i * 2] = { 
			faces[i].a1.x, faces[i].a1.y, faces[i].a1.z, 
			faces[i].a2.x, faces[i].a2.y, faces[i].a2.z, 
			faces[i].a3.x, faces[i].a3.y, faces[i].a3.z 
		};
		data[i * 2 + 1] = { 
			faces[i].b1.x, faces[i].b1.y, faces[i].b1.z, 
			faces[i].b2.x, faces[i].b2.y, faces[i].b2.z, 
			faces[i].b3.x, faces[i].b3.y, faces[i].b3.z 
		};
	}
}

void Cube::set_vertices(vertex left_bottom_back, vertex right_top_front)
{
	vertices[0] = { left_bottom_back.x, left_bottom_back.y, right_top_front.z };
	vertices[1] = { right_top_front.x, left_bottom_back.y, right_top_front.z };
	vertices[2] = { right_top_front.x, left_bottom_back.y, left_bottom_back.z };
	vertices[3] = left_bottom_back;
	vertices[4] = { left_bottom_back.x, right_top_front.y, right_top_front.z };
	vertices[5] = right_top_front;
	vertices[6] = { right_top_front.x, right_top_front.y, left_bottom_back.z };
	vertices[7] = { left_bottom_back.x, right_top_front.y, left_bottom_back.z };

	update_faces();
}

void Cube::rotate(float angle, rotation_axis axis)
{
	op::rotate(vertices, angle, axis);
}



std::string Cube::to_string()
{
	std::string str = "";
	for (int i = 0; i < 8; i++)
		str += "(" + std::to_string(vertices[i].x) + ", " + std::to_string(vertices[i].y) + ", " + std::to_string(vertices[i].z) + ")\n";
	return str;
}

Cube::~Cube()
{

}
