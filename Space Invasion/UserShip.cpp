// UserShip.cpp - UserShip class implementation
// Written by Ryan Geiger

#pragma once
#include "UserShip.h"
#include "Utilities.h"

#define SCREEN_WIDTH		640
#define SCREEN_HEIGHT		480
#define EXPLOSION_PARTICLES	45

// Constructor
UserShip::UserShip()
{
	// Initialize position, velocity 
	X = SCREEN_WIDTH/2 - Width()/2;
	Y = SCREEN_HEIGHT - Height();
	XVelocity = 0;
	YVelocity = 0;

	// UserShip is not yet destroyed
	Dead = false;
}

// Destructor
UserShip::~UserShip()
{
	// delete Particles
	for( unsigned int i = 0; i < Particles.size(); i++ )
		delete Particles[i];
}

// Applies UserShip to the screen
void UserShip::Show( SDL_Surface* shipImage, SDL_Surface* particleImage, SDL_Surface* destination )
{
	if( !Dead )
		applySurface( X, Y, shipImage, destination );

	for( unsigned int i = 0; i < Particles.size(); i++ )
		Particles[i]->Show( particleImage, destination ); 
}

// Changes the position of UserShip and Particles
void UserShip::Move()
{
	if( !Dead )
	{
		// Move left/right
		X += XVelocity;

		// If UserShip went too far left/right
		if( ( X < 0 ) || ( X + Width() > SCREEN_WIDTH ) )
		{
			// Move back
			X -= XVelocity;    
		}

		// Move up/down
		Y += YVelocity;

		// If UserShip went too far up/down
		if( ( Y < 0 ) || ( Y + Height() > SCREEN_HEIGHT ) )
		{
			// Move back
			Y -= YVelocity;    
		}

	}

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

void UserShip::Explode()
{
	if( !Dead )
	{
		for( int i = 0; i < EXPLOSION_PARTICLES; i++ )
			Particles.push_back( new Particle( X + ( Width()/2 ), Y + ( Height()/2 ) ) );
	}
}

// Returns true if ship is at end of screen
bool UserShip::EndOfScreen()
{
	return 
		Y + Height() < 0 || Y > SCREEN_HEIGHT || X + Width() < 0 || X > SCREEN_WIDTH;
}

// 88