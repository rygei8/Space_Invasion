// Asteroid.h - Asteroid class declaration
// Written by Ryan Geiger

#pragma once
#include "SDL.h"
#include "UserShip.h"

#define ASTEROID_WIDTH		16
#define ASTEROID_HEIGHT		16

class Asteroid: public UserShip
{
private:
	// Angle at which asteroid enters screen - 0 is 
	double Theta;

public:
	// Default Constructor
	Asteroid();

	// Destructor
	~Asteroid() {};

	// Return width/height of Asteroid
	int Width() const { return ASTEROID_WIDTH; }
	int Height() const { return ASTEROID_HEIGHT; }

	// Changes the position of Asteroid and Particles
	void Move();

	// Returns true if Asteroid leaves screen
	bool EndOfScreen();

};

// 18