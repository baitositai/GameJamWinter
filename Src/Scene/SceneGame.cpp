#include <DxLib.h>
#include "../Application.h"
#include "../Manager/Common/SceneManager.h"
#include "../Manager/Common/Camera.h"
#include "../Manager/Common/InputManager.h"
#include "../Manager/Common/ResourceManager.h"
#include "../Manager/Common/FontManager.h"
#include "../Manager/Common/SoundManager.h"
#include "../Object/Actor/Stage/Stage.h"
#include "../Utility/UtilityCommon.h"
#include "ScenePause.h"
#include "SceneGame.h"

SceneGame::SceneGame()
{
	// 更新関数のセット
	updataFunc_ = std::bind(&SceneGame::LoadingUpdate, this);
	// 描画関数のセット
	drawFunc_ = std::bind(&SceneGame::LoadingDraw, this);
}

SceneGame::~SceneGame()
{

}

void SceneGame::Init(void)
{
	// ポーズ画面のリソース
	ScenePause_ = std::make_shared<ScenePause>();
	ScenePause_->Load();

	// ステージ
	stage_ = std::make_unique<Stage>();
	stage_->Init();

	// BGMの再生
	sndMng_.PlayBgm(SoundType::BGM::GAME);

	mainCamera.ChangeMode(Camera::MODE::FREE);
}

void SceneGame::NormalUpdate(void)
{
	// ポーズ画面
	if (inputMng_.IsTrgDown(InputManager::TYPE::PAUSE))
	{
		scnMng_.PushScene(ScenePause_);
		return;
	}

	// ステージの更新
	stage_->Update();

#ifdef _DEBUG	

	DebugUpdate();

#endif 
}

void SceneGame::NormalDraw(void)
{	
#ifdef _DEBUG

	DebugDraw();

#endif

	// ステージの描画
	stage_->Draw();
}

void SceneGame::ChangeNormal(void)
{
	// 処理変更
	updataFunc_ = std::bind(&SceneGame::NormalUpdate, this);
	drawFunc_ = std::bind(&SceneGame::NormalDraw, this);

	//フェードイン開始
	scnMng_.StartFadeIn();
}

void SceneGame::DebugUpdate(void)
{
	// シーン遷移
	if (inputMng_.IsTrgDown(InputManager::TYPE::DEBUG_SCENE_CHANGE))
	{
		scnMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
		return;
	}
}

void SceneGame::DebugDraw(void)
{
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, UtilityCommon::CYAN, true);
}