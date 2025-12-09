#include <DxLib.h>
#include "../../../Manager/Common/ResourceManager.h"
#include "../../../Utility/Utility3D.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../Collider/ColliderSphere.h"
#include "../../Common/ControllerEffect.h"
#include "Pitfall.h"

Pitfall::Pitfall(const int ownerNo, const VECTOR& pos):
	INIT_POS(pos),
	OWNER_NO(ownerNo)
{
}

Pitfall::~Pitfall()
{
}

void Pitfall::Init()
{
	ActorBase::Init();

	effectPitFall_ = std::make_unique<ControllerEffect>(resMng_.GetHandle("setPitFall"));
	VECTOR pos = transform_.pos;
	pos.y += 50.0f;
	float scale = 20.0f;
	effectPitFall_->Play(pos, Quaternion(), { scale, scale, scale }, 1.0f, false);
}

void Pitfall::InitTransform()
{
	transform_.scl = INITIAL_SCL;
	transform_.pos = INIT_POS;
	transform_.rot = INITIAL_ROT;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal = Quaternion::Euler({ 0.0f,UtilityCommon::Deg2RadF(DEFAULT_LOCAL_QUAROT_Y_DEG), 0.0f });
	transform_.Update();
}

void Pitfall::InitCollider()
{
	collider_ = std::make_shared<ColliderSphere>(*this, CollisionTags::TAG::NONE);
	collider_->SetRadius(RADIUS);
}

void Pitfall::DebugDraw()
{
	collider_->DebugDraw();
}
