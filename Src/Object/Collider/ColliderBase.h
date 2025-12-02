#pragma once
#include "../../Manager/Game/CollisionTags.h"
#include "../Common/Transform.h"
#include "ColliderType.h"

class ActorBase;

class ColliderBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">所有者のインスタンス</param>
	/// <param name="tag">衝突物の種類</param>
	ColliderBase(ActorBase& owner, const CollisionTags::TAG tag);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ColliderBase();	
	
	/// <summary>
	/// デバッグの描画
	/// </summary>
	virtual void DebugDraw();

	/// <summary>
	/// 衝突後の処理
	/// </summary>
	/// <param name="opponentCollider">衝突相手のインスタンス</param>
	void OnHit(const std::weak_ptr<ColliderBase>& opponentCollider);

	/// <summary>
	/// 衝突してるか返す
	/// </summary>
	/// <returns>衝突していたらtrue,してない場合false</returns>
	const bool IsHit() const { return isHit_; }

	/// <summary>
	/// 削除判定
	/// </summary>
	/// <returns>削除する場合true,そうでない場合false</returns>
	const bool IsDelete() const { return isDelete_; }

	/// <summary>
	/// 削除設定
	/// </summary>
	void SetDelete() { isDelete_ = true; }

	/// <summary>
	/// 自身のコライダー種類を返す
	/// </summary>
	/// <returns>種類</returns>
	const ColliderType::TYPE GetType() const { return type_; }

	/// <summary>
	/// 自身の衝突物の種類を返す
	/// </summary>
	/// <returns>衝突物の種類</returns>
	const CollisionTags::TAG GetTag() const { return tag_; }

	/// <summary>
	/// 衝突相手の種類を返す
	/// </summary>
	/// <returns>相手の衝突物の種類</returns>
	const CollisionTags::TAG GetPartnerTag() const { return partnerTag_; }

	/// <summary>
	/// 所有者のインスタンスを返す
	/// </summary>
	/// <returns>所有者のインスタンス</returns>
	const ActorBase& GetOwner() const { return owner_; }

	/// <summary>
	/// 衝突相手の種類を設定
	/// </summary>
	/// <param name="partnerTag"></param>
	void SetPertnerTag(const CollisionTags::TAG partnerTag) { partnerTag_ = partnerTag; }

	/// <summary>
	/// 衝突タグの変更
	/// </summary>
	void ChangeTag(const CollisionTags::TAG changeTag) { tag_ = changeTag; }

protected:

	// 所有者
	ActorBase& owner_;

	// 所有者のトランスフォーム
	const Transform& transformOwner_;	
	
	// 衝突物の種類
	CollisionTags::TAG tag_;	

	// 相手の衝突物種類
	CollisionTags::TAG partnerTag_;	

	// 自身のコライダー種類
	ColliderType::TYPE type_;

	//衝突判定
	bool isHit_;

	// 削除判定
	bool isDelete_;

	// 相対座標を回転させてワールド座標で取得する
	VECTOR GetRotPos(const VECTOR& localPos) const;
};