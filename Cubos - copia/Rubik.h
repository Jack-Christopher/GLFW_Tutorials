#include <vector>

#include "Cube.h"

class Rubik
{
private:
	double last_time_rubik = 0.0;
	double last_time_button = 0.0;
	int FPS = 60;
	int PPS = 2;
	float remaining_degreees = 0.0f;
	float rotation_angle;
public:
	Cube matrix[3];
	
	Rubik();
	void prepare_VB0_VAO();
	void draw(std::map < std::string, source> Sources);
	void move_plane(float angle, rotation_axis axis);
	void rotate_plane(std::vector<Cube*> pointers, bool is_clockwise);
	void rotate(bool is_clockwise);
	void free();
	std::string to_string();
	~Rubik();
};


Rubik::Rubik()
{
	float size = 0.5f;
	float x, y, z;
	
	x = -size / 2;
	y = -size*1.5;
	for (int j = 2; j >= 0; j--)
	{
		z = -size / 2;
		matrix[j].set_vertices({ x, y+0.01f, z}, {x + size, y + size - 0.01f, z + size});
		y += size;
	}
	
	for (int i = 0; i < 6; i++)
	{
		matrix[0].faces[i].color_name = "red";
		matrix[1].faces[i].color_name = "white";
		matrix[2].faces[i].color_name = "red";
	}
}


void Rubik::prepare_VB0_VAO()
{
	for (int i = 0; i < 3; i++)
	{
		Cube& cube = matrix[i];
		// faces loop
		for (int v = 0; v < 6; v++)
		{
			op::prepare_VB0_VAO(cube.VBO[v * 2], cube.VAO[v * 2], &cube.data[v * 2][0], 9 * sizeof(float), drawType::Triangle);
			op::prepare_VB0_VAO(cube.VBO[v * 2 + 1], cube.VAO[v * 2 + 1], &cube.data[v * 2 + 1][0], 9 * sizeof(float), drawType::Triangle);
		}
	}
}


void Rubik::move_plane(float angle, rotation_axis axis)
{
	double current_time = glfwGetTime();
	if ((current_time - last_time_rubik) > 1.0 / FPS)
	{
		for (int i = 0; i < 3; i++)
		{
			Cube &cube = matrix[i];
			cube.rotate(angle, axis);
			cube.set_faces();

			// faces loop
			for (int v = 0; v < 6; v++)
			{
				op::prepare_VB0_VAO(cube.VBO[v * 2], cube.VAO[v * 2], &cube.data[v * 2][0], 9 * sizeof(float), drawType::Triangle);
				op::prepare_VB0_VAO(cube.VBO[v * 2 + 1], cube.VAO[v * 2 + 1], &cube.data[v * 2 + 1][0], 9 * sizeof(float), drawType::Triangle);
			}

		}
		remaining_degreees -= rotation_angle;
		last_time_rubik = current_time;
	}
}


void Rubik::draw(std::map <std::string, source> Sources)
{
	if (remaining_degreees > 0.02f || remaining_degreees < -0.02f)
		move_plane(rotation_angle, rotation_axis::X);
	else
		remaining_degreees = 0.0f;

	for (int i = 0; i < 3; i++)
	{
		Cube& cube = matrix[i];
		// faces loop
		for (int v = 0; v < 6; v++)
		{
			op::draw(Sources[cube.faces[v].color_name].shaderProgram, cube.VAO[v * 2], drawType::Triangle);
			op::draw(Sources[cube.faces[v].color_name].shaderProgram, cube.VAO[v * 2 + 1], drawType::Triangle);
		}
	}
}




void Rubik::rotate(bool is_clockwise)
{
	double current_time = glfwGetTime();
	if ((current_time - last_time_button) > 1.0 / PPS)
	{
		remaining_degreees = (is_clockwise) ? (90.0f) : (-90.0f);
		rotation_angle = (is_clockwise) ? (5.0f) : (-5.0f);
		last_time_button = current_time;
	}
}


void Rubik::free()
{
	for (int i = 0; i < 3; i++)
	{
		// for each face
		for (int v = 0; v < 6; v++)
		{
			Cube& cube = matrix[i];
			glDeleteVertexArrays(1, &cube.VAO[v]);
			glDeleteBuffers(1, &cube.VBO[v]);
		}
	}
}

std::string Rubik::to_string()
{
	std::string s = "\n";
	for (int i = 0; i < 3; i++)
	{
		s += matrix[i].id;
		s += ": \n";
		s += matrix[i].to_string();
				
		s += "\n";				
	}
	s += "\n\n";
	
	return s;
}

Rubik::~Rubik()
{
	
}