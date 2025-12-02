#pragma once
#include "ColliderBase.h"

class ColliderSphere : public ColliderBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">所有者のインスタンス</param>
	/// <param name="tag">衝突物の種類</param>
	ColliderSphere(ActorBase& owner, const CollisionTags::TAG tag);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ColliderSphere();

	/// <summary>
	/// デバッグの描画
	/// </summary>
	void DebugDraw() override;

	/// <summary>
	/// 座標位置を返す
	/// </summary>
	/// <returns>座標位置</returns>
	const VECTOR& GetPos() const { return transformOwner_.pos; }

	/// <summary>
	/// 半径を返す
	/// </summary>
	/// <param name=""></param>
	/// <returns>半径</returns>
	float GetRadius(void) const { return radius_; }

	/// <summary>
	/// 半径の設定
	/// </summary>
	/// <param name="radius">半径</param>
	void SetRadius(float radius) { radius_ = radius; }

private:

	// 半径
	float radius_;

};

