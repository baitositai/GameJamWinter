#include "../../../Manager/Common/SoundManager.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../Collider/ColliderCapsule.h"
#include "../../Common/AnimationController.h"
#include "CharacterBase.h"

CharacterBase::CharacterBase(const VECTOR& initPos) :
	INIT_POS(initPos)
{
	state_ = STATE::NONE;
	changeStateMap_.emplace(STATE::NONE, std::bind(&CharacterBase::ChangeStateNone, this));
	changeStateMap_.emplace(STATE::ACTION, std::bind(&CharacterBase::ChangeStateAction, this));
	changeStateMap_.emplace(STATE::FALL, std::bind(&CharacterBase::ChangeStateFall, this));
}

CharacterBase::~CharacterBase()
{
}

void CharacterBase::Init()
{
	ActorBase::Init();

	InitAnimation();

	ChangeState(STATE::ACTION);
}

void CharacterBase::Update()
{
	update_();

	// トランスフォームの更新
	transform_.Update();

	anim_->Update();
}

void CharacterBase::Draw()
{
	MV1DrawModel(transform_.modelId);

	ActorBase::Draw();
}

void CharacterBase::ChangeState(const STATE state)
{
	state_ = state;

	changeStateMap_[state_]();
}

void CharacterBase::ChangeStateNone()
{
	update_ = std::bind(&CharacterBase::UpdateNone, this);
}

void CharacterBase::ChangeStateAction()
{
	update_ = std::bind(&CharacterBase::UpdateAction, this);
}

void CharacterBase::ChangeStateFall()
{
	update_ = std::bind(&CharacterBase::UpdateFall, this);

	if (GetRand(10) == 0)
	{
		sndMng_.PlaySe(SoundType::SE::SCREEM);
	}
	else
	{
		sndMng_.PlaySe(SoundType::SE::FALL);
	}
}

void CharacterBase::UpdateAction()
{
}

void CharacterBase::UpdateFall()
{
	// 落下
	transform_.pos.y -= 5.0f;
}

void CharacterBase::InitTransform()
{
	constexpr float SCALE = 0.8f;
	transform_.scl = { SCALE, SCALE, SCALE };
	transform_.pos = INIT_POS;
	transform_.rot = INITIAL_ROT;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal = Quaternion::Euler({ 0.0f,UtilityCommon::Deg2RadF(DEFAULT_LOCAL_QUAROT_Y_DEG), 0.0f });
	transform_.Update();
}

void CharacterBase::InitCollider()
{
	collider_ = std::make_shared<ColliderCapsule>(*this, CollisionTags::TAG::NONE);
	collider_->SetLocalPosTop(PL_CAP_UP_LOCAL_POS);
	collider_->SetLocalPosDown(PL_CAP_DOWN_LOCAL_POS);
	collider_->SetRadius(RADIUS);
}

void CharacterBase::DebugDraw()
{
	collider_->DebugDraw();
}
