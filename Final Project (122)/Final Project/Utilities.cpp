// Utilities.cpp - function implementation
// Written by Ryan Geiger

#pragma once
#include "Utilities.h"

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480
#define SCREEN_BPP		32

// Initializes SDL subsystems, sets screen and window caption
bool initialize( SDL_Surface* &screen )
{
	// Initialize all SDL subsystems
	if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
		return false;    

	// Set up the screen
	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

	// If there was an error in setting up the screen
	if( screen == NULL )
		return false;    

	// Initialize SDL_mixer
	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 512 ) == -1 )
		return false;    

	// Initialize TTF font
	if( TTF_Init() == -1 )
		return false;

	// Set the window caption
	SDL_WM_SetCaption( "Final Project", NULL );

	// Seed random number generator
	srand( SDL_GetTicks() * 100000000 );

	// If everything initialized fine
	return true;
}

// Closes SDL, SDL_mixer, TTF Font
void cleanUp()
{
	// Quit SDL_mixer
	Mix_CloseAudio();

	// Quit TTF font
	TTF_Quit();

	// Quit SDL
	SDL_Quit();
}

// Loads and optimizes image
SDL_Surface* loadImage( const string& filename ) 
{
	//The image that's loaded
	SDL_Surface* loadedImage = NULL;

	//The optimized image that will be used
	SDL_Surface* optimizedImage = NULL;

	//Load the image
	loadedImage = IMG_Load( filename.c_str() );

	//If the image loaded
	if( loadedImage != NULL )
	{
		//Create an optimized image
		optimizedImage = SDL_DisplayFormat( loadedImage );

		//Free the old image
		SDL_FreeSurface( loadedImage );
	}

	//Return the optimized image
	return optimizedImage;
}

// Applies one surface to another at specified position
void applySurface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip )
{
	//Temporary rectangle to hold the offsets
	SDL_Rect offset;

	//Get the offsets
	offset.x = x;
	offset.y = y;

	//Blit the surface
	SDL_BlitSurface( source, clip, destination, &offset );
}

// 45