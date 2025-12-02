#include "../../Utility/UtilityCommon.h"
#include "ColliderSphere.h"

ColliderSphere::ColliderSphere(ActorBase& owner, const CollisionTags::TAG tag) : 
	ColliderBase(owner, tag)
{
	type_ = ColliderType::TYPE::SPHERE;
	radius_ = 0.0f;
}

ColliderSphere::~ColliderSphere()
{
}

void ColliderSphere::DebugDraw()
{
	DrawSphere3D(transformOwner_.pos, radius_, 16.0f, UtilityCommon::RED, UtilityCommon::RED, false);
}
