#include <vector>

#include "Cube.h"

class Rubik
{
private:
	int iteration_times = 20;
public:
	Cube matrix[3][3][3];
	
	Rubik();
	void prepare_VB0_VAO();
	void draw(std::map<std::string, unsigned int> shaderPrograms);
	void move(int& key_press_counter, float angle, rotation_axis axis);
	void free();
	std::string to_string();
	~Rubik();
};


Rubik::Rubik()
{
	float x, y, z;
	x = -0.6f;
	for (int i = 2; i >= 0; i--)
	{
		y = -0.6f;
		for (int j = 2; j >= 0; j--)
		{
			z = -0.6f;
			for (int k = 2; k >= 0; k--)
			{
				matrix[i][j][k].set_vertices( { x, y, z }, { x + 0.4f, y + 0.4f, z + 0.4f } );
				z += 0.4f;
			}
			y += 0.4f;
		}
		x += 0.4f;
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


void Rubik::draw(std::map<std::string, unsigned int> shaderPrograms)
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
					op::draw(shaderPrograms[cube.faces[v].color_name], cube.VAO[v * 2], drawType::Triangle);
					op::draw(shaderPrograms[cube.faces[v].color_name], cube.VAO[v * 2 + 1], drawType::Triangle);
				}
			}
		}
	}
}

void Rubik::move(int& key_press_counter, float angle, rotation_axis axis)
{
	key_press_counter++;
	if (key_press_counter > iteration_times)
	{
		key_press_counter = 0;
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
	std::string s = "";
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				s += matrix[i][j][k].to_string();
				s += "\n";				
			}
		}
	}
	
	return s;
}

Rubik::~Rubik()
{
	
}