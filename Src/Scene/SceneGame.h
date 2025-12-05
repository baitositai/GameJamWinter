#pragma once
#include <memory>
#include "SceneBase.h"

class ScenePause;
class Stage;

class SceneGame : public SceneBase
{
public:

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

	// ステージ
	std::unique_ptr<Stage> stage_;

	// スカイドーム

	//ポーズ画面
	std::shared_ptr<ScenePause> ScenePause_;

	// 更新関数
	void NormalUpdate() override;

	// 描画関数
	void NormalDraw() override;

	// 処理の変更
	void ChangeNormal() override;

	//デバッグ処理
	void DebugUpdate();
	void DebugDraw();
};