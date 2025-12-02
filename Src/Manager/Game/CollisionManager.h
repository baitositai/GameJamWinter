#pragma once
#include <memory>
#include <vector>
#include <functional>
#include "../../Template/Singleton.h"

class ColliderBase;
class ColliderCapsule;
class ColliderModel;

class CollisionManager : public Singleton<CollisionManager>
{
	//シングルトンにだけ共有する
	friend class Singleton<CollisionManager>;

public:

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// コライダーの追加
	/// </summary>
	/// <param name="collider">コライダー</param>
	void Add(std::shared_ptr<ColliderBase> collider);

	/// <summary>
	/// 削除
	/// </summary>
	void Clear();

	/// <summary>
	/// 不要のコライダーの削除
	/// </summary>
	void Sweep();

	/// <summary>
	/// デバッグ描画
	/// </summary>
	void DebugDraw();

private:

	// 衝突用タグのマトリックス
	std::vector<std::vector<bool>> collTagMatrix_;

	// 衝突判定処理用のマトリックス
	std::vector<std::vector<std::function<bool(std::weak_ptr<ColliderBase>, std::weak_ptr<ColliderBase>)>>> collFuncMatrix_;

	// コライダー配列
	std::vector<std::shared_ptr<ColliderBase>> colliders_;
	
	// タグのマトリックスの設定設定
	void InitTagMatrix();

	// コライダー種のマトリックス設定
	void InitColliderMatrix();

	// モデルとカプセルの衝突判定
	bool IsHitCheckModeToCapsule(std::weak_ptr<ColliderBase> collA, std::weak_ptr<ColliderBase> collB);	
	
	// モデルとラインの衝突判定
	bool IsHitCheckModeToLine(std::weak_ptr<ColliderBase> collA, std::weak_ptr<ColliderBase> collB);
	
	// カプセルとカプセルの衝突判定
	bool IsHitCheckCapsuleToCapsule(std::weak_ptr<ColliderBase> collA, std::weak_ptr<ColliderBase> collB);
	
	// カプセルとボックス
	bool IsHitCheckCapsuleToBox(std::weak_ptr<ColliderBase> collA, std::weak_ptr<ColliderBase> collB);

	// カプセルとライン
	bool IsHitCheckCapsuleToLine(std::weak_ptr<ColliderBase> collA, std::weak_ptr<ColliderBase> collB);

	// カプセルとスフィア
	bool IsHitCheckCapsuleToSphere(std::weak_ptr<ColliderBase> collA, std::weak_ptr<ColliderBase> collB);

	// ラインとボックス
	bool IsHitCheckLineToBox(std::weak_ptr<ColliderBase> collA, std::weak_ptr<ColliderBase> collB);

	// ラインとスフィア
	bool IsHitCheckLineToSphere(std::weak_ptr<ColliderBase> collA, std::weak_ptr<ColliderBase> collB);

	// コンストラクタ
	CollisionManager();

	// デストラクタ
	~CollisionManager();
};