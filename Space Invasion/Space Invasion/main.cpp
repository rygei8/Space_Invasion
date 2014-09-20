// main.cpp - Space Invasion game program main entry point
// Written by Ryan Geiger
// December 2013

#include "SDL.h"
#include "Master.h"
#include "Timer.h"
#include "Utilities.h"
using namespace std;

#define FRAMES_PER_SECOND		60

int main( int argc, char* args[] )
{	
	// Initialize screen surface
	SDL_Surface* screen = NULL;

	// SDL_Event allows you to recognize and handle inputs
	SDL_Event event;

	// quit variable used to stop game loop
	bool quit = false;

	// Ends program if SDL does not start up properly
	if( !initialize( screen ) )
		return 1;

	// Timers to regulate frame rate, spawn rate
	Timer fps;

	// Load/apply background
	SDL_Surface* background = loadImage( "background.jpg" );
	applySurface( 0, 0, background, screen );

	// Create instance of Master class (controls all parts of game) and apply all images to the screen
	Master gameMaster;
	gameMaster.Show( screen );

	// Update the screen
	SDL_Flip( screen );

	// Game loop
	while( !quit )
	{
		// Start frame rate regulator at beginning of each frame
		fps.Start();

		// Handle Input
		while( SDL_PollEvent( &event ) )
		{
			gameMaster.HandleInput( event );

			// if user X's out screen then quit game loop
			if( event.type == SDL_QUIT )
				quit = true;
		}

		// Logic - moves images, checks for collisions, spawn enemies
		gameMaster.Move();

		// Reapply background
		applySurface( 0, 0, background, screen );

		// Show images
		gameMaster.Show( screen );

		// Update screen
		SDL_Flip( screen );

		// Cap the frame rate
		if( fps.GetTicks() < 1000 / FRAMES_PER_SECOND )
		{
			// Sleep the remaining frame time
			SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.GetTicks() );
		}
	}

	// Save game
	gameMaster.Save();

	// Free images
	SDL_FreeSurface( background );

	// Closes SDL, SDL_mixer, TTF Font
	cleanUp();

	return 0;
}

// 50