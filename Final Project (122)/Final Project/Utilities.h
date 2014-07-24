// Utilities.h - function declaration
// Written by Ryan Geiger

#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include <string>
using namespace std;

// Initializes SDL subsystems, sets screen and window caption
bool initialize( SDL_Surface* &screen );

// Loads and optimizes image
SDL_Surface* loadImage( const string& filename );

// Applies one surface to another at specified position
void applySurface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL );

// Closes SDL, SDL_mixer, TTF Font
void cleanUp();

// 11