// Master.cpp - Master class implementation
// Written by Ryan Geiger

#pragma once
#include "Master.h"
#include <sstream>
#include <fstream>

#define SHIP_VELOCITY			5
#define ASTEROID_KILL			500
#define ENEMY_SHIP_KILL			1000
#define FONT_SIZE				24
#define FONT_COLOR				255
#define SCORE_X_POSITION		20
#define SCORE_Y_POSITION		20
#define HIGHSCORE_X_POSITION	430
#define SCREEN_WIDTH			640
#define SCREEN_HEIGHT			480
#define ASTEROID_SPAWN_RATE		300
#define ENEMY_SHIP_SPAWN_RATE	1000
#define ENEMY_SHOT_FREQ			1000
#define ENEMY_BULLET_VELOCITY	5

// Constructor
Master::Master()
{
	// Load Images and Sounds
	ShipImage = loadImage( "Ship.png" );
	BulletImage = loadImage( "Bullet.png" );
	AsteroidImage = loadImage( "Asteroid.png" );
	EnemyShipImage = loadImage( "EnemyShip.png" );
	EnemyBulletImage = loadImage( "EnemyBullet.png" );
	ParticleImage = loadImage( "Particle.gif" );
	BulletSound.Load( "photongun.wav" );
	ExplosionSound.Load( "explosion.wav" );

	// Load Font, set color (white = 255, 255, 255)
	Font = TTF_OpenFont( "ACKNOWTT.ttf", FONT_SIZE );
	FontColor.r = FONT_COLOR;
	FontColor.g = FONT_COLOR;
	FontColor.b = FONT_COLOR;

	// Dynamically allocate Ship
	Ship = new UserShip;

	// Initialize Score
	Score = 0;

	// Start Spawn timers
	AsteroidSpawn.Start();
	EnemyShipSpawn.Start();

	// Load Highscore from save file
	ifstream gameSave( "Save.txt" );
	gameSave >> HighScore;
	gameSave.close();
}

// Destructor
Master::~Master()
{
	// Free Images
	SDL_FreeSurface( ShipImage );
	SDL_FreeSurface( BulletImage );
	SDL_FreeSurface( AsteroidImage );
	SDL_FreeSurface( EnemyShipImage );
	SDL_FreeSurface( EnemyBulletImage );
	SDL_FreeSurface( ParticleImage );

	// Delete Ship/Bullets/Asteroids/EnemyShips
	for( unsigned int i = 0; i < UserBullets.size(); i++ )
		delete UserBullets[i];
	for( unsigned int i = 0; i < Asteroids.size(); i++ )
		delete Asteroids[i];
	for( unsigned int i = 0; i < EnemyShips.size(); i++ )
		delete EnemyShips[i];
	for( unsigned int i = 0; i < EnemyBullets.size(); i++ )
		delete EnemyBullets[i];
	delete Ship;

	// Close the font that was used
	TTF_CloseFont( Font );
}

// Handles user input
void Master::HandleInput( SDL_Event& event )
{
	// In the event handling, Ship must only be able to Shoot if it is not Dead, but velocity must be able to change even if Ship is Dead,
	//		otherwise velocity can bug out if user is holding directional key down when they Reset the game

	// If a directional key was pressed - adjust velocity
	if( event.type == SDL_KEYDOWN )
	{
		switch( event.key.keysym.sym )
		{
		case SDLK_UP:
			Ship->YVelocity -= SHIP_VELOCITY;
			break;
		case SDLK_DOWN: 
			Ship->YVelocity += SHIP_VELOCITY; 
			break;
		case SDLK_LEFT: 
			Ship->XVelocity -= SHIP_VELOCITY;
			break;
		case SDLK_RIGHT: 
			Ship->XVelocity += SHIP_VELOCITY; 
			break;
		default:
			break;
		}
	}

	// If Space key was pressed and Ship is not Dead - Shoot Bullets, play sound effect
	if( event.type == SDL_KEYDOWN && !Ship->Dead )
	{
		switch( event.key.keysym.sym )
		{
		case SDLK_SPACE:
			BulletSound.Play();
			UserShoot();
			break;
		default:
			break;
		}
	}

	// If 'R' key was pressed and Ship is Dead - Reset game
	if( event.type == SDL_KEYDOWN && Ship->Dead )
	{
		switch( event.key.keysym.sym )
		{
		case SDLK_r:
			Reset();
		default:
			break;
		}
	}

	// If a directional key was released reset velocity so that Ship can stop
	if( event.type == SDL_KEYUP )
	{
		switch( event.key.keysym.sym )
		{
			if( !Ship->Dead )
			{
		case SDLK_UP:
			Ship->YVelocity += SHIP_VELOCITY; 
			break;
		case SDLK_DOWN: 
			Ship->YVelocity -= SHIP_VELOCITY;
			break;
		case SDLK_LEFT: 
			Ship->XVelocity += SHIP_VELOCITY; 
			break;
		case SDLK_RIGHT:
			Ship->XVelocity -= SHIP_VELOCITY;
			break;   
			}
		}        
	}
}

// Checks for collisions, moves everything on screen, and spawns enemies
void Master::Move()
{
	// Check UserShip/Asteroid collisions
	for( unsigned int i = 0; i < Asteroids.size(); i++ )
	{
		// Must check if ship has died since checking collision with other enemies
		// Also must check if Asteroid is dead since it may be dead but not erased if explosion is still occuring
		if( !Ship->Dead && !Asteroids[i]->Dead )
		{
			if( CheckCollision( Ship, Asteroids[i] ) )
			{
				// Explode Ship/Asteroid, play explosion sound effect, kill each
				Ship->Explode();
				Asteroids[i]->Explode();
				ExplosionSound.Play();
				Ship->Dead = true;
				Asteroids[i]->Dead = true;
			}
		}
	}

	// Check UserShip/EnemyShip collisions
	for( unsigned int i = 0; i < EnemyShips.size(); i++ )
	{
		// Must check if ship has died since checking collision with other enemies
		// Also must check if EnemyShip is dead since it may be dead but not erased if explosion is still occuring
		if( !Ship->Dead && !EnemyShips[i]->Dead )
		{
			if( CheckCollision( Ship, EnemyShips[i] ) )
			{
				// Explode Ship/EnemyShip, play explosion sound effect, kill each
				Ship->Explode();
				EnemyShips[i]->Explode();
				ExplosionSound.Play();
				Ship->Dead = true;
				EnemyShips[i]->Dead = true;
			}
		}
	}

	// Check UserShip/EnemyBullet collisions
	for( unsigned int i = 0; i < EnemyBullets.size(); i++ )
	{
		// Must check if ship has died since checking collision with other enemies
		if( !Ship->Dead )
		{
			if( CheckCollision( Ship, EnemyBullets[i] ) )
			{
				// Explode Ship, play explosion sound effect, kill each
				Ship->Explode();
				ExplosionSound.Play();
				Ship->Dead = true;
				EnemyBullets[i]->Dead = true;
			}
		}
	}

	// Check UserBullet collisions with Asteroids, EnemyShips, and EnemyBullets
	for( unsigned int i = 0; i < UserBullets.size(); i++ )
	{
		// Asteroid collisions
		for( unsigned int n = 0; n < Asteroids.size(); n++ )
		{
			// Must check if bullet has died since checking collision with other enemies
			// Also must check if Asteroid is dead since it may be dead but not erased if explosion is still occuring
			if( !UserBullets[i]->Dead && !Asteroids[n]->Dead )
			{
				if( CheckCollision( UserBullets[i], Asteroids[n] ) )
				{
					// Explode asteroid, play explosion sound effect, add to Score, kill each
					Asteroids[n]->Explode();
					ExplosionSound.Play();
					UserBullets[i]->Dead = true;
					Asteroids[n]->Dead = true;
					Score += ASTEROID_KILL;
				}
			}
		}

		// EnemyShip collisions
		for( unsigned int n = 0; n < EnemyShips.size(); n++ )
		{
			// Must check if bullet has died since checking collision with other enemies
			// Also must check if EnemyShip is dead since it may be dead but not erased if explosion is still occuring
			if( !UserBullets[i]->Dead && !EnemyShips[n]->Dead )
			{
				if( CheckCollision( UserBullets[i], EnemyShips[n] ) )
				{
					// Explode EnemyShip, play explosion sound effect, add to Score, kill each
					EnemyShips[n]->Explode();
					ExplosionSound.Play();
					UserBullets[i]->Dead = true;
					EnemyShips[n]->Dead = true;
					Score += ENEMY_SHIP_KILL;
				}
			}
		}

		// EnemyBullet collisions
		for( unsigned int n = 0; n < EnemyBullets.size(); n++ )
		{
			// Must check if bullet has died since checking collision with other enemies
			if( !UserBullets[i]->Dead )
			{
				if( CheckCollision( UserBullets[i], EnemyBullets[n] ) )
				{
					// Kill both Bullets
					UserBullets[i]->Dead = true;
					EnemyBullets[n]->Dead = true;
				}
			}
		}
	}

	// Move Ship
	Ship->Move();

	// Move Bullets, erase if they pass edge of screen or are dead
	for( unsigned int i = 0; i < UserBullets.size(); i++ )
	{
		UserBullets[i]->Move();

		if( UserBullets[i]->EndOfScreen() || UserBullets[i]->Dead )
		{
			delete UserBullets[i];
			vector<Bullet*>::iterator iter = UserBullets.begin();
			iter += i;
			UserBullets.erase( iter );
			i--;
		}
	}

	// Move Asteroids, erase if they pass the edge of the screen or are dead and explosion particles have faded
	// If the Asteroid is erased before the explosion particles are gone there will be no explosion
	for( unsigned int i = 0; i < Asteroids.size(); i++ )
	{
		Asteroids[i]->Move();

		if( Asteroids[i]->EndOfScreen() || ( Asteroids[i]->Dead && Asteroids[i]->Particles.size() == 0 ) )
		{
			delete Asteroids[i];
			vector<Asteroid*>::iterator iter = Asteroids.begin();
			iter += i;
			Asteroids.erase( iter );
			i--;
		}
	}

	// Move EnemyShips and shoot EnemyBullets, erase if they pass the edge of the screen or are dead and explosion particles have faded
	EnemyShoot();
	for( unsigned int i = 0; i < EnemyShips.size(); i++ )
	{
		EnemyShips[i]->Move();


		if( EnemyShips[i]->EndOfScreen() || ( EnemyShips[i]->Dead && EnemyShips[i]->Particles.size() == 0 ) )
		{
			delete EnemyShips[i];
			vector<EnemyShip*>::iterator iter = EnemyShips.begin();
			iter += i;
			EnemyShips.erase( iter );
			i--;
		}
	}

	// Move EnemyBullets, erase if they pass edge of screen or are dead
	for( unsigned int i = 0; i < EnemyBullets.size(); i++ )
	{
		EnemyBullets[i]->Move();

		if( EnemyBullets[i]->EndOfScreen() || EnemyBullets[i]->Dead )
		{
			delete EnemyBullets[i];
			vector<Bullet*>::iterator iter = EnemyBullets.begin();
			iter += i;
			EnemyBullets.erase( iter );
			i--;
		}
	}

	// Spawn enemies
	Spawn();
}

// Checks for collision between two on screen ships, bullets, asteroids, etc
bool Master::CheckCollision( UserShip* one, UserShip* two )
{
	// Set bounds of one
	int leftOne = one->GetX();
	int rightOne = leftOne + one->Width();
	int topOne = one->GetY();
	int bottomOne = topOne + one->Height();

	// Set bounds of two
	int leftTwo = two->GetX();
	int rightTwo = leftTwo + two->Width();
	int topTwo = two->GetY();
	int bottomTwo = topTwo + two->Height();

	// Check to see if any of the following are true- if they are, then there can't possibly be a collision
	if( leftOne > rightTwo || rightOne < leftTwo || topOne > bottomTwo || bottomOne < topTwo )
		return false;

	// None of the above are true, so there is a collision
	return true;
}

// Applies Images to the screen
void Master::Show( SDL_Surface* destination )
{
	// Show Bullets (must show before ship so they don't appear over front of ship), Asteroids, EnemyBullets, EnemyShips
	for( unsigned int i = 0; i < UserBullets.size(); i++ )
		UserBullets[i]->Show( BulletImage, ParticleImage, destination );
	for( unsigned int i = 0; i < Asteroids.size(); i++ )
		Asteroids[i]->Show( AsteroidImage, ParticleImage, destination );
	for( unsigned int i = 0; i < EnemyBullets.size(); i++ )
		EnemyBullets[i]->Show( EnemyBulletImage, ParticleImage, destination );
	for( unsigned int i = 0; i < EnemyShips.size(); i++ )
		EnemyShips[i]->Show( EnemyShipImage, ParticleImage, destination );

	// Show UserShip
	Ship->Show( ShipImage, ParticleImage, destination );

	// Show Score
	stringstream score;
	score << "Score " << Score;
	SDL_Surface* scoreImage = TTF_RenderText_Solid( Font, score.str().c_str(), FontColor );
	applySurface( SCORE_X_POSITION, SCORE_Y_POSITION, scoreImage, destination );

	// Show HighScore
	stringstream highScore;
	highScore << "HighScore " << HighScore;
	SDL_Surface* highScoreImage = TTF_RenderText_Solid( Font, highScore.str().c_str(), FontColor );
	applySurface( HIGHSCORE_X_POSITION, SCORE_Y_POSITION, highScoreImage, destination );

	// Game Over - Show gameOver/reset/highscore messages
	if( Ship->Dead )
	{
		SDL_Surface* gameOver = TTF_RenderText_Solid( Font, "GAME OVER", FontColor );
		SDL_Surface* reset = TTF_RenderText_Solid( Font, "Press 'R' to Restart", FontColor );
		applySurface( 265, 200, gameOver, destination );
		applySurface( 200, 240, reset, destination );

		// Check for new HighScore
		if( Score >= HighScore )
		{
			// Set HighScore
			HighScore = Score;
			stringstream newHighScore;
			newHighScore << "New HighScore! You earned " << HighScore << " points!";
			SDL_Surface* newHighScoreImage = TTF_RenderText_Solid( Font, newHighScore.str().c_str(), FontColor );
			applySurface( 90, 280, newHighScoreImage, destination );
		}
	}
}

// Shoots Bullets
void Master::UserShoot()
{
	if( !Ship->Dead )
	{
		// Bullet x position will be midpoint of ship, y will be at top of ship
		UserBullets.push_back( new Bullet( Ship->GetX() + 26, Ship->GetY() ) );
	}
}

// Shoots EnemyShips' Bullets
void Master::EnemyShoot()
{
	for( unsigned int i = 0; i < EnemyShips.size(); i++ )
	{
		if( !EnemyShips[i]->Dead )
		{
			// if ShootTimer in EnemyShip has ran for ENEMY_SHOT_FREQ milliseconds
			if( EnemyShips[i]->ShootTimer.GetTicks() > ENEMY_SHOT_FREQ )
			{
				// Add Bullet to EnemyBullets at midpoint of front of EnemyShip and reset ShootTimer
				// Bullet takes optional parameter for bulletVelocity, must fill with positive number so EnemyBullets travel downward
				EnemyBullets.push_back( new Bullet( EnemyShips[i]->GetX() + 11, EnemyShips[i]->GetY() + 18, ENEMY_BULLET_VELOCITY ) );
				EnemyShips[i]->ShootTimer.Start();
			}
		}
	}
}

// Spawns enemies
void Master::Spawn()
{
	// if SPAWN_RATE milliseconds have passed
	if( AsteroidSpawn.GetTicks() > ASTEROID_SPAWN_RATE )
	{
		// Create new Asteroid and restart Spawn timer
		Asteroids.push_back( new Asteroid() );
		AsteroidSpawn.Start();
	}

	if( EnemyShipSpawn.GetTicks() > ENEMY_SHIP_SPAWN_RATE )
	{
		// Create new EnemyShip and restart Spawn timer
		EnemyShips.push_back( new EnemyShip( Ship ) );
		EnemyShipSpawn.Start();
	}
}

// Resets game if user wishes to play again
void Master::Reset()
{
	// Set Ship to alive
	Ship->Dead = false;

	// Reset Score
	Score = 0;

	// Reset Ship position
	Ship->X = SCREEN_WIDTH/2 - Ship->Width()/2;
	Ship->Y = SCREEN_HEIGHT - Ship->Height();

	// Delete all Asteroids on screen
	for( unsigned int i = 0; i < Asteroids.size(); i++ )
	{
		delete Asteroids[i];
		vector<Asteroid*>::iterator iter = Asteroids.begin();
		iter += i;
		Asteroids.erase( iter );
		i--;
	}

	// Delete all EnemyShips on screen
	for( unsigned int i = 0; i < EnemyShips.size(); i++ )
	{
		delete EnemyShips[i];
		vector<EnemyShip*>::iterator iter = EnemyShips.begin();
		iter += i;
		EnemyShips.erase( iter );
		i--;
	}

	// Delete all EnemyBullets on screen
	for( unsigned int i = 0; i < EnemyBullets.size(); i++ )
	{
		delete EnemyBullets[i];
		vector<Bullet*>::iterator iter = EnemyBullets.begin();
		iter += i;
		EnemyBullets.erase( iter );
		i--;
	}
}

// Save HighScore
void Master::Save()
{
	// Save HighScore
	ofstream gameSave( "Save.txt" );
	gameSave << HighScore;
	gameSave.close();
}

// 