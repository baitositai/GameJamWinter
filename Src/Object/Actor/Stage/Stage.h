#pragma once
#include "../ActorBase.h"

class SceneGame;

class Stage : public ActorBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Stage(SceneGame& parent);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Stage() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init()override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw()override;

private:

	SceneGame& parent_;
};