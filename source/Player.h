/*
 *      Player.h
 *      
 *      Copyright 2009 Stephen Eisenhauer <mail@stepheneisenhauer.com>
 * 
 * 		Class header for Player objects.
 */

#include "SmokeTrail.h"

#ifndef PLAYER_H
#define PLAYER_H

#define MAX_VELOCITY 5
#define GRAVITATION .2
#define THRUST (2.5 * GRAVITATION)

class Player {

public:

	// Constructors and destructor
	Player();		// default constructor
	~Player();		// destructor

	// Initializer
	void spawn();

	// Accessors
	float getX() { return x_coord; }
	float getY() { return y_coord; }
	float getWidth() { return width; }
	float getHeight() { return height; }

	// Tests
	// bool CollidesWith(const Rectangle * rect, f32 x=0, f32 y=0);

	// Operations
	void thrust();	// Accelerate the player up
	void stop();	// Stop the player
	void update();	// Recalculate position
	void Draw();	// Draw the player to the screen

protected:

	float x_coord;
	float y_coord;	// Vertical position
	float y_veloc;	// Vertical velocity
	float x_rot;	// x Rotation
	float y_rot;	// y Rotation
	float z_rot;	// z Rotation
	float width;
	float height;
	float piece_offset;	// So pieces separate upon death

	// Drawing objects
	//Image heliImage;
	//Sprite heliSprite;
	
	//Image fireImageSheet;
	//Sprite fireSprite;
	
	//Image smokeImage;
	//Sprite smokeSprites[5];
	
	SmokeTrail theSmokeTrail;

};

#endif
