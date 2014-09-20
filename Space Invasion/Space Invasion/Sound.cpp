// Sound.cpp- Sound class implementation
// Written by Ryan Geiger

#pragma once

#include "Sound.h"

Sound::Sound()
{
	// Initialize Effect to NULL
	Effect = NULL;
}

Sound::~Sound()
{
	Mix_FreeChunk( Effect );
}

void Sound::Load( char* filename )
{
	Effect = Mix_LoadWAV( filename );
}

void Sound::Play() const
{
	Mix_PlayChannel( -1, Effect, 0 );
}

// 18