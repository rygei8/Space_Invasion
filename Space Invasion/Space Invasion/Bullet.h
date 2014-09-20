// Bullet.h - Bullet class declaration
// Written by Ryan Geiger

#pragma once
#include "UserShip.h"
#include "Asteroid.h"

#define BULLET_WIDTH	6
#define BULLET_HEIGHT	30
#define BULLET_VELOCITY	-20

class Bullet: public UserShip
{
public:
	// Init Constructor
	Bullet( int x, int y, int bulletVelocity = BULLET_VELOCITY );

	// Destructor
	~Bullet();

	// Return width/height of Bullet
	int Width() const { return BULLET_WIDTH; }
	int Height() const { return BULLET_HEIGHT; }

	// Changes the position of Bullet and Particles
	void Move();
};

// 16