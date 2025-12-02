#include "../Actor/ActorBase.h"
#include "ColliderModel.h"

ColliderModel::ColliderModel(ActorBase& owner, const CollisionTags::TAG tag) :
	ColliderBase(owner, tag)
{
	collResultPolyDim_ = {};
	collResultPoly_ = {};
	type_ = ColliderType::TYPE::MODEL;

	// モデルの衝突情報の構築
	int ret = MV1SetupCollInfo(owner_.GetTransform().modelId, -1, 1, 1, 1);
}

ColliderModel::~ColliderModel()
{
}

void ColliderModel::CleaningPolyDim()
{
	if (collResultPolyDim_.HitNum > 0 && collResultPolyDim_.Dim != nullptr)
	{
		// 当たり判定情報の解放(球、カプセル)
		MV1CollResultPolyDimTerminate(collResultPolyDim_);

		// 再初期化
		std::memset(&collResultPolyDim_, 0, sizeof(collResultPolyDim_));
	}
}
