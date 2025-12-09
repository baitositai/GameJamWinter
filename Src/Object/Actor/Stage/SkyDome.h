#pragma once
#include "../ActorBase.h"

class SkyDome : public ActorBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SkyDome();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SkyDome() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;
	
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

private:

	// 回転速度
	static constexpr float ROT_SPEED = 0.05f;

	// スカイドーム位置
	static constexpr VECTOR INIT_DOME_POS = { 0.0f, -2000.0f, 0.0f };

	// スカイドームスケール
	static constexpr VECTOR INIT_DOME_SCALE = { 100.0f, 100.0f, 100.0f };

	// トランスフォームの初期設定
	void InitTransform() override;
};