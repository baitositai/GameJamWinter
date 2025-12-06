#include <DxLib.h>
#include <map>
#include <vector>
#include "../Application.h"
#include "../Manager/Common/SceneManager.h"
#include "../Manager/Common/Camera.h"
#include "../Manager/Common/InputManager.h"
#include "../Manager/Common/ResourceManager.h"
#include "../Manager/Common/FontManager.h"
#include "../Manager/Common/SoundManager.h"
#include "../Object/Pitfall/Pitfall.h"
#include "../Object/Player/Player.h"
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
	constexpr int FONT_SIZE = 32;
	int font = fontMng_.CreateMyFont(resMng_.GetFontName("fontKazuki"), FONT_SIZE, 0);
	text_.color = UtilityCommon::BLACK;
	text_.string = L"メインだよぉ";
	text_.fontHandle = font;
	text_.pos = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y };

	// ポーズ画面のリソース
	ScenePause_ = std::make_shared<ScenePause>();
	ScenePause_->Load();

	for (auto& player : player_)
	{
		player = new Player();
		player_.emplace_back(player);
	}

	// BGMの再生
	sndMng_.PlayBgm(SoundType::BGM::GAME);
}

void SceneGame::NormalUpdate(void)
{
	// ポーズ画面
	if (inputMng_.IsTrgDown(InputManager::TYPE::PAUSE))
	{
		scnMng_.PushScene(ScenePause_);
		return;
	}

	if (!player_.empty())
	{
		for (int i = 0; i < player_.size(); i++)
		{
			player_.at(i).
			if (inputMng_.IsTrgDown(InputManager::TYPE::PLAYER_ACTION), Input::JOYPAD_NO::PAD1))
			{

			}
		}
	}

#ifdef _DEBUG	

	DebugUpdate();

#endif 
}

void SceneGame::NormalDraw(void)
{	
#ifdef _DEBUG

	DebugDraw();

#endif
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
	text_.DrawCenter();
}