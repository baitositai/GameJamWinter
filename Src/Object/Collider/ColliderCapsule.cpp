#include "../Common/Transform.h"
#include "../Utility/UtilityCommon.h"
#include "ColliderCapsule.h"

ColliderCapsule::ColliderCapsule(ActorBase& owner, const CollisionTags::TAG tag) :
	ColliderBase(owner,tag)
{
	radius_ = 0.0f;
	localPosTop_ = { 0.0f, 0.0f, 0.0f };
	localPosDown_ = { 0.0f, 0.0f, 0.0f };
	type_ = ColliderType::TYPE::CAPSULE;
}

ColliderCapsule::~ColliderCapsule()
{
}

void ColliderCapsule::DebugDraw()
{
	constexpr int COLOR = UtilityCommon::RED;
	constexpr int DIV_NUM = 5;

	// ã‚Ì‹…‘Ì
	VECTOR pos1 = GetPosTop();
	DrawSphere3D(pos1, radius_, DIV_NUM, COLOR, COLOR, false);

	// ‰º‚Ì‹…‘Ì
	VECTOR pos2 = GetPosDown();
	DrawSphere3D(pos2, radius_, DIV_NUM, COLOR, COLOR, false);

	VECTOR dir;
	VECTOR s;
	VECTOR e;

	// ‹…‘Ì‚ğŒq‚®ü(X+)
	dir = transformOwner_.GetRight();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, COLOR);

	// ‹…‘Ì‚ğŒq‚®ü(X-)
	dir = transformOwner_.GetLeft();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, COLOR);

	// ‹…‘Ì‚ğŒq‚®ü(Z+)
	dir = transformOwner_.GetForward();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, COLOR);

	// ‹…‘Ì‚ğŒq‚®ü(Z-)
	dir = transformOwner_.GetBack();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, COLOR);

	constexpr float CENTER_RADIUS = 5.0f;
	constexpr int CENTER_DIV = 10;

	// ƒJƒvƒZƒ‹‚Ì’†S
	DrawSphere3D(GetCenter(), CENTER_RADIUS, CENTER_DIV, COLOR, COLOR, true);
}

VECTOR ColliderCapsule::GetPosTop(void) const
{
	return GetRotPos(localPosTop_); 
}

VECTOR ColliderCapsule::GetPosDown(void) const
{
	return GetRotPos(localPosDown_); 
}

VECTOR ColliderCapsule::GetCenter(void) const
{
	VECTOR top = GetPosTop();
	VECTOR down = GetPosDown();
	VECTOR diff = VSub(top, down);
	constexpr float SCALE = 0.5f;
	return VAdd(down, VScale(diff, SCALE));
}