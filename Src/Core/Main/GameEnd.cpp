#include "../../Application.h"
#include "../../Manager/Common/ResourceManager.h"
#include "../../Manager/Common/SceneManager.h"
#include "../../Manager/Common/InputManager.h"
#include "../../Manager/Common/SoundManager.h"
#include "../../Utility/UtilityCommon.h"
#include "../Common/Timer.h"
#include "Score.h"
#include "GameEnd.h"

namespace
{
	static constexpr int FIRST_POS_Y = Application::SCREEN_SIZE_Y + 200;
}

GameEnd::GameEnd(const Score& score) : 
	score_(score)
{
	changeStateMap_.emplace(STATE::MOVE, std::bind(&GameEnd::ChangeStateMove, this));
	changeStateMap_.emplace(STATE::WAIT, std::bind(&GameEnd::ChangeStateWait, this));
	changeStateMap_.emplace(STATE::RESULT, std::bind(&GameEnd::ChangeStateResult, this));
}

GameEnd::~GameEnd()
{
}

void GameEnd::Init()
{
	finish_.handleId = resMng_.GetHandle("finish");
	finish_.pos = { Application::SCREEN_HALF_X, FIRST_POS_Y };
	finish_.scale = 1.5f;

	scoreMes_.handleId = resMng_.GetHandle("score");
	scoreMes_.pos = { 900, Application::SCREEN_HALF_Y + 50 };

	playerPlate_.handleIds = resMng_.GetHandles("playerPlates");
	playerPlate_.div = { 1,4 };
	playerPlate_.pos = { Application::SCREEN_HALF_X, 120 };
	playerPlate_.scale = 1.2f;

	numbers_.handleIds = resMng_.GetHandles("numbers");
	numbers_.scale = 2.0f; 

	message_.pos = { Application::SCREEN_HALF_X, 120 };
	message_.handleId = resMng_.GetHandle("draw");

	timer_ = std::make_unique<Timer>(0.5f);
	timer_->InitCountUp();

	moveStep_ = 0.0f;
	isEnd_ = false;
	isDraw_ = false;

	ChangeState(STATE::MOVE);
}

void GameEnd::Update()
{
	update_();
}

void GameEnd::Draw()
{
	draw_();
}

void GameEnd::UpdateMove()
{
	constexpr float MOVE_TIME = 1.0f;

	moveStep_ += scnMng_.GetDeltaTime();

	// 進行度の計算
	float t = moveStep_ / MOVE_TIME;

	// 進行度が終了値を超えている場合
	if (t >= 1.0f)
	{
		t = 1.0f;		// 値を固定
		ChangeState(STATE::WAIT);
	}

	// イーズアウトでスピードを決定
	float speed = 1.0f - t;

	// 3次間数で計算
	float easeOutSpeed = 1.0f - speed * speed * speed;

	finish_.pos.y = UtilityCommon::Lerp(FIRST_POS_Y, Application::SCREEN_HALF_Y, easeOutSpeed);
}

void GameEnd::UpdateWait()
{
	if (timer_->CountUp())
	{
		ChangeState(STATE::RESULT);
	}
}

void GameEnd::UpdateResult()
{
	if (InputManager::GetInstance().IsTrgDown(InputManager::TYPE::SELECT_DECISION))
	{
		isEnd_ = true;
	}
}

void GameEnd::ChangeState(const STATE state)
{
	state_ = state;

	changeStateMap_[state]();
}

void GameEnd::ChangeStateMove()
{
	update_ = std::bind(&GameEnd::UpdateMove, this);
	draw_ = std::bind(&GameEnd::DrawFinish, this);
}

void GameEnd::ChangeStateWait()
{
	update_ = std::bind(&GameEnd::UpdateWait, this);
	draw_ = std::bind(&GameEnd::DrawFinish, this);
}

void GameEnd::ChangeStateResult()
{
	update_ = std::bind(&GameEnd::UpdateResult, this);
	draw_ = std::bind(&GameEnd::DrawResult, this);

	// 表示スコアを計算
	int index = 0;
	int score = 0;
	displayScore_ = -1;
	for (const auto& s : score_.GetScoreMap())
	{
		score = s.second;

		// 引き分けの場合
		if (displayScore_ == score)
		{
			isDraw_ = true;

			// 終了
			break;
		}

		// 現在のスコアが新しいスコアより大きい場合
		else if (displayScore_ < score)
		{
			displayScore_ = score;
			playerPlate_.index = index;
		}

		index++;
	}

	// 表示スコアが負の数の場合
	if (0 > displayScore_)
	{
		displayScore_ = 0;
	}
	//桁数
	numberDigit_ = UtilityCommon::GetDigitCount(displayScore_);

	sndMng_.PlaySe(SoundType::SE::APPLAUSE);
}

void GameEnd::DrawFinish()
{
	finish_.DrawRota();
}

void GameEnd::DrawResult()
{
	constexpr int NUMBER_OFFSET_X = 200;
	constexpr int POS_Y = Application::SCREEN_HALF_Y;
	constexpr int MAX_DISITS = 3;
	constexpr int OFFSET_POS_X = 200;
	const int NUMBER_POS_X = numberDigit_ <= 1 ? Application::SCREEN_HALF_X : Application::SCREEN_HALF_X - OFFSET_POS_X;

	playerPlate_.DrawRota();

	if (isDraw_) { message_.DrawRota(); }

	//数字の描画
	for (int i = 0; i < numberDigit_; i++)
	{
		int index = UtilityCommon::GetDigit(displayScore_, numberDigit_ - 1 - i);
		numbers_.index = index;
		numbers_.pos = { NUMBER_POS_X + NUMBER_OFFSET_X * i,POS_Y, };
		numbers_.DrawRota();
	}

	scoreMes_.DrawRota();
}