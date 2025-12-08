#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/Common/ResourceManager.h"
#include "TitleScreen.h"

TitleScreen::TitleScreen()
{
}

TitleScreen::~TitleScreen()
{
}

void TitleScreen::Init()
{
	auto& res = ResourceManager::GetInstance();
	imgLogo_.handleId = res.GetHandle("titleLogo");
	imgLogo_.pos = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y };

	imgPush_.handleId = res.GetHandle("pushButton");
	imgPush_.pos = { Application::SCREEN_HALF_X, 500 };

	alpha_ = 255;
	isRev_ = -1;
}

void TitleScreen::Update()
{
	// アルファ値の更新
	alpha_ += 0.5f * isRev_;

	if (alpha_ > 255)
	{
		alpha_ = 255;
		isRev_ = -1;
	}
	else if(alpha_ < 0)
	{
		alpha_ = 0;
		isRev_ = 1;
	}

	if
}

void TitleScreen::Draw()
{
	// ロゴ画像
	imgLogo_.DrawRota();
	
	// プッシュ画像
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alpha_);
	imgPush_.DrawRota();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
