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
	std::vector<Cube*> current_plane;
	rotation_type rotation_t;
	float rotation_angle;
	rotation_axis current_axis;
public:
	Cube matrix[3][3][3];
	
	Rubik();
	void prepare_VB0_VAO();
	// void normalize_vector();
	// void calculate_current_axis();
	void draw(std::map < std::string, source> Sources);
	// void move(float angle, rotation_axis axis);
	void move_plane(float angle, rotation_axis axis);
	void rotate_plane(std::vector<Cube*> pointers, bool is_clockwise);
	void rotate(rotation_type rt, bool is_clockwise);
	void free();
	std::string to_string();
	~Rubik();
};


Rubik::Rubik()
{
	float size = 1.0f;
	float space = 0.02f;
	float x, y, z;
	x = -size / 2;
	for (int i = 2; i >= 0; i--)
	{
		y = -size / 2;
		for (int j = 2; j >= 0; j--)
		{
			z = -size / 2;
			for (int k = 2; k >= 0; k--)
			{
				matrix[i][j][k].set_vertices({ x + space, y + space, z + space },
					{ x + size / 3 - space, y + size / 3 - space, z + size / 3 - space });
				z += size / 3;
				matrix[i][j][k].id = std::to_string(i) + std::to_string(j) + std::to_string(k);
			}
			y += size / 3;
		}
		x += size / 3;
	}

	// corner
	matrix[0][0][0].faces[1].color_name = "white";
	matrix[0][0][0].faces[2].color_name = "green";
	matrix[0][0][0].faces[4].color_name = "red";
	// edge
	matrix[0][0][1].faces[1].color_name = "white";
	matrix[0][0][1].faces[4].color_name = "red";
	// corner
	matrix[0][0][2].faces[1].color_name = "white";
	matrix[0][0][2].faces[3].color_name = "blue";
	matrix[0][0][2].faces[4].color_name = "red";
	// edge
	matrix[0][1][0].faces[2].color_name = "green";
	matrix[0][1][0].faces[4].color_name = "red";
	// center
	matrix[0][1][1].faces[4].color_name = "red";
	// edge
	matrix[0][1][2].faces[3].color_name = "blue";
	matrix[0][1][2].faces[4].color_name = "red";
	// corner
	matrix[0][2][0].faces[0].color_name = "yellow";
	matrix[0][2][0].faces[2].color_name = "green";
	matrix[0][2][0].faces[4].color_name = "red";
	// edge
	matrix[0][2][1].faces[0].color_name = "yellow";
	matrix[0][2][1].faces[4].color_name = "red";
	// corner
	matrix[0][2][2].faces[0].color_name = "yellow";
	matrix[0][2][2].faces[3].color_name = "blue";
	matrix[0][2][2].faces[4].color_name = "red";
	// edge
	matrix[1][0][0].faces[1].color_name = "white";
	matrix[1][0][0].faces[2].color_name = "green";
	// center 
	matrix[1][0][1].faces[1].color_name = "white";
	// edge
	matrix[1][0][2].faces[1].color_name = "white";
	matrix[1][0][2].faces[3].color_name = "blue";
	// center 
	matrix[1][1][0].faces[2].color_name = "green";
		
	// center
	matrix[1][1][2].faces[3].color_name = "blue";
	//edge
	matrix[1][2][0].faces[2].color_name = "green";
	matrix[1][2][0].faces[0].color_name = "yellow";
	// center
	matrix[1][2][1].faces[0].color_name = "yellow";
	// edge
	matrix[1][2][2].faces[0].color_name = "yellow";
	matrix[1][2][2].faces[3].color_name = "blue";
	// corner
	matrix[2][0][0].faces[1].color_name = "white";
	matrix[2][0][0].faces[2].color_name = "green";
	matrix[2][0][0].faces[5].color_name = "orange";
	// edge
	matrix[2][0][1].faces[1].color_name = "white";
	matrix[2][0][1].faces[5].color_name = "orange";
	// corner
	matrix[2][0][2].faces[1].color_name = "white";
	matrix[2][0][2].faces[3].color_name = "blue";
	matrix[2][0][2].faces[5].color_name = "orange";
	// edge
	matrix[2][1][0].faces[2].color_name = "green";
	matrix[2][1][0].faces[5].color_name = "orange";
	//center
	matrix[2][1][1].faces[5].color_name = "orange";
	// edge
	matrix[2][1][2].faces[3].color_name = "blue";
	matrix[2][1][2].faces[5].color_name = "orange";
	// corner
	matrix[2][2][0].faces[0].color_name = "yellow";
	matrix[2][2][0].faces[2].color_name = "green";
	matrix[2][2][0].faces[5].color_name = "orange";
	// edge
	matrix[2][2][1].faces[0].color_name = "yellow";
	matrix[2][2][1].faces[5].color_name = "orange";
	// corner
	matrix[2][2][2].faces[0].color_name = "yellow";
	matrix[2][2][2].faces[3].color_name = "blue";
	matrix[2][2][2].faces[5].color_name = "orange";
}


void Rubik::prepare_VB0_VAO()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				Cube& cube = matrix[i][j][k];
				// faces loop
				for (int v = 0; v < 6; v++)
				{
					op::prepare_VB0_VAO(cube.VBO[v * 2], cube.VAO[v * 2], &cube.data[v * 2][0], 9 * sizeof(float), drawType::Triangle);
					op::prepare_VB0_VAO(cube.VBO[v * 2 + 1], cube.VAO[v * 2 + 1], &cube.data[v * 2 + 1][0], 9 * sizeof(float), drawType::Triangle);
				}
			}
		}
	}
}

void Rubik::move_plane(float angle, rotation_axis axis)
{
	double current_time = glfwGetTime();
	if ((current_time - last_time_rubik) > 1.0 / FPS)
	{
		std::cout << remaining_degreees << ", ";

		for (int i = 0; i < current_plane.size(); i++)
		{
			Cube *cube = current_plane[i];
			cube->rotate(angle, axis);
			cube->set_faces();

			// faces loop
			for (int v = 0; v < 6; v++)
			{
				op::prepare_VB0_VAO(cube->VBO[v * 2], cube->VAO[v * 2], &cube->data[v * 2][0], 9 * sizeof(float), drawType::Triangle);
				op::prepare_VB0_VAO(cube->VBO[v * 2 + 1], cube->VAO[v * 2 + 1], &cube->data[v * 2 + 1][0], 9 * sizeof(float), drawType::Triangle);
			}

		}
		remaining_degreees -= rotation_angle;

		last_time_rubik = current_time;
	}
}


void Rubik::draw(std::map <std::string, source> Sources)
{
	//std::cout << "CA: "<< current_axis.x << " " << current_axis.y << " " << current_axis.z << '\n';
	if (remaining_degreees > 0.02f || remaining_degreees < -0.02f)
	{
		// calculate_current_axis();
		move_plane(rotation_angle, current_axis);
		// move_plane(rotation_angle, {0.0f, 1.0f, 0.0f});
		// remaining_degreees -= rotation_angle;
	}
	else
	{
		remaining_degreees = 0.0f;
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				Cube& cube = matrix[i][j][k];
				// faces loop
				for (int v = 0; v < 6; v++)
				{
					op::draw(Sources[cube.faces[v].color_name].shaderProgram, cube.VAO[v * 2], drawType::Triangle);
					op::draw(Sources[cube.faces[v].color_name].shaderProgram, cube.VAO[v * 2 + 1], drawType::Triangle);
				}
			}
		}
	}
}

/*
void Rubik::move(float angle, rotation_axis axis)
{
	double current_time = glfwGetTime();
	if ((current_time - last_time_rubik) > 1.0/FPS)
	{
		// Cube loops
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				for (int k = 0; k < 3; k++)
				{
					Cube& cube = matrix[i][j][k];
					cube.rotate(angle, axis);
					cube.set_faces();

					// faces loop
					for (int v = 0; v < 6; v++)
					{
						op::prepare_VB0_VAO(cube.VBO[v * 2], cube.VAO[v * 2], &cube.data[v * 2][0], 9 * sizeof(float), drawType::Triangle);
						op::prepare_VB0_VAO(cube.VBO[v * 2 + 1], cube.VAO[v * 2 + 1], &cube.data[v * 2 + 1][0], 9 * sizeof(float), drawType::Triangle);
					}
				}
			}
		}
		last_time_rubik = current_time;
	}
}
*/


void Rubik::rotate_plane(std::vector<Cube*> pointers, bool is_clockwise)
{
	std::vector<Cube> copy;
	for (int i = 0; i < pointers.size(); i++)
		copy.push_back(*pointers[i]);

    bool first_method;
    if (is_clockwise)
    {
        if (current_axis == rotation_axis::Y)
            first_method = false;
        else
            first_method = true;
    }
    else
    {
        if (current_axis == rotation_axis::Y)
            first_method = true;
        else
            first_method = false;
    }

    

	if (first_method)
	{
		*pointers[0] = copy[6];		*pointers[1] = copy[3];		*pointers[2] = copy[0];
		*pointers[3] = copy[7];		*pointers[4] = copy[4];		*pointers[5] = copy[1];
		*pointers[6] = copy[8];		*pointers[7] = copy[5];		*pointers[8] = copy[2];
	}
	else
	{
		*pointers[0] = copy[2];		*pointers[1] = copy[5];		*pointers[2] = copy[8];
		*pointers[3] = copy[1];		*pointers[4] = copy[4];		*pointers[5] = copy[7];
		*pointers[6] = copy[0];		*pointers[7] = copy[3];		*pointers[8] = copy[6];
	}
}

void Rubik::rotate(rotation_type rt, bool is_clockwise)
{
	double current_time = glfwGetTime();
	if ((current_time - last_time_button) > 1.0 / PPS)
	{
		remaining_degreees = (is_clockwise) ? (90.0f) : (-90.0f);
		rotation_angle = (is_clockwise) ? (5.0f) : (-5.0f);
		rotation_t = rt;

		std::vector<Cube*> pointers;

		switch (rt)
		{
		case rotation_type::TOP:
			for (int k = 2; k >= 0; k--)
				for (int i = 2; i >= 0; i--)
					pointers.push_back(&matrix[i][0][k]);
			current_axis = rotation_axis::Y;
			break;
        case rotation_type::CENTER_Y:
			for (int k = 2; k >= 0; k--)
				for (int i = 2; i >= 0; i--)
					pointers.push_back(&matrix[i][1][k]);
			current_axis = rotation_axis::Y;
			break;
		case rotation_type::BOTTOM:
			for (int k = 2; k >= 0; k--)
				for (int i = 2; i >= 0; i--)
					pointers.push_back(&matrix[i][2][k]);
			current_axis = rotation_axis::Y;
			break;
		case rotation_type::RIGHT:
			for (int k = 2; k >= 0; k--)
				for (int j = 2; j >= 0; j--)
					pointers.push_back(&matrix[0][j][k]);
			current_axis = rotation_axis::X;
			break;
        case rotation_type::CENTER_X:
			for (int k = 2; k >= 0; k--)
				for (int j = 2; j >= 0; j--)
					pointers.push_back(&matrix[1][j][k]);
			current_axis = rotation_axis::X;
			break;
		case rotation_type::LEFT:
			for (int k = 2; k >= 0; k--)
				for (int j = 2; j >= 0; j--)
					pointers.push_back(&matrix[2][j][k]);
			current_axis = rotation_axis::X;
			break;
		case rotation_type::FRONT:
			for (int j = 2; j >= 0; j--)
				for (int i = 2; i >= 0; i--)
					pointers.push_back(&matrix[i][j][2]);
			current_axis = rotation_axis::Z;
			break;
        case rotation_type::CENTER_Z:
			for (int j = 2; j >= 0; j--)
				for (int i = 2; i >= 0; i--)
					pointers.push_back(&matrix[i][j][1]);
			current_axis = rotation_axis::Z;
			break;
		case rotation_type::BACK:
			for (int j = 2; j >= 0; j--)
				for (int i = 2; i >= 0; i--)
					pointers.push_back(&matrix[i][j][0]);
			current_axis = rotation_axis::Z;
			break;
		}

		rotate_plane(pointers, is_clockwise);
		current_plane = pointers;
		last_time_button = current_time;

		std::cout << to_string();
	}
}


void Rubik::free()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				// for each face
				for (int v = 0; v < 6; v++)
				{
					Cube& cube = matrix[i][j][k];
					glDeleteVertexArrays(1, &cube.VAO[v]);
					glDeleteBuffers(1, &cube.VBO[v]);
				}
			}
		}
	}
}

std::string Rubik::to_string()
{
	std::string s = "\n";
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				s += matrix[i][j][k].id;
				s += ": \n";
				s += matrix[i][j][k].to_string();
				
				//s += " ";
				s += "\n";				
			}
			//s += "\n";
		}
		//s += "\n\n";
	}
	s += "\n\n";
	
	return s;
}

Rubik::~Rubik()
{
	
}