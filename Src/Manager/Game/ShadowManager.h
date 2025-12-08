#pragma once
#include <vector>
#include "../../Template/Singleton.h"

class ModelRenderer;

class ShadowManager : public Singleton<ShadowManager>
{
	// シングルトンにだけ共有する
	friend class Singleton<ShadowManager>;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 描画クラスの追加
	/// </summary>
	/// <param name="renderer">描画処理</param>
	void Add(const std::shared_ptr<ModelRenderer>& renderer);

	/// <summary>
	/// シャドウマップスクリーンを返す
	/// </summary>
	/// <returns>シャドウマップスクリーン</returns>
	const int GetShadowMapScreen() const { return shadowMapScreen_; }

private:

	// レンダラー配列
	std::vector<std::shared_ptr<ModelRenderer>> renderers_;;

	// シャドウマップスクリーンのハンドル
	int shadowMapScreen_;

	// コンストラクタ
	ShadowManager();

	// デストラクタ
	~ShadowManager();
};