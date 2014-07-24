// Timer.h - Timer class declaration
// Written by Ryan Geiger

#pragma once

class Timer
{
private:
	//The clock time when the timer started
	int StartTicks;

	//The ticks stored when the timer was paused
	int PausedTicks;

	//The timer status
	bool Paused;
	bool Started;

public:
	//Initializes variables
	Timer();

	//The various clock actions
	void Start();
	void Stop();
	void Pause();
	void Unpause();

	//Gets the timer's time
	int GetTicks() const;

	//Checks the status of the timer
	bool IsStarted() const;
	bool IsPaused() const;    
};

// 18