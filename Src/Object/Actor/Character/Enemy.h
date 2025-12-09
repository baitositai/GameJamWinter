#pragma once
#include "CharacterBase.h"

class Enemy : public CharacterBase
{
public:

	//アニメーション
	enum class ANIM_TYPE
	{
		WALK = 26,
		MAX,
	};


	/// <summary>
	/// コンストラクタ
	/// </summary>
	Enemy(const VECTOR& initPos);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy() override;
	
	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

private:

	// 移動速度
	static constexpr float MOVE_SPEED = 5.0f;

	// 削除を行う判定ライン
	static constexpr float DELETE_LILNE_Y = -200.0f;

	// 移動量
	float movePowX_;

	// 目的地にたどり着いたか判定
	std::function<bool()> checkGoalMove_;

	//アニメーション初期化処理
	void InitAnimation()override;

	// アクションの更新処理
	void UpdateAction() override;

	// 落下の更新処理
	void UpdateFall() override; 

	// 移動が右の場合の判定
	bool IsGoalByMoveRight() const;

	// 移動が左の場合の判定
	bool IsGoalByMoveLeft() const;
};