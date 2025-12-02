#pragma once

class SceneManager;

class GlitchScreen
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GlitchScreen();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GlitchScreen();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	// グリッチエフェクトのパラメータ
	static constexpr int GLITCH_LINE_HEIGHT_MIN = 5;	// グリッチのブロックの最小高さ
	static constexpr int GLITCH_LINE_HEIGHT_MAX = 30;	// グリッチのブロックの最大高さ
	static constexpr int GLITCH_OFFSET_MAX = 100;		// 水平方向の最大ズレ量
	static constexpr int GLITCH_DENSITY = 3;			// グリッチ発生の頻度 (小さいほど頻繁)
	static constexpr int NOISE_PIXEL_COUNT = 500;		// ランダムなピクセルノイズの数
	static constexpr int GLITCH_COLOR_MAGNITUDE = 100;	// グリッチで発生する色の変化の大きさ

	// シーン管理クラスの参照
	SceneManager& scnMng_;

	// エフェクトスクリーン
	int effectScreen_;
};

