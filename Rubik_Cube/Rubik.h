#include <vector>
#include <queue>
#include <map>
#include <fstream>

#include "shader.hpp"
#include "Cube.h"
#include "solver.h"

std::map<std::string, char> codes = {
	{ "red",    'R' },
	{ "yellow", 'Y' },
	{ "green",  'G' },
	{ "blue",   'B' },
	{ "white",	'W' },
	{ "orange",	'O' },
	{ "gray",	'A' },
};

class Rubik
{
private:
	double last_time_rubik = 0.0;
	double last_time_button = 0.0;
	int FPS = 150;
	int PPS = 4;
	float remaining_degreees = 0.0f;
	std::vector<Cube*> current_plane;
	rotation_type rotation_t;
	float rotation_angle;
	rotation_axis current_axis;
	std::queue<std::string> solution;
	bool is_solving = false;
	bool is_saved = false;
public:
	Cube matrix[3][3][3];

	Rubik();
	void prepare_VB0_VAO();
	void draw(Shader &shader, glm::mat4 proj);
	void move_plane(float angle, rotation_axis axis);
	void rotate_plane(std::vector<Cube*> pointers, bool is_clockwise);
	void rotate(rotation_type rt, bool is_clockwise);
	void save_data();
	void apply_solution();
	void solve();
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
	matrix[0][0][0].faces[2].color_name = "orange";
	matrix[0][0][0].faces[4].color_name = "green";
	// edge
	matrix[0][0][1].faces[1].color_name = "white";
	matrix[0][0][1].faces[4].color_name = "green";
	// corner
	matrix[0][0][2].faces[1].color_name = "white";
	matrix[0][0][2].faces[3].color_name = "red";
	matrix[0][0][2].faces[4].color_name = "green";
	// edge
	matrix[0][1][0].faces[2].color_name = "orange";
	matrix[0][1][0].faces[4].color_name = "green";
	// center
	matrix[0][1][1].faces[4].color_name = "green";
	// edge
	matrix[0][1][2].faces[3].color_name = "red";
	matrix[0][1][2].faces[4].color_name = "green";
	// corner
	matrix[0][2][0].faces[0].color_name = "yellow";
	matrix[0][2][0].faces[2].color_name = "orange";
	matrix[0][2][0].faces[4].color_name = "green";
	// edge
	matrix[0][2][1].faces[0].color_name = "yellow";
	matrix[0][2][1].faces[4].color_name = "green";
	// corner
	matrix[0][2][2].faces[0].color_name = "yellow";
	matrix[0][2][2].faces[3].color_name = "red";
	matrix[0][2][2].faces[4].color_name = "green";
	// edge
	matrix[1][0][0].faces[1].color_name = "white";
	matrix[1][0][0].faces[2].color_name = "orange";
	// center 
	matrix[1][0][1].faces[1].color_name = "white";
	// edge
	matrix[1][0][2].faces[1].color_name = "white";
	matrix[1][0][2].faces[3].color_name = "red";
	// center 
	matrix[1][1][0].faces[2].color_name = "orange";

	// center
	matrix[1][1][2].faces[3].color_name = "red";
	//edge
	matrix[1][2][0].faces[2].color_name = "orange";
	matrix[1][2][0].faces[0].color_name = "yellow";
	// center
	matrix[1][2][1].faces[0].color_name = "yellow";
	// edge
	matrix[1][2][2].faces[0].color_name = "yellow";
	matrix[1][2][2].faces[3].color_name = "red";
	// corner
	matrix[2][0][0].faces[1].color_name = "white";
	matrix[2][0][0].faces[2].color_name = "orange";
	matrix[2][0][0].faces[5].color_name = "blue";
	// edge
	matrix[2][0][1].faces[1].color_name = "white";
	matrix[2][0][1].faces[5].color_name = "blue";
	// corner
	matrix[2][0][2].faces[1].color_name = "white";
	matrix[2][0][2].faces[3].color_name = "red";
	matrix[2][0][2].faces[5].color_name = "blue";
	// edge
	matrix[2][1][0].faces[2].color_name = "orange";
	matrix[2][1][0].faces[5].color_name = "blue";
	//center
	matrix[2][1][1].faces[5].color_name = "blue";
	// edge
	matrix[2][1][2].faces[3].color_name = "red";
	matrix[2][1][2].faces[5].color_name = "blue";
	// corner
	matrix[2][2][0].faces[0].color_name = "yellow";
	matrix[2][2][0].faces[2].color_name = "orange";
	matrix[2][2][0].faces[5].color_name = "blue";
	// edge
	matrix[2][2][1].faces[0].color_name = "yellow";
	matrix[2][2][1].faces[5].color_name = "blue";
	// corner
	matrix[2][2][2].faces[0].color_name = "yellow";
	matrix[2][2][2].faces[3].color_name = "red";
	matrix[2][2][2].faces[5].color_name = "blue";
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
					op::prepare_VBO_VAO(cube.VBO[v * 2], cube.VAO[v * 2], &cube.data[v * 2][0], 9 * sizeof(float));
					op::prepare_VBO_VAO(cube.VBO[v * 2 + 1], cube.VAO[v * 2 + 1], &cube.data[v * 2 + 1][0], 9 * sizeof(float));
				}
			}
		}
	}
}

void Rubik::apply_solution()
{
	double current_time = glfwGetTime();
	if ((current_time - last_time_rubik) > 2.5 / PPS)
	{
		std::cout << "Movimientos restantes: " << solution.size() << "\n";
		std::string word = solution.front();
		std::cout <<  "Movimiento: "<< word << "\n";
		solution.pop();
		if (word == "R")
			rotate(rotation_type::RIGHT, false);
		else if (word == "L")
			rotate(rotation_type::LEFT, true);
		else if (word == "U")
			rotate(rotation_type::TOP, false);
		else if (word == "D")
			rotate(rotation_type::BOTTOM, true);
		else if (word == "F")
			rotate(rotation_type::FRONT, false);
		else if (word == "B")
			rotate(rotation_type::BACK, true);
		else if (word == "R'")
			rotate(rotation_type::RIGHT, true);
		else if (word == "L'")
			rotate(rotation_type::LEFT, false);
		else if (word == "U'")
			rotate(rotation_type::TOP, true);
		else if (word == "D'")
			rotate(rotation_type::BOTTOM, false);
		else if (word == "F'")
			rotate(rotation_type::FRONT, true);
		else if (word == "B'")
			rotate(rotation_type::BACK, false);
	}
}


void Rubik::move_plane(float angle, rotation_axis axis)
{
	double current_time = glfwGetTime();
	if ((current_time - last_time_rubik) > 1.0 / FPS)
	{
		// std::cout << remaining_degreees << ", ";

		for (int i = 0; i < current_plane.size(); i++)
		{
			Cube* cube = current_plane[i];
			cube->rotate(angle, axis);
			cube->update_faces();

			// faces loop
			for (int v = 0; v < 6; v++)
			{
				op::prepare_VBO_VAO(cube->VBO[v * 2], cube->VAO[v * 2], &cube->data[v * 2][0], 9 * sizeof(float));
				op::prepare_VBO_VAO(cube->VBO[v * 2 + 1], cube->VAO[v * 2 + 1], &cube->data[v * 2 + 1][0], 9 * sizeof(float));
			}

		}
		remaining_degreees -= rotation_angle;

		last_time_rubik = current_time;
	}
}


void Rubik::draw(Shader &shader, glm::mat4 proj)
{
	if (solution.size() > 0)
		apply_solution();
		
	else
		is_solving = false;
	if (remaining_degreees > 0.02f || remaining_degreees < -0.02f)
		move_plane(rotation_angle, current_axis);
	else
		remaining_degreees = 0.0f;

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
					op::draw(shader, cube.VAO[v * 2], proj, colors[cube.faces[v].color_name]);
					op::draw(shader, cube.VAO[v * 2 + 1], proj, colors[cube.faces[v].color_name]);
				}
			}
		}
	}
}


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
	if ((current_time - last_time_button) > 2.0 / PPS)
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
					pointers.push_back(&matrix[i][j][0]);
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
					pointers.push_back(&matrix[i][j][2]);
			current_axis = rotation_axis::Z;
			break;
		}

		rotate_plane(pointers, is_clockwise);
		current_plane = pointers;
		last_time_button = current_time;

		// std::cout << to_string();
	}
}


void Rubik::save_data()
{
	if (is_saved)
		return;
	// create file result.txt
	is_saved = true;
	std::ofstream file;
	file.open("data.txt", std::ios::trunc);

	// WHITE
	for (int k = 2; k >= 0; k--)
	{
		for (int i = 2; i >= 0; i--)
		{
			// faces loop
			for (int f = 0; f < 6; f++)
			{
				if (matrix[i][0][k].faces[f].color_name != "gray" && op::get_axis(matrix[i][0][k].faces[f]) == rotation_axis::Y)
					file << codes[matrix[i][0][k].faces[f].color_name];
			}
		}
		file << "\n";
	}

	// ORANGE
	for (int j = 0; j < 3; j++)
	{
		for (int i = 2; i >= 0; i--)
		{
			// faces loop
			for (int f = 0; f < 6; f++)
			{
				if (matrix[i][j][0].faces[f].color_name != "gray" && op::get_axis(matrix[i][j][0].faces[f]) == rotation_axis::Z)
					file << codes[matrix[i][j][0].faces[f].color_name];
			}
		}
		file << "\n";
	}
	
	// GREEN
	for (int j = 0; j < 3; j++)
	{
		for (int k = 0; k < 3; k++)
		{
			// faces loop
			for (int f = 0; f < 6; f++)
			{
				if (matrix[0][j][k].faces[f].color_name != "gray" && op::get_axis(matrix[0][j][k].faces[f]) == rotation_axis::X)
					file << codes[matrix[0][j][k].faces[f].color_name];
			}
		}
		file << "\n";
	}

	// RED
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			// faces loop
			for (int f = 0; f < 6; f++)
			{
				if (matrix[i][j][2].faces[f].color_name != "gray" && op::get_axis(matrix[i][j][2].faces[f]) == rotation_axis::Z)
					file << codes[matrix[i][j][2].faces[f].color_name];
			}
		}
		file << "\n";
	}

	// BLUE
	for (int j = 0; j < 3; j++)
	{
		for (int k = 2; k >= 0; k--)
		{
			// faces loop
			for (int f = 0; f < 6; f++)
			{
				if (matrix[2][j][k].faces[f].color_name != "gray" && op::get_axis(matrix[2][j][k].faces[f]) == rotation_axis::X)
					file << codes[matrix[2][j][k].faces[f].color_name];
			}
		}
			
		file << "\n";
	}
	
	// YELLOW
	for (int k = 0; k < 3; k++)
	{
		for (int i = 2; i >= 0; i--)
		{
			// faces loop
			for (int f = 0; f < 6; f++)
			{
				if (matrix[i][2][k].faces[f].color_name != "gray" && op::get_axis(matrix[i][2][k].faces[f]) == rotation_axis::Y)
					file << codes[matrix[i][2][k].faces[f].color_name];
			}
		}
		file << "\n";
	}

	file.close();

	solver::solve();
}


void Rubik::solve()
{
	if (is_solving)
		return;
	is_solving = true;

    // read from result.txt
    std::ifstream file("result.txt");
    // read word by word separated by spaces
    std::string word;
    while (file >> word)
    {
		//std::cout << "--" << word << "--";
		
		if (word == "R2")
		{
			solution.push("R");
			solution.push("R");
		}
		else if (word == "L2")
		{
			solution.push("L");
			solution.push("L");
		}
		else if (word == "U2")
		{
			solution.push("U");
			solution.push("U");
		}
		else if (word == "D2")
		{
			solution.push("D");
			solution.push("D");
		}
		else if (word == "F2")
		{
			solution.push("F");
			solution.push("F");
		}
		else if (word == "B2")
		{
			solution.push("B");
			solution.push("B");
		}
		else
			solution.push(word);
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
