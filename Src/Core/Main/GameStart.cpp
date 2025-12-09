#include "../../Application.h"
#include "../../Manager/Common/ResourceManager.h"
#include "../../Manager/Common/SceneManager.h"
#include "../../Manager/Common/SoundManager.h"
#include "../../Utility/UtilityCommon.h"
#include "../Common/Timer.h"
#include "GameStart.h"

namespace
{
	static constexpr int FIRST_POS_Y = Application::SCREEN_SIZE_Y + 200;
}

GameStart::GameStart()
{
	state_ = STATE::MOVE;
	changeStateMap_.emplace(STATE::MOVE, std::bind(&GameStart::ChangeStateMove, this));
	changeStateMap_.emplace(STATE::EXIT, std::bind(&GameStart::ChangeStateExit, this));
	changeStateMap_.emplace(STATE::WAIT, std::bind(&GameStart::ChangeStateWait, this));
}

GameStart::~GameStart()
{
}

void GameStart::Init()
{
	start_.handleId = resMng_.GetHandle("start");
	start_.pos = { Application::SCREEN_HALF_X, FIRST_POS_Y };
	start_.scale = 1.5f;

	timer_ = std::make_unique<Timer>(0.5f);
	timer_->InitCountUp();

	isEnd_ = false;

	ChangeState(STATE::MOVE);
}

void GameStart::Update()
{
	update_();
}

void GameStart::Draw()
{
	start_.DrawRota();
}

void GameStart::UpdateMove()
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

	start_.pos.y = UtilityCommon::Lerp(FIRST_POS_Y, Application::SCREEN_HALF_Y, easeOutSpeed);
}

void GameStart::UpdateWait()
{
	if (timer_->CountUp())
	{
		ChangeState(STATE::EXIT);
	}
}

void GameStart::UpdateExit()
{
	constexpr int MOVE_X = 30;
	constexpr int MOVE_Y = 20;
	constexpr int LIMIT_X = Application::SCREEN_SIZE_X + 200;
	constexpr int LIMIT_Y = -128;
	constexpr float ANGLE = 0.2f;

	start_.pos.x += MOVE_X;
	start_.pos.y -= MOVE_Y;
	start_.angle += ANGLE;

	if (start_.pos.x > LIMIT_X && start_.pos.y < 0)
	{
		isEnd_ = true;
	}
}

void GameStart::ChangeState(const STATE state)
{
	state_ = state;

	changeStateMap_[state]();
}

void GameStart::ChangeStateMove()
{
	update_ = std::bind(&GameStart::UpdateMove, this);
}

void GameStart::ChangeStateWait()
{
	update_ = std::bind(&GameStart::UpdateWait, this);
}

void GameStart::ChangeStateExit()
{
	update_ = std::bind(&GameStart::UpdateExit, this);
}
