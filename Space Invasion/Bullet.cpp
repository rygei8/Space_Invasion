// Bullet.cpp - Bullet class implementation
// Written by Ryan Geiger

#pragma once
#include "Bullet.h"
#include "Utilities.h"

// Constructor
Bullet::Bullet( int x, int y, int bulletVelocity )
{
	// Initialize velocities, positions, dead/alive
	// Uses optional parameter for bulletVelocity so that user can change direction/speed of bullet if necessary
	XVelocity = 0;
	YVelocity = bulletVelocity;

	X = x;
	Y = y;

	Dead = false;
}

// Destructor
Bullet::~Bullet()
{
	// delete Particles
	for( unsigned int i = 0; i < Particles.size(); i++ )
		delete Particles[i];
}

// Changes the position of Bullet and Particles
void Bullet::Move()
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

// 55