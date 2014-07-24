// Particle.cpp - Particle class implementation
// Written by Ryan Geiger

#pragma once
#include "Particle.h"
#include "Utilities.h"

#define PARTICLE_DISTANCE	40
#define FRAME_LIFE_ONE		10
#define FRAME_LIFE_TWO		15
#define FRAME_LIFE_THREE	20

// Constructor
Particle::Particle( int x, int y )
{
	// Set Origin
	OriginX = x;
	OriginY = y;

	// Set offsets
	X = OriginX - PARTICLE_DISTANCE + ( rand() % ( PARTICLE_DISTANCE * 2 + 1 ) );
	Y = OriginY - PARTICLE_DISTANCE + ( rand() % ( PARTICLE_DISTANCE * 2 + 1 ) );

	// Initialize Frame
	Frame = 0;

	// Initialize FrameLife randomly
	if( rand() % 2 > 0 )
		FrameLife = FRAME_LIFE_ONE;
	else
	{
		if( rand() % 3 > 0 )
			FrameLife = FRAME_LIFE_TWO;
		else
			FrameLife = FRAME_LIFE_THREE;
	}
}

// Shows the particle
void Particle::Show( SDL_Surface* image, SDL_Surface* destination )
{
	// Show image
	applySurface( X, Y, image, destination );
}

// Moves particle to create explosion effect
void Particle::Move()
{
	// Continue to move particle away from origin of explosion
	if( X < OriginX )
		X--;
	else
		X++;

	if( Y < OriginY )
		Y--;
	else
		Y++;
}

// Checks if particle is dead
bool Particle::IsDead() const
{
	if( Frame > FrameLife )
		return true;

	return false;
}

// 45