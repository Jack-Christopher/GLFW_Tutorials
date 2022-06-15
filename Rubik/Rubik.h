#include <vector>

#include "Cube.h"

class Rubik
{
private:
	// x - row
	// y - column
	// z - layer
	
	Cube matrix[3][3][3];
	
public:
	Rubik();
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
				matrix[i][j][k].set_coords( { x, y, z }, { x + 0.4f, y + 0.4f, z + 0.4f } );
				z += 0.4f;
			}
			y += 0.4f;
		}
		x += 0.4f;
	}

	//print matrix
	for (int i = 2; i >= 0; i--)
	{
		for (int j = 2; j >= 0; j--)
		{
			for (int k = 2; k >= 0; k--)
			{
				std::cout<< matrix[i][j][k].to_string()<<"\n";
			}
		}
	}
	
}

Rubik::~Rubik()
{
	
}