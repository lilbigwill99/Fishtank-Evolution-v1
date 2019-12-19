/*
Tools.cpp
Will Massey
*/

#include "Tools.h"
#include <SFML\Graphics.hpp>

#include <iostream>
#include <math.h>
#include <vector>
#include <ctime>
#include <random>

double MUTATION_RATE = 23;

double Tools::generateRandomNumber()
{
	/*
		generating a custom number, in this case between -2 and 2
	*/
	double number = (rand() % 2000) - 1000;
	number /= 500;
	return number;
}

sf::Color Tools::generateRandomColor()
{
	/*
		generating a random rbg value
	*/
	double r = (rand() % 2550) / 10;
	double g = (rand() % 2550) / 10;
	double b = (rand() % 2550) / 10;
	return sf::Color(r, g, b);
}

void Tools::printVector(vector<bool> v)
{
	/*
		debug tool
	*/
	for (int i = 0; i < v.size(); i++)
	{
		cout << v[i] << endl;
	}
}

void Tools::printVector(vector<double> v)
{
	/*
		overloaded debug tool
	*/
	for (int i = 0; i < v.size(); i++)
	{
		cout << v[i] << endl;
	}
}

vector<double> Tools::createNewDNA(vector<double> firstDNA, vector<double> secondDNA)
{
	/*
		takes two vectors and creates a new one based on a genetic algorithm
	*/
	vector<double> newDNA = firstDNA;
	int size = newDNA.size();

	for (int i = 0; i < size; i++)
	{
		if (rand() % 1000 < mutationRate)
		{
			newDNA[i] += generateRandomNumber();
		}
		else
		{
			if (rand() % 2 == 0)
			{
				newDNA[i] = firstDNA[i];
			}
			else
			{
				newDNA[i] = secondDNA[i];
			}
		}
	}

	return newDNA;
}

sf::Color Tools::createNewColor(sf::Color firstColor, sf::Color secondColor)
{
	/*
		takes two rbg values and creates a new one using a genetic algorithm
	*/
	sf::Color newColor = firstColor;

	if (rand() % 2 == 0)
	{
		newColor.r = firstColor.r;
	}
	else
	{
		newColor.r = secondColor.r;
	}

	if (rand() % 2 == 0)
	{
		newColor.g = firstColor.g;
	}
	else
	{
		newColor.g = secondColor.g;
	}

	if (rand() % 2 == 0)
	{
		newColor.b = firstColor.b;
	}
	else
	{
		newColor.b = secondColor.b;
	}

	if (rand() % 1000 < mutationRate)
	{
		newColor.r = rand() % 255;
	}

	if (rand() % 1000 < mutationRate)
	{
		newColor.g = rand() % 255;
	}

	if (rand() % 1000 < mutationRate)
	{
		newColor.b = rand() % 255;
	}

	return newColor;
}
