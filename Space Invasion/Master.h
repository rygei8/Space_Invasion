// Master.h - Master class declaration
// Written by Ryan Geiger

#pragma once
#include <vector>
#include "SDL.h"
#include "Utilities.h"
#include "Sound.h"
#include "UserShip.h"
#include "Bullet.h"
#include "Asteroid.h"
#include "EnemyShip.h"
#include "Timer.h"

class Master
{
private:
	// Images used in game
	SDL_Surface* ShipImage;
	SDL_Surface* BulletImage;
	SDL_Surface* AsteroidImage;
	SDL_Surface* EnemyShipImage;
	SDL_Surface* EnemyBulletImage;
	SDL_Surface* ParticleImage;

	// Font used for on screen words
	TTF_Font* Font;

	// Color of Font
	SDL_Color FontColor;

	// Bullet shooting sound effect
	Sound BulletSound;

	// Explosion sound effect
	Sound ExplosionSound;

	// Ship that user controls
	UserShip* Ship;

	// Stores EnemyShips
	vector<EnemyShip*> EnemyShips;

	// Stores Asteroids
	vector<Asteroid*> Asteroids;

	// Bullets that UserShip will shoot
	vector<Bullet*> UserBullets;

	// Bullets that EnemyShips will shoot
	vector<Bullet*> EnemyBullets;

	// User's Score
	int Score;

	// Highscore obtained from save file
	int HighScore;

	// Keeps track of how often enemies are spawned
	Timer AsteroidSpawn;
	Timer EnemyShipSpawn;

public:
	// Constructor
	Master();

	// Destructor
	~Master();

	// Handles user input
	void HandleInput( SDL_Event& event );

	// Checks for collisions, moves everything on screen, and spawns enemies
	void Move();

	// Checks for collision between two on screen ships, bullets, asteroids, etc
	bool CheckCollision( UserShip* one, UserShip* two );

	// Applies Images to the screen
	void Show( SDL_Surface* destination );

	// Shoots UserShip's Bullets
	void UserShoot();

	// Shoots EnemyShips' Bullets
	void EnemyShoot();

	// Spawns enemies
	void Spawn();

	// Resets game if user wishes to play again
	void Reset();

	// Save HighScore
	void Save();

};