#pragma once
#include "../ActorBase.h"

class ColliderSphere;

class Pitfall : public ActorBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos"></param>
	Pitfall(const VECTOR& pos);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Pitfall();

	/// <summary>
	/// コライダー情報を返す
	/// </summary>
	/// <returns>コライダー情報</returns>
	const std::shared_ptr<ColliderSphere>& GetSphere() const { return collider_; }

private:

	// 初期位置
	const VECTOR INIT_POS;
	
	// 衝突判定用半径
	static constexpr float RADIUS = 50.0f;

	// コライダー
	std::shared_ptr<ColliderSphere> collider_;

	// トランスフォーム初期化処理
	void InitTransform() override;

	// コライダー生成
	void InitCollider() override;

	// デバッグ描画
	void DebugDraw() override;
};