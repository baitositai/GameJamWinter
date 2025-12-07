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

	// トランスフォーム初期化情報
	static constexpr VECTOR INITIAL_POS = { 0.0f,-1000.0f,0.0f };
	static constexpr VECTOR INITIAL_SCL = { 100.0f,100.0f,100.0f };
	static constexpr VECTOR INITIAL_ROT = { 0.0f,0.0f,0.0f };

	// トランスフォームの初期設定
	void InitTransform() override;
};