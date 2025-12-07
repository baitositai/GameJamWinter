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
class Timer;

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
	
	static constexpr VECTOR FIX_CAMERA_POS = { 300.0f, 595.0f, 926.0f };
	static constexpr VECTOR FIX_CAMERA_TARGET_POS = { 300.0f, 505.0f, 750.0f };
	static constexpr VECTOR FIX_CAMERA_ANGLES = { 0.52f,3.15f, 0.0f };

	// 敵の生成までの時間
	static constexpr float FIRST_ENEMY_CREATE_TIME = 1.2f;
	static constexpr float SECOND_ENEMY_CREATE_TIME = 1.0f;
	static constexpr float THIRD_ENEMY_CREATE_TIME = 0.6f;

	// エフェクトの制御
	std::unique_ptr<ControllerEffect> effect_;

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

	//デバッグ処理
	void DebugUpdate();
	void DebugDraw();
};