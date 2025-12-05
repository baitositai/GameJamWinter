#include "ActorBase.h"
#include "../Manager/Common/ResourceManager.h"
#include "../Manager/Common/SceneManager.h"
#include "../Manager/Game/CollisionManager.h"
#include "../Utility/Utility3D.h"
#include "../Utility/UtilityCommon.h"
#include "../Collider/ColliderBase.h"
#include "../Collider/ColliderFactory.h"

ActorBase::ActorBase() :
	resMng_(ResourceManager::GetInstance()),
	scnMng_(SceneManager::GetInstance()),
	collMng_(CollisionManager::GetInstance()),
	collFtr_(ColliderFactory::GetInstance())
{	
	isActive_ = true;	// 生成時は全員アクティブで
	isDelete_ = false;
	transform_ = {};
	collider_ = nullptr;
}

ActorBase::~ActorBase()
{
}

void ActorBase::Init()
{
	AddCollider();

	InitTransform();
}

void ActorBase::Update()
{	
	if (!isActive_) { return; }

	UpdateBody();

	UpdateApply();

#ifdef _DEBUG
	DebugUpdate();
#endif
}

void ActorBase::Draw()
{
	if (!isActive_) { return; }

	DrawMain();

	DrawTranslucent();

	DrawUI();

#ifdef _DEBUG
	DebugDraw();
#endif 
}

void ActorBase::InitTransform()
{
	//transform_.scl = INITIAL_SCL;
	//transform_.pos = INITIAL_POS;	
	//transform_.rot = INITIAL_ROT;
	//transform_.quaRot = Quaternion();
	//transform_.quaRotLocal = Quaternion::Euler({ 0.0f,UtilityCommon::Deg2RadF(DEFAULT_LOCAL_QUAROT_Y_DEG), 0.0f });
	//transform_.Update();
}

void ActorBase::UpdateBody()
{
}

void ActorBase::UpdateApply()
{
	transform_.Update();
}

void ActorBase::DrawMain()
{
}

void ActorBase::DrawTranslucent()
{
}

void ActorBase::DrawUI()
{
}

void ActorBase::AddCollider()
{
}

void ActorBase::OnHit(const std::weak_ptr<ColliderBase>& opponentCollider)
{
	MV1RefreshCollInfo(transform_.modelId);
}

void ActorBase::DebugUpdate()
{
}

void ActorBase::DebugDraw()
{
}