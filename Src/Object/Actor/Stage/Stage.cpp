#include "../../../Manager/Common/ResourceManager.h"
#include "../../../Manager/Game/ShadowManager.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../../Render/ModelMaterial.h"
#include "../../../Render/ModelRenderer.h"
#include "Stage.h"

Stage::Stage()
{
}

Stage::~Stage()
{
}

void Stage::Init()
{
	// モデルリソースの設定
	transform_.SetModel(resMng_.GetHandle("stage"));

	// 影用の描画設定
	//shadowMaterial_ = std::make_unique<ModelMaterial>(resMng_.GetHandle("normalMeshDepthShadowVs"), 0, resMng_.GetHandle("normalMeshDepthShadowPs"), 0);
	//shadowRenderer_ = std::make_shared<ModelRenderer>(transform_.modelId, *shadowMaterial_);
	//ShadowManager::GetInstance().Add(shadowRenderer_);

	//// 通常描画の描画設定
	//normalMaterial_ = std::make_unique<ModelMaterial>(resMng_.GetHandle("normalMeshDirLightDepthShadowVs"), 0, resMng_.GetHandle("stagePs"), 0);
	//normalRenderer_ = std::make_unique<ModelRenderer>(transform_.modelId, *normalMaterial_);

	//// バッファーやテクスチャの設定
	//normalMaterial_->SetConstMtx(43, GetCameraViewMatrix());
	//normalMaterial_->SetConstMtx(47, GetCameraProjectionMatrix());
	//normalMaterial_->SetTextureBuf(8, ShadowManager::GetInstance().GetShadowMapScreen());

	// トランスフォームの初期化
	InitTransform();
}

void Stage::Update()
{
}

void Stage::Draw()
{
	MV1DrawModel(transform_.modelId);

	//normalMaterial_->SetConstMtx(43, GetCameraViewMatrix());
	//normalMaterial_->SetConstMtx(47, GetCameraProjectionMatrix());
	//normalMaterial_->SetTextureBuf(8, ShadowManager::GetInstance().GetShadowMapScreen());
	//normalRenderer_->Draw();
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
