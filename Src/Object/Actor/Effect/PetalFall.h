#pragma once
#include "../ActorBase.h"

class PetalFall : public ActorBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PetalFall();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PetalFall() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

private:

};

