#include "Score.h"

Score::Score()
{
	playerScoreMap_.clear();
}

Score::~Score()
{
}

void Score::Init()
{
	playerScoreMap_.clear();
}

void Score::SetScore(const int playerNo, const int score)
{
	if (playerScoreMap_.count(playerNo) == 0)
	{
		playerScoreMap_.emplace(playerNo, score);
	}
	else
	{
		playerScoreMap_[playerNo] += score;
	}
}