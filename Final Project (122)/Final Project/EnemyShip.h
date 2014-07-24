// EnemyShip.h - EnemyShip class declaration
// Written by Ryan Geiger

#pragma once
#include "UserShip.h"
#include "Timer.h"

#define ENEMY_SHIP_WIDTH	20
#define ENEMY_SHIP_HEIGHT	20

class EnemyShip: public UserShip
{
public:
	// Init Constructor
	EnemyShip( UserShip* enemy );

	// Return width/height of EnemyShip
	int Width() const { return ENEMY_SHIP_WIDTH; }
	int Height() const { return ENEMY_SHIP_HEIGHT; }

	// Changes the position of EnemyShip and Particles
	void Move();

	// Returns true if EnemyShip leaves screen
	bool EndOfScreen();

	// Timer used for keeping track of how often ship should shoot, started in constructor (shooting done in Master class)
	Timer ShootTimer;
};

// 14