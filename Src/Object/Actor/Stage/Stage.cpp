#include "../Manager/Common/ResourceManager.h"
#include "../Utility/UtilityCommon.h"
#include "Stage.h"

Stage::Stage()
{
}

Stage::~Stage()
{
}

void Stage::Init()
{
	transform_.SetModel(resMng_.GetHandle("stage"));
	InitTransform();
}

void Stage::Update()
{
}

void Stage::Draw()
{
	MV1DrawModel(transform_.modelId);
}

void Stage::InitTransform()
{
	transform_.scl = INITIAL_SCL;
	transform_.pos = INITIAL_POS;
	transform_.rot = INITIAL_ROT;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal = Quaternion::Euler({ 0.0f,UtilityCommon::Deg2RadF(DEFAULT_LOCAL_QUAROT_Y_DEG), 0.0f });
	transform_.Update();
}
