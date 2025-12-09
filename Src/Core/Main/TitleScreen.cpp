#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/Common/SceneManager.h"
#include "../../Manager/Common/ResourceManager.h"
#include "../../Manager/Common/InputManager.h"
#include "../../Manager/Common/SoundManager.h"
#include "../../Utility/UtilityCommon.h"
#include "TitleScreen.h"

TitleScreen::TitleScreen() :
	 inputMng_(InputManager::GetInstance())
{
	changeStateMap_.emplace(STATE::MAIN, std::bind(&TitleScreen::ChangeStateMain, this));
	changeStateMap_.emplace(STATE::UI_MOVE, std::bind(&TitleScreen::ChangeStateUiMove, this));
	changeStateMap_.emplace(STATE::NUM_SELECT, std::bind(&TitleScreen::ChangeStateNumSelect, this));
}

TitleScreen::~TitleScreen()
{
}

void TitleScreen::Init()
{
	auto& res = ResourceManager::GetInstance();
	imgLogo_.handleId = res.GetHandle("titleLogo");
	imgLogo_.pos = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y };
	imgLogo_.scale = 0.35f;

	imgPush_.handleId = res.GetHandle("pushSpace");
	imgPush_.pos = { Application::SCREEN_HALF_X, 550 };

	imgSelect_.handleId = res.GetHandle("selectPlayerNum");
	imgSelect_.pos = { Application::SCREEN_SIZE_X + Application::SCREEN_HALF_X, 120 };

	sprNumbers_.handleIds = res.GetHandles("numbers");
	sprNumbers_.index = 1;
	sprNumbers_.scale = 2.0f;
	sprNumbers_.div = { 5, 2 };
	sprNumbers_.pos = { Application::SCREEN_SIZE_X + Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y };

	isEnd_ = false;
	alpha_ = UtilityCommon::ALPHA_MAX;
	isRev_ = -1;
	step_ = 0.0f;
	playerNum_ = 1;

	ChangeState(STATE::MAIN);
}

void TitleScreen::Update()
{
	update_();
}

void TitleScreen::Draw()
{
	// ロゴ画像
	imgLogo_.DrawRota();
	
	// プッシュ画像
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alpha_);
	imgPush_.DrawRota();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 選択描画
	imgSelect_.DrawRota();

	// 数字画像
	sprNumbers_.DrawRota();
}

void TitleScreen::ChangeState(const STATE state)
{
	state_ = state;

	changeStateMap_[state_]();
}

void TitleScreen::ChangeStateMain()
{
	update_ = std::bind(&TitleScreen::UpdateMainScreen, this);
}

void TitleScreen::ChangeStateUiMove()
{
	update_ = std::bind(&TitleScreen::UpdateUiMove, this);	
}

void TitleScreen::ChangeStateNumSelect()
{
	update_ = std::bind(&TitleScreen::UpdateNumSelect, this);
}

void TitleScreen::UpdateMainScreen()
{
	constexpr float ALPHA_SPEED = 10.0f;

	// アルファ値の更新
	alpha_ += ALPHA_SPEED * isRev_;

	if (alpha_ > UtilityCommon::ALPHA_MAX)
	{
		alpha_ = UtilityCommon::ALPHA_MAX;
		isRev_ = -1;
	}
	else if (alpha_ < 0)
	{
		alpha_ = 0;
		isRev_ = 1;
	}
	
	if (inputMng_.IsTrgDown(InputManager::TYPE::GAME_STATE_CHANGE))
	{
		sndMng_.PlaySe(SoundType::SE::DECISION);
		ChangeState(STATE::UI_MOVE);
	}
}

void TitleScreen::UpdateUiMove()
{
	constexpr float MOVE_TIME = 1.0f;

	step_ += scnMng_.GetDeltaTime();

	// 進行度の計算
	float t = step_ / MOVE_TIME;

	// 進行度が終了値を超えている場合
	if (t >= 1.0f)
	{
		t = 1.0f;		// 値を固定
	}

	// イーズアウトでスピードを決定
	float speed = 1.0f - t;

	// 3次間数で計算
	float easeOutSpeed = 1.0f - speed * speed * speed;

	imgLogo_.pos.x = UtilityCommon::Lerp(Application::SCREEN_HALF_X, -Application::SCREEN_HALF_X, easeOutSpeed);
	imgPush_.pos.x = UtilityCommon::Lerp(Application::SCREEN_HALF_X, -Application::SCREEN_HALF_X, easeOutSpeed);
	imgSelect_.pos.x = UtilityCommon::Lerp(Application::SCREEN_HALF_X + Application::SCREEN_SIZE_X, Application::SCREEN_HALF_X, easeOutSpeed);
	sprNumbers_.pos.x = UtilityCommon::Lerp(Application::SCREEN_HALF_X + Application::SCREEN_SIZE_X, Application::SCREEN_HALF_X, easeOutSpeed);

	if (step_ > MOVE_TIME)
	{
		ChangeState(STATE::NUM_SELECT);
	}
}

void TitleScreen::UpdateNumSelect()
{
	constexpr int LIST_MAX = 5;

	if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_RIGHT))
	{
		sndMng_.PlaySe(SoundType::SE::SELECT);
		playerNum_ = UtilityCommon::WrapStepIndex(playerNum_, 1, 1, LIST_MAX);
	}
	else if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_LEFT))
	{
		sndMng_.PlaySe(SoundType::SE::SELECT);
		playerNum_ = UtilityCommon::WrapStepIndex(playerNum_, -1, 1, LIST_MAX);
	}
	else if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_DECISION))
	{
		sndMng_.PlaySe(SoundType::SE::DECISION);
		isEnd_ = true;
	}

	// インデックスの反映
	sprNumbers_.index = playerNum_;
}