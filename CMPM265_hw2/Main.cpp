#include <SFML/Graphics.hpp>
#include "TileMap.h"
#include <noise/noise.h>
#include <iostream>

using namespace noise;
using namespace std;

module::Perlin perlin;
const int row = 48;
const int col = 48;
int row_offset = 1;
int col_offset = 1;
int level[row * col];
double value;
double multiplier = 0.1f;

void generateLevel(float z)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			value = perlin.GetValue(multiplier * i + row_offset, multiplier * j + col_offset, z);
			if (value < -0.6f)
			{
				level[i*col + j] = 0;
			}
			else if (-0.6f <= value && value < -0.2f)
			{
				level[i*col + j] = 1;
			}
			else if (-0.2f <= value && value < 0.2f)
			{
				level[i*col + j] = 2;
			}
			else if (0.2f <= value && value < 0.6f)
			{
				level[i*col + j] = 3;
			}
			else
			{
				level[i*col + j] = 4;
			}
		}
	}
}

int main()
{
	// create the window
	sf::RenderWindow window(sf::VideoMode(row * 16, col * 16), "Tilemap");

	// define the level with an array of tile indices
	int seed = time(NULL);
	float z = 0.1f;
	perlin.SetSeed(seed);
	generateLevel(z);

	// create the tilemap from the level definition
	TileMap map;
	if (!map.load("tileset.png", sf::Vector2u(16, 16), level, row, col))
		return -1;

	// run the main loop
	while (window.isOpen())
	{
		// handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				perlin.SetSeed(++seed);
				generateLevel(z);
				if (!map.load("tileset.png", sf::Vector2u(16, 16), level, row, col))
					return -1;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				perlin.SetSeed(--seed);
				generateLevel(z);
				if (!map.load("tileset.png", sf::Vector2u(16, 16), level, row, col))
					return -1;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				z += 0.1f;
				generateLevel(z);
				if (!map.load("tileset.png", sf::Vector2u(16, 16), level, row, col))
					return -1;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				z -= 0.1f;
				generateLevel(z);
				if (!map.load("tileset.png", sf::Vector2u(16, 16), level, row, col))
					return -1;
			}
		}

		// draw the map
		window.clear();
		window.draw(map);
		window.display();
	}

	return 0;
}