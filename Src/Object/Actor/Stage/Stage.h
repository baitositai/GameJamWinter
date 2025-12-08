#pragma once
#include "../ActorBase.h"

class Stage : public ActorBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Stage();

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
};