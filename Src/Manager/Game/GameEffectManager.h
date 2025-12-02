#pragma once
#include <unordered_map>
#include "../../Template/Singleton.h"

class PostEffectBase;

class GameEffectManager : public Singleton<GameEffectManager>
{
	// シングルトンにだけ共有する
	friend class Singleton<GameEffectManager>;

public:

	// 種類
	enum class TYPE
	{
		NONE,		// 使用しない
	};

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// エフェクトの切り替え
	/// </summary>
	/// <param name="type">エフェクトの種類</param>
	void ChangeEffect(const TYPE type) { type_ = type; }

private:	

	// 種類
	TYPE type_;
	
	// エフェクトスクリーン
	int effectScreen_;

	// ポストエフェクトの管理
	std::unordered_map<TYPE, std::unique_ptr<PostEffectBase>> effectMap_;

	// コンストラクタ
	GameEffectManager();

	// デストラクタ
	~GameEffectManager();
};

