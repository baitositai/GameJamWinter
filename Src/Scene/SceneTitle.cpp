#include <string>
#include <cmath>
#include <DxLib.h>
#include "../Application.h"
#include "../Manager/Common/SceneManager.h"
#include "../Manager/Common/InputManager.h"
#include "../Manager/Common/ResourceManager.h"
#include "../Manager/Common/SoundManager.h"
#include "../Manager/Common/FontManager.h"
#include "../Utility/UtilityCommon.h"
#include "../Core/PostEffect/PostEffectCracks.h"
#include "SceneTitle.h"

SceneTitle::SceneTitle()
{
	// 更新関数のセット
	updataFunc_ = std::bind(&SceneTitle::LoadingUpdate, this);

	// 描画関数のセット
	drawFunc_ = std::bind(&SceneTitle::LoadingDraw, this);

	// 初期化
	effectScreen_ = -1;
	postEffect_ = nullptr;
}

SceneTitle::~SceneTitle()
{
	DeleteGraph(effectScreen_);
}

void SceneTitle::Init()
{
	constexpr int FONT_SIZE = 32;
	int font = fontMng_.CreateMyFont(resMng_.GetFontName("fontKazuki"), FONT_SIZE, 0);
	text_.color = UtilityCommon::BLACK;
	text_.string = L"タイトルだよぉ";
	text_.fontHandle = font;
	text_.pos = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y };

	imgTitle_.handleId = resMng_.GetHandle("titleImage");
	imgTitle_.pos = { 0,0 };
	imgTitle_.size = { Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y };

	postEffect_ = std::make_unique<PostEffectCracks>();
	postEffect_->Init();

	effectScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	sndMng_.PlayBgm(SoundType::BGM::TITLE);
}

void SceneTitle::NormalUpdate()
{	
	// シーン遷移
	if (inputMng_.IsTrgDown(InputManager::TYPE::DEBUG_SCENE_CHANGE))
	{
		scnMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
		sndMng_.PlaySe(SoundType::SE::DECISION);
		return;
	}
}

void SceneTitle::NormalDraw()
{
	// 背景
	imgTitle_.DrawExtend();

	// テキスト中央描画
	text_.DrawCenter();

	// エフェクト設定
	SetDrawScreen(effectScreen_);

	// 画面初期化
	ClearDrawScreen();

	// ポストエフェクト
	postEffect_->Draw();

	// スクリーンを戻す
	SetDrawScreen(scnMng_.GetMainScreen());

	// エフェクトを描画
	DrawGraph(0, 0, effectScreen_, true);
}

void SceneTitle::ChangeNormal()
{
	//処理変更
	updataFunc_ = std::bind(&SceneTitle::NormalUpdate, this);
	drawFunc_ = std::bind(&SceneTitle::NormalDraw, this);

	//フェードイン開始
	scnMng_.StartFadeIn();
}