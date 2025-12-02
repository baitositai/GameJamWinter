#pragma once
#include "PostEffectBase.h"

class PostEffectCracks : public PostEffectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostEffectCracks();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PostEffectCracks() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

private:

	// ステップ
	float step_;

	// バッファーサイズ
	static constexpr int CONST_BUFFER_SIZE = 1;

};

