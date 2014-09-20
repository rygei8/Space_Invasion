// Sound.h- Sound class declaration
// Written by Ryan Geiger

#pragma once

#include "SDL_mixer.h"
using namespace std;

class Sound
{
private:
	Mix_Chunk* Effect;

public:
	// Default constructor
	Sound();

	// Destructor
	~Sound();

	// Loads sound file
	void Load( char* filename );

	// Play sound effect
	void Play() const;

};

// 13