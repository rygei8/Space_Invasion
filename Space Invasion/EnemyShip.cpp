// EnemyShip.cpp - EnemyShip class implementation
// Written by Ryan Geiger

#pragma once
#include "EnemyShip.h"

#define ENEMY_SHIP_X_VELOCITY	1.0
#define ENEMY_SHIP_Y_VELOCITY	2.0
#define SCREEN_WIDTH			640
#define SCREEN_HEIGHT			480

// Init Constructor
EnemyShip::EnemyShip( UserShip* enemy )
{
	// EnemyShip starts at a random spot along the top of the screen
	X = rand() % ( SCREEN_WIDTH - Width() );
	Y = -Height();

	// Ship's XVelocity goes in direction of enemy, YVelocity is constant
	if( enemy->GetX() > X )
		XVelocity = ENEMY_SHIP_X_VELOCITY;
	else
		XVelocity = -ENEMY_SHIP_X_VELOCITY;

	YVelocity = ENEMY_SHIP_Y_VELOCITY;

	ShootTimer.Start();
	Dead = false;
}

// Changes the position of EnemyShip and Particles
void EnemyShip::Move()
{
	// Move left/right
	X += XVelocity;

	// Move up/down
	Y += YVelocity;

	// Move Particles
	for( unsigned int i = 0; i < Particles.size(); i++ )
	{
		Particles[i]->Move();
		Particles[i]->Frame++;

		if( Particles[i]->IsDead() )
		{
			delete Particles[i];
			vector<Particle*>::iterator iter = Particles.begin();
			iter += i;
			Particles.erase( iter );
			i--;
		}
	}
}

// Returns true if EnemyShip leaves screen
bool EnemyShip::EndOfScreen()
{
	return 
		Y > SCREEN_HEIGHT || X + Width() < 0 || X > SCREEN_WIDTH;
}