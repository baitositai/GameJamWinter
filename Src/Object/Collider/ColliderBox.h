#pragma once
#include "ColliderBase.h"

class ColliderBox : public ColliderBase
{
public:

	// バウンディングボックス
	struct OBB
	{
		VECTOR vMin;
		VECTOR vMax;
		VECTOR axis[3];
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">所有者のインスタンス</param>
	/// <param name="tag">衝突物の種類</param>
	ColliderBox(ActorBase& owner, const CollisionTags::TAG tag);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ColliderBox() override;

	/// <summary>
	/// デバッグの描画
	/// </summary>
	void DebugDraw() override;

	/// <summary>
	/// 回転バウンティボックスの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>回転バウンティボックス</returns>
	const OBB& GetObb()const { return obb_; }

	/// <summary>
	/// 座標を返す
	/// </summary>
	/// <returns>座標</returns>
	const VECTOR& GetPos()const { return transformOwner_.pos; }

	/// <summary>
	/// 箱の最小地点の取得
	/// </summary>
	/// <returns>箱の最小地点</returns>
	const VECTOR& GetVecMin()const { return obb_.vMin; }

	/// <summary>
	/// 箱の最大地点の取得
	/// </summary>
	/// <returns>箱の最大地点</returns>
	const VECTOR& GetVecMax()const { return obb_.vMax; }

	/// <summary>
	/// 回転バウンティボックスの設定
	/// </summary>
	/// <param name="obb">回転バウンティボックス</param>
	void SetObb(const OBB& obb) { obb_ = obb; }

	/// <summary>
	/// 箱の最小地点の設定
	/// </summary>
	/// <param name="min">箱の最小地点</param>
	void SetVecMin(const VECTOR& min) { obb_.vMin = min; }

	/// <summary>
	/// 箱の最大地点の設定
	/// </summary>
	/// <param name="max">箱の最大地点</param>
	void SetVecMax(const VECTOR& max) { obb_.vMax = max; }

	/// <summary>
	/// サイズの半分の設定
	/// </summary>
	/// <param name="halfSize">サイズの半分</param>
	void SetHalfSize(const VECTOR& halfSize);

private:
	
	// 回転バウンディングボックス
	OBB obb_;

	// 箱の回転情報の取得
	const VECTOR GetAxis(const int num)const { return obb_.axis[num]; }

	// クォータニオンから回転軸を計算
	void UpdateObbAxis(void);

	// 各頂点の計算（ワールド座標）
	void CalculateVertices(VECTOR outVertices[8]) const;
};