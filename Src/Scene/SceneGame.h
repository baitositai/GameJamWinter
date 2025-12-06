#pragma once
#include <memory>
#include <vector>
#include <map>
#include "SceneBase.h"
#include "../Common/CharacterString.h"
class ScenePause;
class Player;
class Pitfall;

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

	// テキスト
	CharacterString text_;

	//ポーズ画面
	std::shared_ptr<ScenePause> ScenePause_;

	// 落とし穴
	std::map<int, Pitfall*> pitfalls_;

	// プレイヤー
	std::vector<Player*> player_;


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