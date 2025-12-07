#include <DxLib.h>
#include <map>
#include <vector>
#include <string>
#include "../Application.h"
#include "../Manager/Common/SceneManager.h"
#include "../Manager/Common/Camera.h"
#include "../Manager/Common/InputManager.h"
#include "../Manager/Common/ResourceManager.h"
#include "../Manager/Common/FontManager.h"
#include "../Manager/Common/SoundManager.h"
#include "../Object/Pitfall/Pitfall.h"
#include "../Object/Player/Player.h"
#include "../Manager/Game/ShadowManager.h"
#include "../Object/Actor/Stage/Stage.h"
#include "../Object/Actor/Stage/SkyDome.h"
#include "../Object/Common/ControllerEffect.h"
#include "../Object/Enviroment/Shadow.h"
#include "../Utility/UtilityCommon.h"
#include "../Utility/Utility3D.h"
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

	for (auto& player : player_)
	{
		player = std::make_shared<Player>(Transform::Transform());
		player_.emplace_back(player);
	}

	// 影
	//shadow_ = std::make_unique<Shadow>();
	//shadow_->Init();

	// ステージ
	stage_ = std::make_unique<Stage>();
	stage_->Init();

	// スカイドーム
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Init();

	// エフェクト制御
	effect_ = std::make_unique<ControllerEffect>(resMng_.GetHandle("petalFall"));

	// BGMの再生
	sndMng_.PlayBgm(SoundType::BGM::GAME);

	// カメラを固定
	mainCamera.ChangeMode(Camera::MODE::FREE);
	mainCamera.SetPos(FIX_CAMERA_POS);
	mainCamera.SetTargetPos(FIX_CAMERA_TARGET_POS);
	mainCamera.SetAngles(FIX_CAMERA_ANGLES);

	// 再生
	effect_->Play({0.0f,0.0f,0.0f}, Quaternion(), Utility3D::VECTOR_ZERO, 1.0f);
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
		int playerCnt = 0;
		for (auto& player : player_)
		{
			// 落とし穴生成
			if (inputMng_.IsTrgDown(InputManager::TYPE::PLAYER_ACTION, Input::JOYPAD_NO::PAD1))
			{
				Transform transform = player.get()->GetTransform();
				pitfalls_[playerCnt].emplace_back(std::make_shared<Pitfall>(transform));
			}

			playerCnt++;
		}
	}

	// ステージの更新
	stage_->Update();

	// スカイドームの更新
	skyDome_->Update();

#ifdef _DEBUG	

	DebugUpdate();

#endif 
}

void SceneGame::NormalDraw(void)
{	
	//const int shadowMapHandle = shadow_->GetShadowMapHandle();

	// スカイドームの描画
	skyDome_->Draw();

	//shadow_->SetUp();

	// ステージの描画
	stage_->Draw();

	//shadow_->Terminate();

	//// 描画に使用するシャドウマップを設定
	//SetUseShadowMap(0, shadowMapHandle);

	// ステージの描画
	//stage_->Draw();

	//// 描画に使用するシャドウマップの設定を解除
	//SetUseShadowMap(0, -1);

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
	//DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, UtilityCommon::CYAN, true);

	// 各落とし穴位置
	int textY = 1;
	const int Y = 16;
	if (!pitfalls_.empty())
	{
		for (auto& [pCnt, pitfall] : pitfalls_)
		{
			if (pitfall.empty()) { continue; }

			std::wstring text = L"P" + std::to_wstring(pCnt) + L":";
			for (int i = 0; i < pitfall.size(); i++)
			{
				std::wstring num = L"[" + std::to_wstring(pitfall[i].get()->GetTransform().pos.x) + L","
					+ std::to_wstring(pitfall[i].get()->GetTransform().pos.z) + L"]";
				text += num;
			}

			DrawString(0, (Y * (pCnt + 1)), text.c_str(), 0xffffff);
		}
	}
	// カメラ位置
	VECTOR cPos = mainCamera.GetPos();
	VECTOR cTarget = mainCamera.GetTargetPos();
	VECTOR cAngles = mainCamera.GetAngles();

	DrawFormatString(0, 60, UtilityCommon::RED, L"カメラ位置：%2f,%2f,%2f", cPos.x, cPos.y, cPos.z);
	DrawFormatString(0, 80, UtilityCommon::RED, L"注視点位置：%2f,%2f,%2f", cTarget.x, cTarget.y, cTarget.z);
	DrawFormatString(0, 100, UtilityCommon::RED, L"カメラ角度：%2f,%2f,%2f", cAngles.x, cAngles.y, cAngles.z);
}