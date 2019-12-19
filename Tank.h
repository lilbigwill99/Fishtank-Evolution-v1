#pragma once

/*
Tank.h
Will Massey
The Enviorment For Simulation
*/

#include <iostream>
#include <math.h>
#include <vector>
#include <ctime>
#include <random>

#include <SFML\Graphics.hpp>
#include "Net.h"
#include "Fish.h"

bool sortByFitness(Fish const& a, Fish const& b);

class Tank
{
	public:

	vector<Fish> population;
	vector<sf::CircleShape> food;
	sf::Vector2f spawnPoint;

	int width_screen = 1080;
	int height_screen = 600;
	int safezone_y;
	int minumum_fish = 16;
	int minumum_food;

	int num_input_neurons = 16;
	int num_hidden_layers = 1;
	int num_neurons_in_hidden_layer = 9;
	int num_output_neurons = 5;

	Tank();

	void startTank();
	void spawnPellets();
	void handleCollisions();
	void handleDeaths();
	void addNewFish();
	sf::Vector2f getRandomBottomSpawn();
	sf::Vector2f getRandomTopSpawn();
	int getSize_fish();
	int getSize_food();
};

