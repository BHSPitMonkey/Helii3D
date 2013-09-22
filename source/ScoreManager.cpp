// ScoreManager.cpp

#include "ScoreManager.h"

extern GRRLIB_ttfFont *scoreFont;

ScoreManager::ScoreManager()
{
	for (int i=0; i < NUM_HIGH_SCORES; i++)
		scores[i] = 0;
}

unsigned int * ScoreManager::getScores()
{
	return &scores[0];
}

// Tries to add score to high scores list.
// Returns true if score qualified for the list.
bool ScoreManager::submitScore(unsigned int score)
{
	for (int i=0; i < NUM_HIGH_SCORES; i++)
	{
		if (score > scores[i])
		{
			// If we're on any index but the last one
			if (i != NUM_HIGH_SCORES-1)
			{
				// Bump scores down
				for (int j=NUM_HIGH_SCORES-2; j>=i; j--)
				{
					scores[j+1] = scores[j];
				}
			}
			scores[i] = score;
			return true;
		}
	}

	return false;
}

void ScoreManager::draw(f32 x, f32 y)
{
	// Print the scores
	GRRLIB_PrintfTTF(x, y, scoreFont, "High Scores", 22, 0xFFFFFFFF);
	for (int i=0; i < NUM_HIGH_SCORES; i++)
	{
		sprintf(&scoreString[0], "%d", scores[i]);
		GRRLIB_PrintfTTF(x, y+36+(i*30), scoreFont, &scoreString[0], 18, 0xFFFFFFFF);
	}
}
