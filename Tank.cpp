/*
Tank.cpp
Will Massey
*/

#include "Tank.h"
#include "Fish.h"
#include "Net.h"
#include "Tools.h"
#include <SFML\Graphics.hpp>

#include <iostream>
#include <math.h>
#include <vector>
#include <ctime>
#include <random>

Tank::Tank()
{
	/*
		setting up the enviornment 
	*/
	safezone_y = height_screen / 3;
	safezone_y *= 2;

	minumum_food = minumum_fish * 25;
	spawnPoint = sf::Vector2f(width_screen / 2, height_screen - 100);
}

void Tank::startTank()
{
	/*
		intializes the tank
	*/
	for (int i = 0; i < minumum_fish; i++)
	{
		addNewFish();
	}

	spawnPellets();
}

void Tank::spawnPellets()
{
	/*
		spawns minimum amount of pellets
	*/
	sf::CircleShape pellet;
	pellet.setFillColor(sf::Color::Green);
	pellet.setRadius(2);
	pellet.setOrigin(sf::Vector2f(pellet.getRadius() / 2, pellet.getRadius() / 2));
	
	int topFood = minumum_food / 3 * 2; // two thirds at the top
	int bottomFood = minumum_food / 3; // one third at the bottom
	int topCount = 0;
	int bottomCount = 0;

	for (int i = 0; i < food.size(); i++)
	{
		//checking if food is in the safezone/bottom of the tank
		if(food[i].getPosition().y < height_screen / 3 * 2)
		{
			topCount += 1;
		}
		else if (food[i].getPosition().y >= height_screen / 3 * 2)
		{
			bottomCount += 1;
		}

	}
	
	while (bottomCount < bottomFood)
	{
		pellet.setPosition(getRandomBottomSpawn());
		food.push_back(pellet);
		bottomCount++;
	}

	while (topCount < topFood)
	{
		pellet.setPosition(getRandomTopSpawn());
		food.push_back(pellet);
		topCount++;
	}
}

void Tank::handleCollisions()
{
	/*
		checking collision between food and other fish
	*/
	
	//go through every fish and see if the body is colliding with food or another fish
	for (int target = 0; target < getSize_fish(); target++)
	{
		//checking for food collision
		for (int f = 0; f < getSize_food(); f++)
		{
			if (population[target].body.getGlobalBounds().intersects(food[f].getGlobalBounds()))
			{
				food.erase(food.begin() + f);
				population[target].currentEnergy += 1500;
				population[target].fitnessScore += 1;
			}
		}
	}
}

void Tank::handleDeaths()
{
	/*
		replacing dead fish with offspring with the two best fish
	*/
	for (int i = getSize_fish() - 1; i >= 0; i--)
	{
		if (population[i].currentEnergy <= 0)
		{
			population.erase(population.begin() + i);
		}
	}

	while (getSize_fish() < minumum_fish)
	{
		sort(population.begin(), population.end(), sortByFitness);

		vector<double> first_weights = population[0].getDNA_weights();
		vector<double> first_bias = population[0].getDNA_bias();
		vector<double> second_weights = population[1].getDNA_weights();
		vector<double> second_bias = population[1].getDNA_bias();

		sf::Color firstColor_body = population[0].getBodyColor();
		sf::Color firstColor_sensor = population[0].getSensorColor();
		sf::Color secondColor_body = population[1].getBodyColor();
		sf::Color secondColor_sensor = population[1].getSensorColor();

		Tools toolbox;
		vector<double> newDNA_weights = toolbox.createNewDNA(first_weights, second_weights);
		vector<double> newDNA_bias = toolbox.createNewDNA(first_bias, second_bias);
		sf::Color newColor_body = toolbox.createNewColor(firstColor_body, secondColor_body);
		sf::Color newColor_sensor = toolbox.createNewColor(firstColor_sensor, secondColor_sensor);

		Fish newFish;
		newFish.createRandomFish(num_input_neurons, num_hidden_layers, num_neurons_in_hidden_layer, num_output_neurons);
		newFish.setDNA_weights(newDNA_weights);
		newFish.setDNA_bias(newDNA_bias);
		newFish.setBodyColor(newColor_body);
		newFish.setSensorColor(newColor_sensor);
		//newFish.setPosition(spawnPoint);
		newFish.setPosition(getRandomBottomSpawn());
		population.push_back(newFish);
	}
}

void Tank::addNewFish()
{
	/*
		adds a random new fish
	*/
	Fish newFish;
	newFish.createRandomFish(num_input_neurons, num_hidden_layers, num_neurons_in_hidden_layer, num_output_neurons);
	//newFish.setPosition(spawnPoint);
	newFish.setPosition(getRandomBottomSpawn());
	population.push_back(newFish);
}

sf::Vector2f Tank::getRandomBottomSpawn()
{
	/*
		get a random position in the safe zone
	*/
	int x = rand() % width_screen;
	int y = height_screen / 3 * 2;
	y += rand() % (height_screen - y);
	return sf::Vector2f(x, y);
}

sf::Vector2f Tank::getRandomTopSpawn()
{
	/*
		get a random position in the hostile zone
	*/
	int x = rand() % width_screen;
	int y = rand() % height_screen / 3 * 2;
	return sf::Vector2f(x, y);
}

int Tank::getSize_fish()
{
	/*
		gets the current amount of fish in the tank
	*/
	return population.size();
}

int Tank::getSize_food()
{
	/*
		gets the current amount of food pellets in the tank
	*/
	return food.size();
}

bool sortByFitness(Fish const& a, Fish const& b)
{
	/*
		sorts the fish vector based on fitness
	*/
	return a.fitnessScore > b.fitnessScore;
}
