// ScoreManager.h

#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H

#include <grrlib.h>

#define NUM_HIGH_SCORES 5

class ScoreManager
{
	private:
		unsigned int scores[NUM_HIGH_SCORES];

	public:
		ScoreManager();
		unsigned int * getScores();
		// Tries to add score to high scores list.
		// Returns true if score qualified for the list.
		bool submitScore(unsigned int score);
		void draw(f32 x, f32 y);
		char scoreString[20];
};

#endif
