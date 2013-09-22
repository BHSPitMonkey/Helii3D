/*
 *      main.cpp
 * 
 * 		Helii
 * 		A helicopter flying game for the Wii.
 *      
 *      Copyright 2009 Stephen Eisenhauer <mail@stepheneisenhauer.com>
 * 
 * 		This is the main source file for Helii.
 * 
 * 		More details should be in README.
 */

#include <grrlib.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <wiiuse/wpad.h>
#include <fat.h>
#include <iostream>
#include <fstream>
#include "globals.h"
#include "Player.h"
#include "CaveManager.h"
#include "ScoreManager.h"
#include "MainMenu.h"

// Font
#include "ka1_ttf.h"

// #define DEBUGMODE

// Global variables
unsigned int g_ticks = 0;			// Counts how many frames we've drawn, period
unsigned int g_gameTime;			// Counts how many frames have passed in current game
unsigned int g_currentStateTime;	// Counts how many frames have passed in current state
GameState g_currentState;			// Decides what state or mode the game is in
Player thePlayer;
CaveManager theCave;
ScoreManager scoreMgr;
MainMenu mainMenu(&scoreMgr);
std::ofstream logFile;
char scoreString[20];
GRRLIB_ttfFont * scoreFont;

void setGameState(GameState newState)
{
	// First things first
	g_currentState = newState;

	// Reset the timer
	g_currentStateTime = 0;

	// Do certain things when these changes happen
	switch (newState) {

		// When changing to the Main Menu state
		case MAIN_MENU_STATE:
		
			// Respawn the player and the cave
			thePlayer.spawn();
			theCave.resetAll();
			mainMenu.reset();

			// Update the player and the cave once, just to get their starting appearances right.
			theCave.update();		// Update the position of the cave
			thePlayer.update();		// Reposition the player

		break;

		// When changing to the Flying state
		case FLYING_STATE:

			g_gameTime = 0;

		break;

		// When changing to the Dead state (does nothing for now)
		case DEAD_STATE:
		
			thePlayer.stop();
			bool ranked = scoreMgr.submitScore(g_gameTime/10);
			// TODO: Do something if player made high scores
		
		break;
	}
}

int main(int argc, char **argv)
{
	GRRLIB_Init();

	//GRRLIB_texImg *tex_font = GRRLIB_LoadTexture(font);
	//GRRLIB_InitTileSet(tex_font, 16, 16, 32);
	
    // Load the font from memory
    scoreFont = GRRLIB_LoadTTF(ka1_ttf, ka1_ttf_size);

	GRRLIB_Settings.antialias = true;

	GRRLIB_SetBackgroundColour(0x00, 0x00, 0x00, 0xFF);
	GRRLIB_Camera3dSettings(0.0f,3.0f,5.0f, 0,1,0, 0,-0.4,0);
	

	// Initialise Wiimote
	WPAD_Init();

	// Enable SD card stuff
	fatInitDefault();
	
	// Logging
#ifdef DEBUGMODE
		logFile.open ("/log.txt");
		logFile << "Opening log for Wiings.\n";
#endif

	// Go into the main menu state
	setGameState(MAIN_MENU_STATE);

	bool paused = false;

	// Camera setup
	GRRLIB_Camera3dSettings(320, 240, 600, // position
							0,1,0,
							320, 240, 0 // look at
							);

	// This is the game's run loop. It is executed several times per second.
	while(1)
	{
		// Flat sky
		GRRLIB_2dMode();
		GRRLIB_Rectangle(0, 0, 640, 480, 0x201700FF, 1);
		// Main menu stuff
		if (g_currentState == FLYING_STATE && !paused)
			mainMenu.update();
		mainMenu.draw();

		// Set up 3D and lighting
        GRRLIB_3dMode(0.1,1000,45,0,1);
        GRRLIB_SetLightAmbient(0x111111FF);
        GRRLIB_SetLightDiff(0,(guVector){320.0f,240.0f,300.0f},20.0f,1.0f,0xCCCCCCFF);
		
		/* ============================
		 ====== INPUT PROCESSING ======
		 ============================ */
		 
		// Look for new Wii controllers
		WPAD_ScanPads();
		
		// Quit the game (break out of the runloop) if HOME is pressed
		if(WPAD_ButtonsDown(WPAD_CHAN_0)&WPAD_BUTTON_HOME)
			break;

		// Do different things depending on the game state.
		switch (g_currentState)
		{
			
			case MAIN_MENU_STATE:

				// When A is pressed, start the game's flying mode
				if(WPAD_ButtonsDown(WPAD_CHAN_0)&WPAD_BUTTON_A)
					setGameState(FLYING_STATE);
				
				thePlayer.update();		// Reposition the player	
				GRRLIB_SetLightDiff(1,(guVector){thePlayer.getX(),thePlayer.getY(),0},20.0f,1.0f,0xCCCCCCFF);

				// Draw everything
				theCave.Draw();			// Draw the cave
				thePlayer.Draw();		// Draw the player
				
				// Print the score
				sprintf(&scoreString[0], "Score: %d", g_gameTime/10);
				GRRLIB_2dMode();
				GRRLIB_PrintfTTF(20, 20, scoreFont, &scoreString[0], 18, 0xFFFFFFFF);
				
				// TODO: Draw some menu text on screen
			
			break;
			
			case FLYING_STATE:

				// When A is held (and game is not paused), fly the plane.
				if(WPAD_ButtonsHeld(WPAD_CHAN_0)&WPAD_BUTTON_A && !paused)
					thePlayer.thrust();
				// When + is pressed, pause or unpause.
				if(WPAD_ButtonsDown(WPAD_CHAN_0)&WPAD_BUTTON_PLUS)
					paused = !paused;

				// If the player touches the cave, you die.
				if (theCave.CollidesWithPlayer()) {
					setGameState(DEAD_STATE);
				}

				// Move everything (unless paused)
				if (!paused) {
					theCave.update();		// Update the position of the cave
					thePlayer.update();		// Reposition the player
					g_gameTime++;			// Increment the game timer
				}
				
				GRRLIB_SetLightDiff(1,(guVector){thePlayer.getX(),thePlayer.getY(),0},20.0f,1.0f,0xCCCCCCFF);

				// Draw everything
				theCave.Draw();			// Draw the cave
				thePlayer.Draw();		// Draw the player

				// Print the score
				sprintf(&scoreString[0], "Score: %d", g_gameTime/10);
				GRRLIB_2dMode();
				GRRLIB_PrintfTTF(20, 20, scoreFont, &scoreString[0], 18, 0xFFFFFFFF);

				

			break;
			
			case DEAD_STATE:

				// When + is pressed, reset back to main menu
				if(WPAD_ButtonsDown(WPAD_CHAN_0)&WPAD_BUTTON_PLUS) {
					setGameState(MAIN_MENU_STATE);
				}
				
				thePlayer.update();
				
				// Draw everything
				theCave.Draw();			// Draw the cave
				thePlayer.Draw();		// Draw the player
				
				// Print the score
				sprintf(&scoreString[0], "Score: %d", g_gameTime/10);
				GRRLIB_2dMode();
				GRRLIB_PrintfTTF(20, 20, scoreFont, &scoreString[0], 18, 0xFFFFFFFF);
			
			break;			
		}

		// Draw the buffer to the screen
		GRRLIB_Render();

		// Increase our count of how many frames have been drawn.
		g_ticks++;
		g_currentStateTime++;
	}
	
	// This seems to help not crash upon exit...
	GRRLIB_2dMode();
	GRRLIB_Rectangle(0, 0, 640, 480, 0x000000FF, 1);
	GRRLIB_Render();
	
	/*

	// Closing game animation loop (or, Fancy Outro Sequence!)
	int closingTicks = 0;
	u32 color = 0x201700FF;
	u32 colorstep = color / 30;
	while (closingTicks < 180)
	{
		GRRLIB_2dMode();
		GRRLIB_Rectangle(0, 0, 640, 480, color, 1);

		// Draw/manipulate player
		if (g_currentState != DEAD_STATE)
			thePlayer.thrust();				// Lift the player
		thePlayer.Move();				// Move the player
		if (thePlayer.getY() < -100)	// If the player flies off-screen
			break;						// 		we're done.
		thePlayer.Draw();				// Draw the player
		
		// Fade
		if (color > colorstep)
			color -= colorstep;
		
		GRRLIB_Render();
		closingTicks++;
	}
	*/

    GRRLIB_FreeTTF(scoreFont);
    GRRLIB_Exit(); // Be a good boy, clear the memory allocated by GRRLIB

    exit(0);  // Use exit() to exit a program, do not use 'return' from main()
}
