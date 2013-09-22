// MainMenu.h

#ifndef MAINMENU_H
#define MAINMENU_H

#include "ScoreManager.h"

class MainMenu
{
	private:
		ScoreManager * _scoreMgr;
		float x_pos;
		float y_pos;
		bool visible;
		GRRLIB_texImg * tex_title;

	public:
		MainMenu(ScoreManager * scoreMgr);
		MainMenu();
		void reset();
		void update();
		void draw();
};

#endif
