#include "../Manager/Common/ResourceManager.h"
#include "../Utility/UtilityCommon.h"
#include "../Utility/Utility3D.h"
#include "SkyDome.h"

SkyDome::SkyDome()
{
}

SkyDome::~SkyDome()
{
}

void SkyDome::Init()
{
	transform_.SetModel(resMng_.GetHandle("skyDome"));
	InitTransform();
}

void SkyDome::Update()
{
	Quaternion yRot = Quaternion::AngleAxis(UtilityCommon::Deg2RadF(ROT_SPEED), Utility3D::AXIS_Y);
	transform_.quaRot = Quaternion::Mult(transform_.quaRot, yRot);
	transform_.Update();
}

void SkyDome::Draw()
{
	MV1DrawModel(transform_.modelId);
}

void SkyDome::InitTransform()
{
	transform_.scl = INIT_DOME_SCALE;
	transform_.pos = INIT_DOME_POS;
	transform_.rot = INITIAL_ROT;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal = Quaternion::Euler({ 0.0f,UtilityCommon::Deg2RadF(DEFAULT_LOCAL_QUAROT_Y_DEG), 0.0f });
	transform_.Update();
}