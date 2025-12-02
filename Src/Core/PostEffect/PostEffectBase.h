#pragma once
#include <memory>
#include "../CoreBase.h"

class PixelMaterial;
class PixelRenderer;

class PostEffectBase : public CoreBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostEffectBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~PostEffectBase();

	/// <summary>
	/// 読み込み処理
	/// </summary>
	virtual void Load();

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw();

protected:

	// マテリアル
	std::unique_ptr<PixelMaterial> material_;

	// レンダラー
	std::unique_ptr<PixelRenderer> renderer_;

};