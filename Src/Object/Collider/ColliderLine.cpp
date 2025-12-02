#include "../../Utility/Utility3D.h"
#include "../../Utility/UtilityCommon.h"
#include "ColliderLine.h"

ColliderLine::ColliderLine(ActorBase& owner, const CollisionTags::TAG tag) :
	ColliderBase(owner,tag)
{
	localPosPointHead_ = Utility3D::VECTOR_ZERO;
	localPosPointEnd_ = Utility3D::VECTOR_ZERO;
	type_ = ColliderType::TYPE::LINE;
}

ColliderLine::~ColliderLine()
{
}

void ColliderLine::DebugDraw()
{
	VECTOR point1 = GetRotPos(localPosPointHead_);
	VECTOR point2 = GetRotPos(localPosPointEnd_);

	DrawLine3D(point1, point2, UtilityCommon::RED);
}
