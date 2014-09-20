// Particle.h - Particle class declaration
// Written by Ryan Geiger

#pragma once
#include "SDL.h"

class Particle
{
private:
	// Offsets
	int X, Y;

	// Origin particle is generated from
	int OriginX, OriginY;

	// Amount of frames particle will live for
	int FrameLife;

public:
	// Constructor
	Particle( int x, int y );

	// Shows the particle
	void Show( SDL_Surface* image, SDL_Surface* destination );

	// Moves particle to create explosion effect
	void Move();

	// Frames particle has been alive
	int Frame;

	// Checks if particle is dead
	bool IsDead() const;
};

// 15