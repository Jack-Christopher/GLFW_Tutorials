#include <vector>

#include "Operations.h"

class Cube
{
private:
	std::vector<std::vector<double>> coords;
public:
	Cube();
	void set_coords(std::vector<double> left_bottom_back, std::vector<double> right_top_front);
	void rotate(double angle, rotation_axis axis);
	std::string to_string();
	~Cube();
	
};


Cube::Cube() 
{
	coords.resize(8);
	for (int i = 0; i < 8; i++)
		coords[i].resize(3);
}

void Cube::set_coords(std::vector<double> left_bottom_back, std::vector<double> right_top_front)
{
	coords[0] = { left_bottom_back[0], left_bottom_back[1], right_top_front[2] };
	coords[1] = { right_top_front[0], left_bottom_back[1], right_top_front[2] };
	coords[2] = { right_top_front[0], left_bottom_back[1], left_bottom_back[2] };
	coords[3] = left_bottom_back;
	
	coords[4] = { left_bottom_back[0], right_top_front[1], right_top_front[2] };
	coords[5] = right_top_front;
	coords[6] = { right_top_front[0], right_top_front[1], left_bottom_back[2] };
	coords[7] = { left_bottom_back[0], right_top_front[1], left_bottom_back[2] };	
}

void Cube::rotate(double angle, rotation_axis axis)
{
	op::rotate(coords, angle, axis);
}
				
				
			
std::string Cube::to_string()
{
	std::string str = "";
	for (int i = 0; i < 8; i++)
	{
		str += "(";
		for (int j = 0; j < 3; j++)
		{
			str += std::to_string(coords[i][j]);
			if (j != 2)
				str += ", ";
		}
		str += ")";
		if (i != 7)
			str += "; ";
	}
	return str;
}

Cube::~Cube()
{
	
}