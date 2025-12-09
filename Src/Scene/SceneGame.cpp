#include <string>
#include <random>
#include "../Application.h"
#include "../Manager/Common/SceneManager.h"
#include "../Manager/Common/Camera.h"
#include "../Manager/Common/InputManager.h"
#include "../Manager/Common/ResourceManager.h"
#include "../Manager/Common/FontManager.h"
#include "../Manager/Common/SoundManager.h"
#include "../Object/Actor/Pitfall/Pitfall.h"
#include "../Object/Actor/Character/Player.h"
#include "../Object/Actor/Character/Enemy.h"
#include "../Object/Actor/Stage/Stage.h"
#include "../Object/Actor/Stage/SkyDome.h"
#include "../Object/Common/ControllerEffect.h"
#include "../Object/Common/ControllerCamera.h"
#include "../Object/Collider/ColliderCapsule.h"
#include "../Object/Collider/ColliderSphere.h"
#include "../Object/Enviroment/Shadow.h"
#include "../Core/Common/Timer.h"
#include "../Core/Main/Score.h"
#include "../Core/Main/TitleScreen.h"
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

	state_ = STATE::TITLE;

	// 状態遷移処理
	changeStateMap_.emplace(STATE::TITLE, std::bind(&SceneGame::ChangeStateTitle, this));
	changeStateMap_.emplace(STATE::CAMERA_ROLL_TO_DOWN, std::bind(&SceneGame::ChangeStateCameraRollDown, this));
	changeStateMap_.emplace(STATE::READY, std::bind(&SceneGame::ChangeStateReady, this));
	changeStateMap_.emplace(STATE::MAIN, std::bind(&SceneGame::ChangeStateMain, this));
	changeStateMap_.emplace(STATE::END, std::bind(&SceneGame::ChangeStateEnd, this));
	changeStateMap_.emplace(STATE::RESULT, std::bind(&SceneGame::ChangeStateResult, this));
	changeStateMap_.emplace(STATE::CAMERA_ROLL_TO_UP, std::bind(&SceneGame::ChangeStateCameraRollUp, this));
}

SceneGame::~SceneGame()
{
}

void SceneGame::Init(void)
{
	SceneBase::Init();

	// ポーズ画面のリソース
	ScenePause_ = std::make_shared<ScenePause>();
	ScenePause_->Load();

	// ステージ
	stage_ = std::make_unique<Stage>();
	stage_->Init();

	// スカイドーム
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Init();

	// 敵生成用タイマー
	enemyCreateTimer_ = std::make_unique<Timer>(FIRST_ENEMY_CREATE_TIME);
	enemyCreateTimer_->InitCountUp();

	// ゲーム時間タイマー
	gameTimer_ = std::make_unique<Timer>(GAME_TIME);
	gameTimer_->InitCountUp();

	// エフェクト制御
	effect_ = std::make_unique<ControllerEffect>(resMng_.GetHandle("petalFall"));
	effectFall_ = std::make_unique<ControllerEffect>(resMng_.GetHandle("Fall"));

	// カメラ制御
	camera_ = std::make_unique<ControllerCamera>();
	camera_->Init();

	// スコア
	score_ = std::make_unique<Score>();
	score_->Init();

	// タイトル
	title_ = std::make_unique<TitleScreen>();
	title_->Init();

	// カメラを固定
	mainCamera.ChangeMode(Camera::MODE::FIXED_POINT);
	mainCamera.SetPos(TITLE_FIX_CAMERA_POS);
	mainCamera.SetTargetPos(TITLE_FIX_CAMERA_TARGET_POS);
	mainCamera.SetAngles(TITLE_FIX_CAMERA_ANGLES);

	// 再生
	effect_->Play({0.0f,0.0f,0.0f}, Quaternion(), { 100.0f, 100.0f, 100.0f}, 1.0f);

	// 初期状態設定
	ChangeState(STATE::TITLE);
}

void SceneGame::NormalUpdate(void)
{
	update_();

	skyDome_->Update();
}

void SceneGame::NormalDraw(void)
{	
	// スカイドームの描画
	skyDome_->Draw();

	// ステージの描画
	stage_->Draw();

	// 共通描画
	DrawCommon();

	// 状態別UI描画
	draw_();

#ifdef _DEBUG

	// 落とし穴の描画
	for (const auto& pitFall : pitFalls_)
	{
		pitFall->Draw();
	}

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

void SceneGame::CreatePitFall()
{
	int playerIndex = 0;
	for (auto& player : players_)
	{
		// 生成不可の場合(生成予定の落とし穴あり)
		if (!player->IsCreatePitFall())
		{
			// 落とし穴の生成
			auto pitFall = std::make_shared<Pitfall>(playerIndex, player->GetCreatePitFallPos());
			pitFall->Init();
			pitFalls_.push_back(std::move(pitFall));

			// 生成可能に設定
			player->SetCreatePitFall();
		}
		playerIndex++;
	}
}

void SceneGame::CreateEnemy()
{
	// 時間に達した場合
	if (enemyCreateTimer_->CountUp())
	{
		VECTOR pos = Utility3D::VECTOR_ZERO;
		constexpr float OFFSET_X = 200;
		constexpr float OFFSET_Z_MIN = 200;
		constexpr float OFFSET_Z_MAX = 500;
		constexpr float OFFSET_RANGE_X = 100;
		const float offset = OFFSET_X - GetRand(OFFSET_RANGE_X);

		// 生成位置をランダムで決定
		pos.x = GetRand(1) == 0 ? MOVE_LIMIT_MAX_X + offset : MOVE_LIMIT_MIN_X - offset;
		pos.z = MOVE_LIMIT_MIN_Z + OFFSET_Z_MIN + GetRand(MOVE_LIMIT_MAX_X - (MOVE_LIMIT_MIN_Z + OFFSET_Z_MIN) - OFFSET_Z_MAX);

		// 敵を生成
		auto enemy = std::make_shared<Enemy>(pos);
		enemy->Init();
		enemies_.push_back(enemy);
	}

}

void SceneGame::Collision()
{
	// 配列サイズの取得
	int pitFallSize = 0;

	// 敵と落とし穴の衝突判定
	for (auto& enemy : enemies_)
	{
		pitFallSize = static_cast<int>(pitFalls_.size()) - 1;
		for (int i = pitFallSize; i >= 0; i--)
		{
			// コライダーの取得
			const std::weak_ptr<ColliderCapsule>& capsule = enemy->GetCapsule();
			const std::weak_ptr<ColliderSphere>& sphere = pitFalls_[i]->GetSphere();

			// 判定に必要な情報を取得
			const VECTOR& spherePos = sphere.lock()->GetPos();
			const VECTOR& capPosTop = capsule.lock()->GetPosTop();
			const VECTOR& capPosEnd = capsule.lock()->GetPosDown();
			const float capRadius = capsule.lock()->GetRadius();
			const float sphereRadius = sphere.lock()->GetRadius();

			// 衝突判定
			if (Utility3D::CheckHitSphereToCapsule(sphereRadius, spherePos, capPosTop, capPosEnd, capRadius))
			{
				// スコアを格納
				score_->SetScore(pitFalls_[i]->GetOwnerNo(), 1);
				
				// 落とし穴削除
				pitFalls_.erase(pitFalls_.begin() + i);

				// 敵の状態遷移
				enemy->ChangeState(Enemy::STATE::FALL);

				// 次の敵へ
				continue;
			}
		}
	}

	// プレイヤーと落とし穴の衝突判定
	int playerIndex = 0;
	for (auto& player : players_)
	{
		// 無敵の場合
		if (player->IsInvincible())
		{
			// 番号を更新して次へ
			playerIndex++;
			continue;
		}

		pitFallSize = static_cast<int>(pitFalls_.size()) - 1;
		for (int i = pitFallSize; i >= 0; i--)
		{
			// コライダーの取得
			const std::weak_ptr<ColliderCapsule>& capsule = player->GetCapsule();
			const std::weak_ptr<ColliderSphere>& sphere = pitFalls_[i]->GetSphere();

			// 判定に必要な情報を取得
			const VECTOR& spherePos = sphere.lock()->GetPos();
			const VECTOR& capPosTop = capsule.lock()->GetPosTop();
			const VECTOR& capPosEnd = capsule.lock()->GetPosDown();
			const float capRadius = capsule.lock()->GetRadius();
			const float sphereRadius = sphere.lock()->GetRadius();
			
			// 衝突判定
			if (Utility3D::CheckHitSphereToCapsule(sphereRadius, spherePos, capPosTop, capPosEnd, capRadius))
			{
				// スコアを格納
				score_->SetScore(pitFalls_[i]->GetOwnerNo(), 1);

				// 落下者のスコアを原点
				score_->SetScore(playerIndex, -1);

				// プレイヤーの状態遷移
				player->ChangeState(Player::STATE::FALL);

				// SE再生
				//sndMng_.PlaySe(SoundType::SE::FALL);

				// 次のプレイヤーへ
				break;
			}
		}
		// プレイヤーの番号更新
		playerIndex++;
	}
}

void SceneGame::Reset()
{
	players_.clear();
	enemies_.clear();
	pitFalls_.clear();
	score_->Init();
	gameTimer_->InitCountUp();
	enemyCreateTimer_->InitCountUp();
	title_->Init();
}

void SceneGame::ChangeState(const STATE state)
{
	state_ = state;

	changeStateMap_[state]();
}

void SceneGame::ChangeStateTitle()
{
	update_ = std::bind(&SceneGame::UpdateTitle, this);

	draw_ = std::bind(&SceneGame::DrawTitle, this);

	sndMng_.FadeOutSe(SoundType::SE::CHEERS);
	sndMng_.PlayBgm(SoundType::BGM::TITLE);

	int i = 0;
	if (!sndMng_.IsPlay(SoundType::BGM::TITLE))
	{
		i++;
		return;
	}
	//PlaySoundMem(resMng_.GetHandle("bgmTitle"), DX_PLAYTYPE_LOOP);
}

void SceneGame::ChangeStateCameraRollDown()
{
	update_ = std::bind(&SceneGame::UpdateCameraRollDown, this);

	draw_ = std::bind(&SceneGame::DrawNone, this);

	camera_->Set(FIX_CAMERA_POS, FIX_CAMERA_TARGET_POS, Utility3D::DIR_U, 0.0f, 2.0f);

	sndMng_.PlaySe(SoundType::SE::CHEERS);
	//StopSoundMem(resMng_.GetHandle("bgmTitle"));
	sndMng_.StopBgm(SoundType::BGM::TITLE);
}

void SceneGame::ChangeStateReady()
{
	update_ = std::bind(&SceneGame::UpdateReady, this);

	draw_ = std::bind(&SceneGame::DrawReady, this);

	sndMng_.FadeOutSe(SoundType::SE::CHEERS);
	sndMng_.PlayBgm(SoundType::BGM::GAME);
}

void SceneGame::ChangeStateMain()
{
	update_ = std::bind(&SceneGame::UpdateMain, this);

	draw_ = std::bind(&SceneGame::DrawMain, this);
}

void SceneGame::ChangeStateEnd()
{
	update_ = std::bind(&SceneGame::UpdateEnd, this);

	draw_ = std::bind(&SceneGame::DrawEnd, this);
}

void SceneGame::ChangeStateResult()
{
	update_ = std::bind(&SceneGame::UpdateResult, this);

	draw_ = std::bind(&SceneGame::DrawResult, this);
}

void SceneGame::ChangeStateCameraRollUp()
{
	update_ = std::bind(&SceneGame::UpdateCameraRollUp, this);

	draw_ = std::bind(&SceneGame::DrawNone, this);
	
	Reset();
	
	camera_->Set(TITLE_FIX_CAMERA_POS, TITLE_FIX_CAMERA_TARGET_POS, Utility3D::DIR_U, 0.0f, 2.0f);

	sndMng_.PlaySe(SoundType::SE::CHEERS);
	sndMng_.StopBgm(SoundType::BGM::GAME);
}

void SceneGame::UpdateTitle()
{
	title_->Update();

	if (title_->IsEnd())
	{
		// プレイヤー生成
		const int playerCnt = title_->GetPlayerNum();
		for (int i = 0; i < playerCnt; i++)
		{
			auto player = std::make_shared<Player>(VECTOR{ -200.0f + 150.0f * i,0.0f,0.0f }, static_cast<Input::JOYPAD_NO>(i + 1));
			player->Init();
			players_.emplace_back(player);
		}

		// 状態遷移
		ChangeState(STATE::CAMERA_ROLL_TO_DOWN);
	}
	int i = 0;
	if (!sndMng_.IsPlay(SoundType::BGM::TITLE))
	{
		i++;
		return;
	}
}

void SceneGame::UpdateCameraRollDown()
{	
	if(camera_->IsEnd())
	{
		ChangeState(STATE::MAIN);
	}
	else
	{
		camera_->Update();
	}
}

void SceneGame::UpdateReady()
{
}

void SceneGame::UpdateMain()
{
	// ポーズ画面
	if (inputMng_.IsTrgDown(InputManager::TYPE::PAUSE))
	{
		scnMng_.PushScene(ScenePause_);
		return;
	}

	// プレイヤー更新
	for (const auto& player : players_)
	{
		player->Update();
	}

	// 敵更新
	const int enemySize = enemies_.size() - 1;
	for (int i = enemySize; i >= 0; i--)
	{
		enemies_[i]->Update();
		// 削除判定がある場合
		if (enemies_[i]->IsDelete())
		{
			// 落下エフェクト
			const float SCALE = 17.5f;
			VECTOR pos = enemies_[i]->GetTransform().pos;
			if (pos.y < 0.0f)
			{
				pos.y = 0.0f;
				effectFall_->Play(pos, Quaternion::Identity(), { SCALE, SCALE, SCALE }, 1.0f, false);
			}

			// 敵削除
			enemies_.erase(enemies_.begin() + i);
		}
	}

	// ステージの更新
	stage_->Update();

	// 敵生成処理
	CreateEnemy();

	// 落とし穴生成処理
	CreatePitFall();

	// 衝突判定
	Collision();

	// ゲーム終了
	if (gameTimer_->CountUp())
	{
		ChangeState(STATE::CAMERA_ROLL_TO_UP);
	}

#ifdef _DEBUG	

	DebugUpdate();

#endif 
}

void SceneGame::UpdateEnd()
{
}

void SceneGame::UpdateResult()
{
}

void SceneGame::UpdateCameraRollUp()
{
	if (camera_->IsEnd())
	{
		ChangeState(STATE::TITLE);
	}
	else
	{
		camera_->Update();
	}
}

void SceneGame::DrawTitle()
{
	title_->Draw();
}

void SceneGame::DrawReady()
{
}

void SceneGame::DrawMain()
{
}

void SceneGame::DrawEnd()
{
}

void SceneGame::DrawResult()
{
}

void SceneGame::DrawCommon()
{
	// タイトルのみ非表示
	if (STATE::TITLE == state_)
	{
		return;
	}

	// プレイヤーの描画
	for (const auto& player : players_)
	{
		player->Draw();
	}

	// 敵の描画
	for (const auto& enemy : enemies_)
	{
		enemy->Draw();
	}
}

void SceneGame::DebugUpdate(void)
{

}

void SceneGame::DebugDraw(void)
{
	// 各落とし穴位置
	int textY = 1;
	const int Y = 16;
	
	// カメラ位置
	VECTOR cPos = mainCamera.GetPos();
	VECTOR cTarget = mainCamera.GetTargetPos();
	VECTOR cAngles = mainCamera.GetAngles();

	DrawFormatString(0, 60, UtilityCommon::RED, L"カメラ位置：%2f,%2f,%2f", cPos.x, cPos.y, cPos.z);
	DrawFormatString(0, 80, UtilityCommon::RED, L"注視点位置：%2f,%2f,%2f", cTarget.x, cTarget.y, cTarget.z);
	DrawFormatString(0, 100, UtilityCommon::RED, L"カメラ角度：%2f,%2f,%2f", cAngles.x, cAngles.y, cAngles.z);
	DrawFormatString(0, 120, UtilityCommon::RED, L"ゲーム時間：%d", gameTimer_->GetCount());

	// 移動制限の区域を描画
	constexpr float OFFSET_Y = 50.0f;
	VECTOR top = { MOVE_LIMIT_MIN_X, OFFSET_Y, MOVE_LIMIT_MIN_Z };
	VECTOR end = { MOVE_LIMIT_MIN_X, OFFSET_Y, MOVE_LIMIT_MAX_Z };
	DrawLine3D(top, end, UtilityCommon::RED);

	top = { MOVE_LIMIT_MIN_X, OFFSET_Y, MOVE_LIMIT_MAX_Z };
	end = { MOVE_LIMIT_MAX_X, OFFSET_Y, MOVE_LIMIT_MAX_Z };
	DrawLine3D(top, end, UtilityCommon::RED);

	top = { MOVE_LIMIT_MAX_X, OFFSET_Y, MOVE_LIMIT_MAX_Z };
	end = { MOVE_LIMIT_MAX_X, OFFSET_Y, MOVE_LIMIT_MIN_Z };
	DrawLine3D(top, end, UtilityCommon::RED);

	top = { MOVE_LIMIT_MAX_X, OFFSET_Y, MOVE_LIMIT_MIN_Z };
	end = { MOVE_LIMIT_MIN_X, OFFSET_Y, MOVE_LIMIT_MIN_Z };
	DrawLine3D(top, end, UtilityCommon::RED);
}