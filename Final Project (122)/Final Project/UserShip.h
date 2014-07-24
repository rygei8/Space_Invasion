// UserShip.h - UserShip class declaration
// Written by Ryan Geiger

#pragma once
#include "SDL.h"
#include <vector>
#include "Particle.h"
using namespace std;

#define SHIP_WIDTH		55
#define SHIP_HEIGHT		60

class UserShip
{
protected:
	// Position
	int X, Y;

	// Velocity
	int XVelocity, YVelocity;

	// Particles that appear when ship explodes
	vector<Particle*> Particles;

	// Keeps track of whether ship has been destroyed
	bool Dead;

public:	
	friend class Master;

	// Constructor
	UserShip();

	// Destructor
	~UserShip();

	// Return X, Y positions
	int GetX() const { return X; }
	int GetY() const { return Y; }

	// Return ship height and width, don't declare const or issues appear while using polymorphism
	virtual int Height() const { return SHIP_HEIGHT; }
	virtual int Width() const { return SHIP_WIDTH; }

	// Applies UserShip to the screen
	void Show( SDL_Surface* shipImage, SDL_Surface* particleImage, SDL_Surface* destination );

	// Changes the position of UserShip and Particles
	virtual void Move();

	void Explode();

	// Returns true if ship is at end of screen
	virtual bool EndOfScreen();
};

// 28