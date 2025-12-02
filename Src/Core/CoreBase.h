#pragma once
#include "../Common/CharacterString.h"

class ResourceManager;
class SceneManager;
class FontManager;

class CoreBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CoreBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CoreBase();

	/// <summary>
	/// 読み込み
	/// </summary>
	virtual void Load();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

protected:

	// シングルトン参照
	ResourceManager& resMng_;
	SceneManager& scnMng_;
	FontManager& fontMng_;

	// テキスト
	CharacterString text_;

	// 通常フォントサイズ
	static constexpr int FONT_SIZE = 24;

	// 通常フォント太さ
	static constexpr int FONT_THICK = 0;

	//メインの更新処理
	virtual void UpdateMain();

	//処理の適用
	virtual void UpdateApply();

	//デバッグの更新処理
	virtual void UpdateDebug();

	//メインの描画
	virtual void DrawMain();

	//デバッグの描画処理
	virtual void DrawDebug();

};