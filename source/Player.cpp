/*
 *      Player.cpp
 *      
 *      Copyright 2009 Stephen Eisenhauer <mail@stepheneisenhauer.com>
 * 
 * 		Class for Player objects.
 */

#include <grrlib.h>
#include <math.h>
#include "Player.h"
//#include "heli.h"
//#include "fire.h"
#include "globals.h"

// Default constructor
Player::Player() { 

	// Set up the sprite and texture
	//heliImage.LoadImage(heli, IMG_LOAD_TYPE_BUFFER);
	//heliSprite.SetImage(&heliImage);
	//heliSprite.SetX(120);
	//heliSprite.DefineCollisionRectangle(3, 17, 47, 29);

	// Set up the sprite and texture
	//fireImageSheet.LoadImage(fire, IMG_LOAD_TYPE_BUFFER);
	//fireSprite.SetImage(&fireImageSheet, 64, 64);
	//fireSprite.SetX(120);

	// Set default physical state
	x_coord = 120;
	width = 20;
	height = 20;
	piece_offset = 0;
	spawn();
}

// Destructor
Player::~Player()
{
}

// Initializer
void Player::spawn() {

	// Reset the smoke trail
	theSmokeTrail.resetAll();

	// Starting position for a Player
	y_coord = 240;

	// Reset Player velocity to 0
	y_veloc = -50;
	
	x_rot = 0;
	y_rot = 0;
	z_rot = 0;
}

// Control the player
void Player::thrust() {
	y_veloc += THRUST;		// Remember, the y- direction is up.
}

// Stop y velocity
void Player::stop()
{
	y_veloc = 0;
}

// Reposition the player for a new frame
void Player::update()
{
	switch (g_currentState)
	{
		case DEAD_STATE:
			x_rot += 3;
			y_rot += 7;
			z_rot += 5;
			piece_offset = 10;
			
			y_veloc -= GRAVITATION;

			y_coord += y_veloc;		// Velocity influences position
			break;

		case MAIN_MENU_STATE:
			y_coord = 240 + cos(g_ticks / 10.0);
			piece_offset = 0;
			break;

		default:
			z_rot = 2 * y_veloc;
			piece_offset = 0;
			
			y_veloc -= GRAVITATION;		// Gravity influences velocity

			// Keep the velocity within bounds
			if (y_veloc > MAX_VELOCITY)
				y_veloc = MAX_VELOCITY;
			else if (y_veloc < -MAX_VELOCITY)
				y_veloc = -MAX_VELOCITY;

			y_coord += y_veloc;		// Velocity influences position
	}
	
	theSmokeTrail.update();	// Update the smoke trail
}

// Draw the player
void Player::Draw()
{
	// Draw the smoke trail
	theSmokeTrail.Draw();

	// Draw the player
	/*
	GRRLIB_ObjectViewBegin();
	GRRLIB_ObjectViewRotate(0, 0, y_veloc);	// cute tilting effect
	GRRLIB_ObjectViewScale(width,height,width);
	GRRLIB_ObjectViewTrans(x_coord, y_coord, 0);
	GRRLIB_ObjectViewEnd();
	GRRLIB_DrawCube(1,1,0xFF000033); */

	// Cockpit
	GRRLIB_ObjectViewBegin();
		GRRLIB_ObjectViewRotate(x_rot, y_rot, z_rot);
		GRRLIB_ObjectViewScale(width,height,width);
		GRRLIB_ObjectViewTrans(x_coord, y_coord, 0);
	GRRLIB_ObjectViewEnd();
	GRRLIB_DrawSphere(0.5, 9, 9, 1, 0xFF0000FF);

	// Blades
	GRRLIB_ObjectViewBegin();
		GRRLIB_ObjectViewRotate(x_rot, y_rot, z_rot);
		GRRLIB_ObjectViewScale(width,height,width);
		GRRLIB_ObjectViewTrans(x_coord, y_coord+10, 0);
	GRRLIB_ObjectViewEnd();
	GRRLIB_DrawCylinder(0.8, .05, 9, 1, 0x999999FF);

	// Rear
	GRRLIB_ObjectViewBegin();
		GRRLIB_ObjectViewScale(width,height,width);
		GRRLIB_ObjectViewRotate(0, 0, 90+piece_offset);	// Rotate initial cylinder shape
		GRRLIB_ObjectViewTrans(-10-piece_offset, 0, piece_offset);	// Move to back of helicopter
		GRRLIB_ObjectViewRotate(x_rot, y_rot, z_rot);	// Rotate entire heli
		GRRLIB_ObjectViewTrans(x_coord, y_coord, 0);	// Move to heli position
	GRRLIB_ObjectViewEnd();
	GRRLIB_DrawCylinder(0.2, 0.8, 9, 1, 0xFF0000FF);

	// Rear Blades
	GRRLIB_ObjectViewBegin();
		GRRLIB_ObjectViewScale(width,height,width);
		GRRLIB_ObjectViewRotate(90, 0, 0);
		GRRLIB_ObjectViewTrans(-20-piece_offset, 0, 4+piece_offset);
		GRRLIB_ObjectViewRotate(x_rot, y_rot, z_rot);
		GRRLIB_ObjectViewTrans(x_coord, y_coord, 0);
	GRRLIB_ObjectViewEnd();
	GRRLIB_DrawCylinder(0.2, .05, 9, 1, 0x999999FF);

	// Skis
	GRRLIB_ObjectViewBegin();
		GRRLIB_ObjectViewScale(15,2,2);
		GRRLIB_ObjectViewRotate(x_rot, y_rot, z_rot);
		GRRLIB_ObjectViewTrans(x_coord+piece_offset, y_coord-10-piece_offset, 4+piece_offset);
	GRRLIB_ObjectViewEnd();
	GRRLIB_DrawCube(1, 1, 0x666666FF);
	GRRLIB_ObjectViewBegin();
		GRRLIB_ObjectViewScale(15,2,2);
		GRRLIB_ObjectViewRotate(x_rot, y_rot, z_rot);
		GRRLIB_ObjectViewTrans(x_coord-piece_offset, y_coord-10-piece_offset, -4-piece_offset);
	GRRLIB_ObjectViewEnd();
	GRRLIB_DrawCube(1, 1, 0x666666FF);

	// Place the sprite where the player is
	//heliSprite.SetY(y_coord);

	// Rotate the helicopter in accordance with its movement
	//heliSprite.SetRotation(y_veloc);

	// Draw the heli sprite
	//heliSprite.Draw();

	// Draw the fire, if DEAD_STATE.
	/*
	if (g_currentState == DEAD_STATE) {
		fireSprite.SetY(y_coord);
		if (g_ticks % 5 == 0)
			fireSprite.NextFrame();
		fireSprite.Draw();
	}
	*/
}
