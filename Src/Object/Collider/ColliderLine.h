#pragma once
#include "ColliderBase.h"

class ActorBase;

class ColliderLine : public ColliderBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">所有者のインスタンス</param>
	/// <param name="tag">衝突物の種類</param>
	ColliderLine(ActorBase& owner, const CollisionTags::TAG tag);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ColliderLine() override;

	/// <summary>
	/// デバッグの描画
	/// </summary>
	void DebugDraw() override;

	/// <summary>
	/// 先頭の点の座標を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns> 先頭の点の座標</returns>
	const VECTOR& GetLocalPosPointHead()const { return localPosPointHead_; }

	/// <summary>
	/// 末尾の点の座標を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns> 末尾の点の座標</returns>
	const VECTOR& GetLocalPosPointEnd()const { return localPosPointEnd_; }

	/// <summary>
	/// 回転済みの先頭の点の座標を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>回転済みの先頭の点の座標</returns>
	const VECTOR& GetPosPointHead() const { return GetRotPos(localPosPointHead_); }

	/// <summary>
	/// 回転済みの末尾の点の座標を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>回転済みの末尾の点の座標</returns>
	const VECTOR& GetPosPointEnd() const { return GetRotPos(localPosPointEnd_); }

	/// <summary>
	/// 先頭の点の座標の設定
	/// </summary>
	/// <param name="_pos">先頭の点の座標</param>
	void SetLocalPosPointHead(const VECTOR& pos) { localPosPointHead_ = pos; }

	/// <summary>
	/// 末尾の点の座標の設定
	/// </summary>
	/// <param name="_pos">末尾の点の座標</param>
	void SetLocalPosPointEnd(const VECTOR& pos) { localPosPointEnd_ = pos; }

private:

	// 線分の要素
	VECTOR localPosPointHead_;	// 先頭の点の座標
	VECTOR localPosPointEnd_;	// 末尾の点の座標
};