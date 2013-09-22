/*
 *      SmokeTrail.h
 *      
 *      Copyright 2009 Stephen Eisenhauer <mail@stepheneisenhauer.com>
 * 
 * 		Class implementation for SmokeTrail objects.
 */

#include <grrlib.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include "CaveManager.h"
#include "Player.h"
#include "smoke.h"
#include "SmokeTrail.h"

extern std::ofstream logFile;
extern unsigned int g_gameTime;
extern Player thePlayer;

// Default constructor
SmokeTrail::SmokeTrail()
{
	resetAll();
	
	// Initially, first is the first index in the array.
	first = 0;
}

// Destructor
SmokeTrail::~SmokeTrail() { }

// Reset all clouds
void SmokeTrail::resetAll()
{
	float x = -MARGIN;

	for (int i=0; i<NUM_CLOUDS; i++)
	{
		cloud_x[i] = x + (((120.0+MARGIN)/NUM_CLOUDS) * i);
		cloud_y[i] = 240;
		cloud_visible[i] = false;	
		cloud_alpha[i] = 0x000000AA;
		cloud_size[i] = CLOUD_WIDTH;
	}
	
	first = 0;
}

// Shift and update the cave manager.
void SmokeTrail::update()
{
	// Push all clouds left and lower their opacity
	for (int i=0; i<NUM_CLOUDS; i++)
	{
		cloud_x[i] -= CAVE_SPEED;	// Move left
		cloud_alpha[i] *= 0.95;		// Fade alpha
		cloud_size[i] += 0.5;		// Increase size
	}

	// If the first cloud is too far off-screen, reuse it
	if (cloud_x[first] <= -MARGIN)
	{
		// Move the first cloud to where the player is
		cloud_x[first] = 110+10;
		cloud_y[first] = thePlayer.getY()+10;
		cloud_visible[first] = true;
		cloud_alpha[first] = 0x000000AA;	// Fully opaque
		cloud_size[first] = CLOUD_WIDTH;

		first = (first+1) % NUM_CLOUDS;	// Change the index of the first column
	}	
}

void SmokeTrail::Draw()
{
	// Draw all the clouds
	for (int i=0; i<NUM_CLOUDS; i++)
	{
		if (cloud_visible[i])
		{
			GRRLIB_ObjectViewBegin();
			//GRRLIB_ObjectViewRotate(0, 0, y_veloc);
			GRRLIB_ObjectViewScale(cloud_size[i], cloud_size[i], cloud_size[i]);
			GRRLIB_ObjectViewTrans(cloud_x[i], cloud_y[i], 0);
			GRRLIB_ObjectViewEnd();
			GRRLIB_DrawSphere(0.5, 5, 5, 1, 0x88888800 | cloud_alpha[i]);
		}
	}
}
