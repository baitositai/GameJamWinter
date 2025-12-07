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
	/// 更新処理
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw()override;

private:

	// トランスフォーム初期化情報
	static constexpr VECTOR INITIAL_POS = { 0.0f,0.0f,0.0f };
	static constexpr VECTOR INITIAL_SCL = { 1.0f,1.0f,1.0f };
	static constexpr VECTOR INITIAL_ROT = { 0.0f,0.0f,0.0f };

	// トランスフォームの初期設定
	void InitTransform() override;
};