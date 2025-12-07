#pragma once

class Shadow
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Shadow();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Shadow();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 影の設定
	/// </summary>
	void SetUp();

	/// <summary>
	/// 影の終了
	/// </summary>
	void Terminate();

	/// <summary>
	/// 影のエリアハンドルを返す
	/// </summary>
	/// <returns>影のエリア</returns>
	const int GetShadowMapHandle() const { return shadowMapHandle_; }

private:

	// 影の大きさ
	static constexpr float SHADOW_SCALE_X = 4096;
	static constexpr float SHADOW_SCALE_Y = 4096;

	// 影の最小位置
	static constexpr VECTOR SHADOW_MAP_MIN_AREA = { -3000, -1, -1000 };
	static constexpr VECTOR SHADOW_MAP_MAX_AREA = { 3000, 1200, 1000 };

	// 影の方向
	static constexpr VECTOR SHADOW_MAP_LIGHT_DIR = { 0.5f, -0.5f, 0.5f };

	// シャドウマップのハンドル
	int shadowMapHandle_;
};

