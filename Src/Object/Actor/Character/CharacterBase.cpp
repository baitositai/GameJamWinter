#include "../../../Manager/Common/SoundManager.h"
#include "../../../Manager/Common/ResourceManager.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../Collider/ColliderCapsule.h"
#include "../../Common/AnimationController.h"
#include "../../../Render/ModelMaterial.h"
#include "../../../Render/ModelRenderer.h"
#include "../../../Scene/SceneGame.h"
#include "CharacterBase.h"

CharacterBase::CharacterBase(SceneGame& parent, const VECTOR& initPos) :
	parent_(parent),
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

	normalMaterial_ = std::make_unique<ModelMaterial>(resMng_.GetHandle("characterVs"), 2, resMng_.GetHandle("characterPs"), 2);
	normalRenderer_ = std::make_unique<ModelRenderer>(transform_.modelId, *normalMaterial_);

	VECTOR cameraPos = GetCameraPosition();
	float fogStart, fogEnd;
	GetFogStartEnd(&fogStart, &fogEnd);
	VECTOR lightDir = GetLightDirection();
	normalMaterial_->AddConstBufVS(FLOAT4{ cameraPos.x, cameraPos.y, cameraPos.z, 0.0f });
	normalMaterial_->AddConstBufVS(FLOAT4{ fogStart, fogEnd, 0.0f, 0.0f });
	normalMaterial_->AddConstBufPS(FLOAT4{ 0.0f, 0.0f, 0.0f, 0.0f });
	normalMaterial_->AddConstBufPS(FLOAT4{ lightDir.x, lightDir.y, lightDir.z, 0.0f });
	normalMaterial_->AddConstBufVSMatrix(parent_.GetLightViewMatrix());
	normalMaterial_->AddConstBufVSMatrix(parent_.GetLightProjectionMatrix());

	normalMaterial_->SetTextureBuf(8, parent_.GetShadowMapTex());

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
	VECTOR cameraPos = GetCameraPosition();
	float fogStart, fogEnd;
	GetFogStartEnd(&fogStart, &fogEnd);
	VECTOR lightDir = GetLightDirection();
	normalMaterial_->SetConstBufVS(0, FLOAT4{ cameraPos.x, cameraPos.y, cameraPos.z, 0.0f });
	normalMaterial_->SetConstBufVS(1, FLOAT4{ fogStart, fogEnd, 0.0f, 0.0f });
	normalMaterial_->SetConstBufPS(0, FLOAT4{ 0.0f, 0.0f, 0.0f, 0.0f });
	normalMaterial_->SetConstBufPS(1, FLOAT4{ lightDir.x, lightDir.y, lightDir.z, 0.0f });
	normalMaterial_->SetConstBufVSMatrix(0, parent_.GetLightViewMatrix());
	normalMaterial_->SetConstBufVSMatrix(1, parent_.GetLightProjectionMatrix());
	normalMaterial_->SetTextureBuf(8, parent_.GetShadowMapTex());
	normalRenderer_->Draw();
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
