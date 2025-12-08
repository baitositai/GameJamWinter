#pragma once
#include <vector>
#include <map>
#include <DxLib.h>
#include "SceneBase.h"

class ScenePause;
class Shadow;
class Player;
class Enemy;
class Pitfall;
class Stage;
class SkyDome;
class ControllerEffect;
class ControllerCamera;
class Timer;
class Score;
class TitleScreen;

class SceneGame : public SceneBase
{
public:

	// 移動制限
	static constexpr float MOVE_LIMIT_MAX_X = 950.0f;
	static constexpr float MOVE_LIMIT_MIN_X = -350.0f;
	static constexpr float MOVE_LIMIT_MAX_Z = 440.0f;
	static constexpr float MOVE_LIMIT_MIN_Z = -400.0f;

	enum class STATE
	{
		TITLE,					// タイトル
		CAMERA_ROLL_TO_GAME,	// カメラを下げる(ゲーム画面を見せる)
		READY,					// 準備(カウントダウン)
		MAIN,					// 本編
		END,					// 終了(画面はそのままで終了用のUIを表示)
		RESULT,					// リザルト画面の表示	
		CAMERA_ROLL_TO_TITLE,	// カメラを上げる(タイトル画面用の視点へ戻る)
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneGame();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SceneGame();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

private:
	
	// タイトル視点
	static constexpr VECTOR TITLE_FIX_CAMERA_POS = { 300.0f, 1350, 1120.0f };
	static constexpr VECTOR TITLE_FIX_CAMERA_TARGET_POS = { 300.0f, 1400.0f, 930.0f };
	static constexpr VECTOR TITLE_FIX_CAMERA_ANGLES = { -0.35,3.15f, 0.0f };

	// メインゲーム視点
	static constexpr VECTOR FIX_CAMERA_POS = { 300.0f, 595.0f, 926.0f };
	static constexpr VECTOR FIX_CAMERA_TARGET_POS = { 300.0f, 505.0f, 750.0f };
	static constexpr VECTOR FIX_CAMERA_ANGLES = { 0.52f,3.15f, 0.0f };

	// 敵の生成までの時間
	static constexpr float FIRST_ENEMY_CREATE_TIME = 1.2f;
	static constexpr float SECOND_ENEMY_CREATE_TIME = 1.0f;
	static constexpr float THIRD_ENEMY_CREATE_TIME = 0.6f;

	// ゲーム時間
	static constexpr float GAME_TIME = 30.0f;

	// 状態
	STATE state_;

	// エフェクトの制御
	std::unique_ptr<ControllerEffect> effect_;

	// カメラ制御
	std::unique_ptr<ControllerCamera> camera_;

	// 影
	std::unique_ptr<Shadow> shadow_;

	// ステージ
	std::unique_ptr<Stage> stage_;

	// スカイドーム
	std::unique_ptr<SkyDome> skyDome_;

	//ポーズ画面
	std::shared_ptr<ScenePause> ScenePause_;

	// 落とし穴
	std::vector<std::shared_ptr<Pitfall>> pitFalls_;

	// プレイヤー
	std::vector<std::shared_ptr<Player>> players_;

	// 敵
	std::vector<std::shared_ptr<Enemy>> enemies_;

	// 敵の生成用タイマー
	std::unique_ptr<Timer> enemyCreateTimer_;

	// ゲームタイマー
	std::unique_ptr<Timer> gameTimer_;

	// スコア
	std::unique_ptr<Score> score_;

	// タイトル
	std::unique_ptr<TitleScreen> title_;

	// 状態別更新処理
	std::function<void()> update_;

	// 状態別描画処理
	std::function<void()> draw_;

	// 状態遷移管理
	std::unordered_map<STATE, std::function<void()>> changeStateMap_;

	// 更新関数
	void NormalUpdate() override;

	// 描画関数
	void NormalDraw() override;

	// 処理の変更
	void ChangeNormal() override;

	// 落とし穴の生成処理
	void CreatePitFall();

	// 敵の生成
	void CreateEnemy();

	// 衝突判定処理
	void Collision();

	// リセット処理
	void Reset();

	// 状態遷移処理
	void ChangeState(const STATE state);
	void ChangeStateTitle();
	void ChangeStateCameraRollDown();
	void ChangeStateReady();
	void ChangeStateMain();
	void ChangeStateEnd();
	void ChangeStateResult();
	void ChangeStateCameraRollUp();

	// 状態別更新処理
	void UpdateTitle();
	void UpdateCameraRollDown();
	void UpdateReady();
	void UpdateMain();
	void UpdateEnd();
	void UpdateResult();
	void UpdateCameraRollUp();

	// 描画処理
	void DrawNone() {};
	void DrawTitle();
	void DrawReady();
	void DrawMain();
	void DrawEnd();
	void DrawResult();

	// 共通描画
	void DrawCommon();

	//デバッグ処理
	void DebugUpdate();
	void DebugDraw();
};