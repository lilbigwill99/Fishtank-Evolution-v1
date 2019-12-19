#pragma once

/*
Tools.h
Will Massey
Random Collection of Useful Functions
*/

#include <SFML\Graphics.hpp>

#include <iostream>
#include <math.h>
#include <vector>
#include <ctime>
#include <random>

using namespace std;

extern double MUTATION_RATE;

class Tools
{
	public:

	double mutationRate = MUTATION_RATE;

	double generateRandomNumber();
	sf::Color generateRandomColor();
	void printVector(vector<bool> v);
	void printVector(vector<double> v);
	
	vector<double> createNewDNA(vector<double> firstDNA, vector<double> secondDNA);
	sf::Color createNewColor(sf::Color firstColor, sf::Color secondColor);
};

