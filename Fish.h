#pragma once

/*
Fish.h
Will Massey
Neural Network Controlled Simulated Fish
*/

#include <iostream>
#include <math.h>
#include <vector>
#include <ctime>
#include <random>

#include <SFML\Graphics.hpp>
#include "Net.h"

class Tank;

using namespace std;

const float MAX_SPEED = 0.8;
const float MAX_TURN = 2.45;
const int MAX_ENERGY = 5400; 

enum SESNOR_SETUP
{
	LEFT,
	RIGHT,
	BACK
};

class Fish
{
	public:

	Net brain;

	sf::RectangleShape body;
	vector<sf::RectangleShape> sensors;
	sf::Clock matingTimer;

	//inputs
	double currentEnergy = MAX_ENERGY;
	vector<bool> seeFood = {false, false, false};
	vector<bool> seeFish = {false, false, false};
	vector<double> seeFish_friendliness = { 0, 0, 0 };
	bool isSafe = false; //whether or not fish is in the safezone
	double memory1 = 0;
	double memory2 = 0;

	//outputs
	double desiredMovementSpeed = 0;
	double desiredTurnSpeed = 0;
	double friendliness = 1; // < -0.6 eat fish, > +0.6 mate
	//memory gets fed back into inputs

	//genetics
	vector<double> dna_weights;
	vector<double> dna_bias;

	//statistics and stuff
	sf::Clock timeAlive;
	int generation = 1; 
	int fitnessScore = 0;

	Fish();
	
	void gatherInputs(Tank fishtank);
	void gatherOutputs();

	void update(float ellapsedTime, Tank fishtank); 
	void rotate(float amountOfRotation);
	void move(float ellapsedTime);
	void alignSensors();
	void feedForward();
	void depleteEnergy(double amount);
	sf::Vector2f getPosition();
	void setPosition(sf::Vector2f pos);
	void setPosition(float pos_x, float pos_y);
	double getRotation();
	void setBodyColor(sf::Color color);
	void setSensorColor(sf::Color color);
	sf::Color getBodyColor();
	sf::Color getSensorColor();
	void createRandomFish(int numInputNeurons, int numHiddenLayers,
		int numHiddenLayerNeurons, int numOutputNeurons);
	vector<double> getDNA_weights();
	vector<double> getDNA_bias();
	void setDNA_weights(vector<double> newDNA);
	void setDNA_bias(vector<double> newDNA);
};

