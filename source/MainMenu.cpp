// MainMenu.cpp

#include <grrlib.h>
#include "MainMenu.h"
#include "CaveManager.h"
#include "title_png.h"

MainMenu::MainMenu(ScoreManager * scoreMgr)
{
	_scoreMgr = scoreMgr;
	y_pos = 100;
	reset();
	
	tex_title = GRRLIB_LoadTexture(title_png);
}
MainMenu::MainMenu()
{
	MainMenu(new ScoreManager());
}
void MainMenu::reset()
{
	visible = true;
	x_pos = 180;
}
void MainMenu::update()
{
	if (visible)
	{
		if (x_pos < -640)
			visible = false;
		else
			x_pos -= CAVE_SPEED;
	}
}
void MainMenu::draw()
{
	if (visible)
	{
		// Draw title graphic
		GRRLIB_DrawImg(x_pos, y_pos, tex_title, 0, 1, 1, 0xFFFFFFFF); // Draw a bitmap
		
		// Draw score manager
		_scoreMgr->draw(x_pos + 240, y_pos+70);
	}
}
