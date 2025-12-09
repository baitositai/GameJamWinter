#include "../../Utility/UtilityCommon.h"
#include "../../Manager/Common/FontManager.h"
#include "../../Manager/Common/ResourceManager.h"
#include "Score.h"

Score::Score()
{
	scores_.clear();
}

Score::~Score()
{
}

void Score::Init()
{
	scores_.clear();
	texts_.clear();

	font_ = fontMng_.CreateMyFont(resMng_.GetFontName("fontHanazome"), FONT_SIZE, FONT_THINESS);
}

void Score::Draw()
{
	for (auto& text : texts_)
	{
		text.Draw();
	}
}

void Score::SetScore(const int playerNo, const int score)
{
	if (scores_.size() > playerNo)
	{
		scores_[playerNo] += score;
		texts_[playerNo].data2 += score;
	}
}

void Score::SetPlayerNum(const int playerNum)
{
	scores_.clear();
	texts_.clear();

	scores_.resize(playerNum, 0);
	texts_.resize(playerNum, CharacterString());

	for (int i = 0; i < playerNum; i++)
	{
		texts_[i].data1 = i + 1;
		texts_[i].data2 = 0;
		texts_[i].string = L"PLAYER%d‚Í%d“_";
		texts_[i].color = UtilityCommon::BLACK;
		texts_[i].pos = { 0, i * FONT_SIZE + 10 };
		texts_[i].fontHandle = font_;
	}
}
