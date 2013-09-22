/*
 *      SmokeTrail.h
 *      
 *      Copyright 2009 Stephen Eisenhauer <mail@stepheneisenhauer.com>
 * 
 * 		Class header for SmokeTrail objects.
 */

#ifndef SMOKETRL_H
#define SMOKETRL_H

#define NUM_CLOUDS 5
#define CLOUD_WIDTH 10.0

class SmokeTrail
{
	public:

		// Constructors and destructor
		SmokeTrail();						// default constructor
		~SmokeTrail();						// destructor

		// Initializer
		void resetAll();		// All columns to starting positions!

		// Operations
		void update();	// Recalculate positions
		void Draw();	// Draw the columns to the screen

	protected:

		// Drawing objects
		float cloud_x[NUM_CLOUDS];		
		float cloud_y[NUM_CLOUDS];		
		float cloud_size[NUM_CLOUDS];		
		// float cloud_ang[NUM_CLOUDS];		
		bool cloud_visible[NUM_CLOUDS];		
		u32 cloud_alpha[NUM_CLOUDS];

		unsigned short int first;
};

#endif
