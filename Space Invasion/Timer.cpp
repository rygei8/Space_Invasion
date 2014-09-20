// Timer.cpp - Timer class implementation
// Written by Ryan Geiger

#pragma once
#include "Timer.h"
#include "SDL.h"

Timer::Timer()
{
    //Initialize the variables
    StartTicks = 0;
    PausedTicks = 0;
    Paused = false;
    Started = false;    
}

void Timer::Start()
{
    //Start the timer
    Started = true;
    
    //Unpause the timer
    Paused = false;
    
    //Get the current clock time
    StartTicks = SDL_GetTicks();    
}

void Timer::Stop()
{
    //Stop the timer
    Started = false;
    
    //Unpause the timer
    Paused = false;    
}

int Timer::GetTicks() const
{
    //If the timer is running
    if( Started == true )
    {
        //If the timer is Paused
        if( Paused == true )
        {
            //Return the number of ticks when the timer was Paused
            return PausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            return SDL_GetTicks() - StartTicks;
        }    
    }
    
    //If the timer isn't running
    return 0;    
}

void Timer::Pause()
{
    //If the timer is running and isn't already Paused
    if( ( Started == true ) && ( Paused == false ) )
    {
        //Pause the timer
        Paused = true;
    
        //Calculate the Paused ticks
        PausedTicks = SDL_GetTicks() - StartTicks;
    }
}

void Timer::Unpause()
{
    //If the timer is Paused
    if( Paused == true )
    {
        //Unpause the timer
        Paused = false;
    
        //Reset the starting ticks
        StartTicks = SDL_GetTicks() - PausedTicks;
        
        //Reset the Paused ticks
        PausedTicks = 0;
    }
}

bool Timer::IsStarted() const
{
    return Started;    
}

bool Timer::IsPaused() const
{
    return Paused;    
}

// 61