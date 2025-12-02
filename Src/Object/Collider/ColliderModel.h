#pragma once
#include "ColliderBase.h"
#include "../Common/Transform.h"

class ColliderModel : public ColliderBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">所有者のインスタンス</param>
	/// <param name="tag">衝突物の種類</param>
	ColliderModel(ActorBase& owner, const CollisionTags::TAG tag);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ColliderModel() override;

	/// <summary>
	/// 判定情報配列の後始末
	/// </summary>
	void CleaningPolyDim();

	/// <summary>
	/// コリジョン結果代入用ポリゴン配列を設定
	/// </summary>
	/// <param name="hitInfo">コリジョン結果代入用ポリゴン配列</param>
	void SetCollResultPolyDim(const MV1_COLL_RESULT_POLY_DIM hitInfo) { collResultPolyDim_ = hitInfo; }

	/// <summary>
	/// コリジョン結果代入用ポリゴン
	/// </summary>
	/// <param name="hitInfo">コリジョン結果代入用ポリゴン</param>
	void SetCollResultPoly(const MV1_COLL_RESULT_POLY hitInfo) { collResultPoly_ = hitInfo; }

	/// <summary>
	/// モデルを返す
	/// </summary>
	/// <returns>モデル</returns>
	const int GetModel() const { return transformOwner_.modelId; }

	/// <summary>
	/// コリジョン結果代入用ポリゴン配列を返す
	/// </summary>
	/// <returns>コリジョン結果代入用ポリゴン配列</returns>
	const MV1_COLL_RESULT_POLY_DIM& GetCollResultPolyDim() const{ return collResultPolyDim_; }

	/// <summary>
	/// コリジョン結果代入用ポリゴン
	/// </summary>
	/// <returns>コリジョン結果代入用ポリゴン</returns>
	const MV1_COLL_RESULT_POLY& GetCollResultPoly() const { return collResultPoly_; }

private:

	// コリジョン結果代入用ポリゴン配列
	MV1_COLL_RESULT_POLY_DIM collResultPolyDim_;

	// コリジョン結果代入用ポリゴン
	MV1_COLL_RESULT_POLY collResultPoly_;
};

