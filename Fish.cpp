/*
Fish.cpp
Will Massey
*/

#include "Fish.h"
#include "Tank.h"
#include "Net.h"
#include "Tools.h"
#include <SFML\Graphics.hpp>

#include <iostream>
#include <math.h>
#include <vector>
#include <ctime>
#include <random>

Fish::Fish()
{
	/*
		building the fishes body
	*/
	sf::RectangleShape newFrontSensors;
	newFrontSensors.setSize(sf::Vector2f(60, 1));
	newFrontSensors.setOrigin(sf::Vector2f(0, newFrontSensors.getSize().y / 2));
	
	sf::RectangleShape newBackSensor;
	newBackSensor.setSize(sf::Vector2f(45, 1));
	newBackSensor.setOrigin(sf::Vector2f(0, newBackSensor.getSize().y / 2));

	sensors.push_back(newFrontSensors);
	sensors.push_back(newFrontSensors);
	sensors.push_back(newBackSensor);
	sensors[LEFT].rotate(-17);
	sensors[RIGHT].rotate(17);
	sensors[BACK].rotate(180);

	body.setSize(sf::Vector2f(12, 12));
	body.setOrigin(body.getSize().x / 2, body.getSize().y / 2);
}

void Fish::gatherInputs(Tank fishtank)
{
	/*
		scans the tank to grab valuable information for the brain
	*/
	
	//resetting inputs
	for (int i = 0; i < sensors.size(); i++)
	{
		seeFood[i] = false;
		seeFish[i] = false;
		seeFish_friendliness[i] = 0;
	}

	isSafe = false;
	
	//trying to keep inputs as normalized as possible, between 0 and 1
	vector<double> newInputs;

	newInputs.push_back(currentEnergy / MAX_ENERGY);
	newInputs.push_back(getRotation() / 360);
	newInputs.push_back(getPosition().x / fishtank.width_screen);
	newInputs.push_back(getPosition().y / fishtank.height_screen);

	//for each sensor, must check vision for food and other fish
	for (int s = 0; s < sensors.size(); s++)
	{
		//checking vision for other fish
		for (int f = 0; f < fishtank.getSize_fish(); f++)
		{
			if (getPosition().x != fishtank.population[f].getPosition().x
				&& getPosition().y != fishtank.population[f].getPosition().y)
			{
				if (sensors[s].getGlobalBounds()
					.intersects(fishtank.population[f].body.getGlobalBounds()))
				{
					seeFish[s] = true;
					seeFish_friendliness[s] = fishtank.population[f].friendliness;
				}
			}
		}

		//checking vision for food
		for (int f = 0; f < fishtank.getSize_food(); f++)
		{
			if (sensors[s].getGlobalBounds().intersects(fishtank.food[f].getGlobalBounds()))
			{
				seeFood[s] = true;
			}
		}

		newInputs.push_back(seeFish[s]);
		newInputs.push_back(seeFish_friendliness[s]);
		newInputs.push_back(seeFood[s]);
	}

	if (getPosition().y >= fishtank.safezone_y)
	{
		isSafe = true;
	}

	newInputs.push_back(isSafe);
	newInputs.push_back(memory1);
	newInputs.push_back(memory2);

	brain.setInputs(newInputs);

}

void Fish::gatherOutputs()
{
	/*
		checking the brain to see what the fish wants to do
	*/
	desiredMovementSpeed = brain.layers.back().neurons[0].output;
	desiredTurnSpeed = brain.layers.back().neurons[1].output;
	friendliness = brain.layers.back().neurons[2].output;
	memory1 = brain.layers.back().neurons[3].output;
	memory2 = brain.layers.back().neurons[4].output;
}

void Fish::update(float ellapsedTime, Tank fishtank) 
{
	/*
		frame by frame updates
	*/
	gatherInputs(fishtank);
	feedForward();
	gatherOutputs();
	rotate(desiredTurnSpeed * MAX_TURN);
	move(ellapsedTime);
	depleteEnergy(1); // -1 energy a frame

	//in Tank::update we will check collisions and handle deaths
}

void Fish::rotate(float amountOfRotation)
{
	/*
		rotating the fish x degrees
	*/
	body.rotate(amountOfRotation);
	sensors[LEFT].rotate(amountOfRotation);
	sensors[RIGHT].rotate(amountOfRotation);
	sensors[BACK].rotate(amountOfRotation);
}

void Fish::move(float ellapsedTime)
{	
	/*
		moving the fish in it's desired way
	*/
	
	//unused parameter, dont like how it affects movement, plan to fix later
	float newX = cos(body.getRotation() * (3.14159265359 / 180));
	float newY = sin(body.getRotation() * (3.14159265359 / 180));
	newX *= MAX_SPEED * desiredMovementSpeed;
	newY *= MAX_SPEED * desiredMovementSpeed;

	body.move(sf::Vector2f(newX, newY));
	alignSensors();
}

void Fish::alignSensors()
{
	/*
		the sensors need
	*/
	sensors[LEFT].setPosition(body.getPosition());
	sensors[RIGHT].setPosition(body.getPosition());
	sensors[BACK].setPosition(body.getPosition());
}

void Fish::feedForward()
{
	/*
		takes the inputs gathered from the tank and feeds forward to get outputs
	*/
	brain.feedForward();
}

void Fish::depleteEnergy(double amount)
{
	/*
		decrement current energy by x
	*/
	currentEnergy -= amount;
}

sf::Vector2f Fish::getPosition()
{
	/*
		gets window position
	*/
	return body.getPosition();
}

void Fish::setPosition(sf::Vector2f pos)
{
	/*
		sets window position
	*/
	body.setPosition(pos);
	alignSensors();
}

void Fish::setPosition(float pos_x, float pos_y)
{
	/*
		overloaded
	*/
	body.setPosition(sf::Vector2f(pos_x, pos_y));
}

double Fish::getRotation()
{
	/*
		gets current facing direction
	*/
	return (double)body.getRotation();
}

void Fish::setBodyColor(sf::Color color)
{
	/*
		sets the body's rgb value
	*/
	body.setFillColor(color);
}

void Fish::setSensorColor(sf::Color color)
{
	/*
		sets the sensors' rgb value
	*/
	sensors[LEFT].setFillColor(color);
	sensors[RIGHT].setFillColor(color);
	sensors[BACK].setFillColor(color);
}

sf::Color Fish::getBodyColor()
{
	/*
		gets body's rgb value
	*/
	return body.getFillColor();
}

sf::Color Fish::getSensorColor()
{
	/*
		gets sensors' rgb value
	*/
	return sensors[LEFT].getFillColor();
}

void Fish::createRandomFish(int numInputNeurons, int numHiddenLayers,
	int numHiddenLayerNeurons, int numOutputNeurons)
{
	/*
		creates a random fish
	*/
	brain.createRandomNet(numInputNeurons, 
		numHiddenLayers, numHiddenLayerNeurons, numOutputNeurons);
	
	Tools toolbox;
	setBodyColor(toolbox.generateRandomColor());
	setSensorColor(toolbox.generateRandomColor());
}

vector<double> Fish::getDNA_weights()
{
	/*
		gets weights from brain to use in a genetic algorithm 
	*/
	return brain.getDNA_weights();
}

vector<double> Fish::getDNA_bias()
{
	/*
		gets biases from brain to use in a genetic algorithm
	*/
	return brain.getDNA_bias();
}

void Fish::setDNA_weights(vector<double> newDNA)
{
	/*
		sets new weight for the brain
	*/
	brain.setDNA_weights(newDNA);
}

void Fish::setDNA_bias(vector<double> newDNA)
{
	/*
		set new bias for the brain
	*/
	brain.setDNA_bias(newDNA);
}
