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