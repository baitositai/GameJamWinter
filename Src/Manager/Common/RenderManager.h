#pragma once
#include <vector>
#include "../../Template/Singleton.h"

class PixelRenderer;
class ModelRenderer;

class RenderManager : public Singleton<RenderManager>
{
	// シングルトンにだけ共有
	friend class Singleton<RenderManager>;

public:

	enum class TEXTURE_TYPE
	{
		MAIN,
		SHADOW,
		POST_EFFECT,
	};

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();
	
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// ピクセルレンダラーの追加
	/// </summary>
	/// <param name="renderer">ピクセルレンダラー</param>
	void AddPixelRenderer(const std::shared_ptr<PixelRenderer> renderer);

	/// <summary>
	/// モデルレンダラーの追加
	/// </summary>
	/// <param name="renderer">モデルレンダラー</param>
	void AddModelRenderer(const std::shared_ptr<ModelRenderer> renderer);

private:

	// ピクセルレンダラーの配列
	std::vector<std::shared_ptr<PixelRenderer>> pixelRenderers_;

	// モデルレンダラーの配列
	std::vector<std::shared_ptr<ModelRenderer>> modelRenderers_;


};

