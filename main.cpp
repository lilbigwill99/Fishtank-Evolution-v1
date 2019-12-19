/*
main.cpp
Will Massey
*/

#include <SFML\Graphics.hpp>
#include "Net.h"
#include "Tools.h"
#include "Fish.h"
#include "Tank.h"

#include <iostream>
#include <math.h>
#include <vector>
#include <ctime>
#include <random>

using namespace std;

const sf::Color BACKGROUND_BLUE(66, 77, 153, 255);
const sf::Color SAFEZONE_BLUE(60, 65, 135, 255);

int main()
{
	srand(time(0));
	Tools toolbox;

	Tank fishtank;

	//inputs
	cout << "Number of fish (slower computers stay under 20): ";
	int number_of_fish;
	cin >> number_of_fish;
	fishtank.minumum_fish = number_of_fish;

	cout << "Food multiplier (default 25): ";
	int food_multiplier;
	cin >> food_multiplier;
	fishtank.minumum_food = fishtank.minumum_fish * food_multiplier;

	cout << "Mutation Rate (1 = 0.1%, 1000 = 100.0%) (default 23): ";
	double mutation_rate;
	cin >> mutation_rate;
	MUTATION_RATE = mutation_rate;

	sf::RenderWindow window(sf::VideoMode(fishtank.width_screen, fishtank.height_screen), "FISHTANK Evolution");
	window.setFramerateLimit(30);

	sf::RectangleShape safeZone;
	safeZone.setSize(sf::Vector2f(window.getSize().x, window.getSize().y / 3));
	safeZone.setPosition(0, window.getSize().y / 3 * 2);
	safeZone.setFillColor(SAFEZONE_BLUE);

	fishtank.startTank();
	sf::Clock fps_clock;

	//starts the main window/loop
	while (window.isOpen())
	{	
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// // // clear, update, draw, display // // //
		
		window.clear(BACKGROUND_BLUE);

		// updates //

		fishtank.spawnPellets();

		float time;

		for (int i = 0; i < fishtank.population.size(); i++)
		{
			time = fps_clock.getElapsedTime().asMilliseconds();
			fishtank.population[i].update(time, fishtank);
		}

		fps_clock.restart();
		fishtank.handleCollisions();
		fishtank.handleDeaths();

		// drawing //
		
		window.draw(safeZone);

		for (int f = 0; f < fishtank.getSize_fish(); f++)
		{
			for (int s = 0; s < fishtank.population[f].sensors.size(); s ++)
			{
				window.draw(fishtank.population[f].sensors[s]);
			}

			window.draw(fishtank.population[f].body);
		}

		for (int p = 0; p < fishtank.getSize_food(); p++)
		{
			window.draw(fishtank.food[p]);
		}

		// display //

		window.display();

	} // ends the main loop

	return 0;
}