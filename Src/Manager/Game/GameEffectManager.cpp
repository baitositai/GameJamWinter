#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/Common/SceneManager.h"
#include "../../Core/PostEffect/PostEffectCracks.h"
#include "GameEffectManager.h"

void GameEffectManager::Init()
{


	// エフェクト画面の生成
	effectScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
}

void GameEffectManager::Draw()
{
	// 種類が指定されてない場合描画しない
	if (type_ == TYPE::NONE) { return; }

	//スクリーンの設定
	SetDrawScreen(effectScreen_);

	// 画面を初期化
	ClearDrawScreen();

	// エフェクトを描画
	effectMap_[type_]->Draw();

	// メインに戻す
	SetDrawScreen(SceneManager::GetInstance().GetMainScreen());

	// 描画
	DrawGraph(0, 0, effectScreen_, false);
}

GameEffectManager::GameEffectManager()
{
	type_ = TYPE::NONE;
	effectScreen_ = -1;
}

GameEffectManager::~GameEffectManager()
{
	DeleteGraph(effectScreen_);
}
