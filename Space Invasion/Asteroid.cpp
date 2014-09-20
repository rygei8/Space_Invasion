// Asteroid.cpp - Asteroid class implementation
// Written by Ryan Geiger

#pragma once
#include "Asteroid.h"
#include <math.h>

#define ASTEROID_VELOCITY	4.0
#define SCREEN_WIDTH		640
#define SCREEN_HEIGHT		480
#define PI					3.14159265

// Default Constructor
Asteroid::Asteroid()
{
	// Initialize velocities, position, and theta
	// Angle will be between -45 and 45 degrees
	Theta = ( rand() % 91 ) - 45;

	// Velocities will be determined using trigonometry and Theta, which is converted from degrees to radians
	XVelocity = ASTEROID_VELOCITY * sin( Theta * ( PI/180.0 ) );
	YVelocity = ASTEROID_VELOCITY * cos( Theta * ( PI/180.0 ) );

	// Asteroid starts at a random spot along the top of the screen
	X = rand() % ( SCREEN_WIDTH - Width() );
	Y = -Height();

	Dead = false;
}

// Changes the position of Asteroid and Particles
void Asteroid::Move()
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

// Returns true if Asteroid leaves screen
bool Asteroid::EndOfScreen()
{
	return 
		Y > SCREEN_HEIGHT || X + Width() < 0 || X > SCREEN_WIDTH;
}

// 59