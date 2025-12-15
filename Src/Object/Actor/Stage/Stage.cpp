#include "../../../Manager/Common/ResourceManager.h"
#include "../../../Manager/Game/ShadowManager.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../../Render/ModelMaterial.h"
#include "../../../Render/ModelRenderer.h"
#include "../../../Scene/SceneGame.h"
#include "Stage.h"

Stage::Stage(SceneGame& parent) :
	parent_(parent)
{
}

Stage::~Stage()
{
}

void Stage::Init()
{
	// モデルリソースの設定
	transform_.SetModel(resMng_.GetHandle("stage"));

	transform_.pos.y -= 300.0f;
	transform_.Update();

	// 影用の描画設定
	//shadowMaterial_ = std::make_unique<ModelMaterial>(resMng_.GetHandle("normalMeshDepthShadowVs"), 0, resMng_.GetHandle("normalMeshDepthShadowPs"), 0);
	//shadowRenderer_ = std::make_shared<ModelRenderer>(transform_.modelId, *shadowMaterial_);
	//ShadowManager::GetInstance().Add(shadowRenderer_);

	//// 通常描画の描画設定
	normalMaterial_ = std::make_unique<ModelMaterial>(resMng_.GetHandle("stageVs"), 2, resMng_.GetHandle("stagePs"), 3);
	normalRenderer_ = std::make_unique<ModelRenderer>(transform_.modelId, *normalMaterial_);

	//// バッファーやテクスチャの設定
	VECTOR cameraPos = GetCameraPosition();
	float fogStart, fogEnd;
	GetFogStartEnd(&fogStart, &fogEnd);
	VECTOR lightDir = GetLightDirection();

	normalMaterial_->AddConstBufVS(FLOAT4{ cameraPos.x, cameraPos.y, cameraPos.z, 0.0f});
	normalMaterial_->AddConstBufVS(FLOAT4{ fogStart, fogEnd, 0.0f, 0.0f});
	normalMaterial_->AddConstBufPS(FLOAT4{ 0.0f, 0.0f, 0.0f, 0.0f});
	normalMaterial_->AddConstBufPS(FLOAT4{ lightDir.x, lightDir.y, lightDir.z, 1.0f});
	normalMaterial_->AddConstBufPS(FLOAT4{ 3.0f,0.0f,0.0f,0.0f });
	normalMaterial_->AddConstBufVSMatrix(parent_.GetLightViewMatrix());
	normalMaterial_->AddConstBufVSMatrix(parent_.GetLightProjectionMatrix());

	normalMaterial_->SetTextureBuf(8, parent_.GetShadowMapTex());

	// トランスフォームの初期化
	InitTransform();
}

void Stage::Draw()
{
	//MV1DrawModel(transform_.modelId);
	VECTOR cameraPos = GetCameraPosition();
	float fogStart, fogEnd;
	GetFogStartEnd(&fogStart, &fogEnd);
	VECTOR lightDir = GetLightDirection();
	normalMaterial_->SetConstBufVS(0,FLOAT4{ cameraPos.x, cameraPos.y, cameraPos.z, 0.0f });
	normalMaterial_->SetConstBufVS(1,FLOAT4{ fogStart, fogEnd, 0.0f, 0.0f });
	normalMaterial_->SetConstBufPS(0,FLOAT4{ 0.0f, 0.0f, 0.0f, 0.0f });
	normalMaterial_->SetConstBufPS(1,FLOAT4{ lightDir.x, lightDir.y, lightDir.z, 1.0f });
	normalMaterial_->SetConstBufPS(1, FLOAT4{ 3.0f,0.0f,0.0f, 0.0f });
	normalMaterial_->SetConstBufVSMatrix(0,parent_.GetLightViewMatrix());
	normalMaterial_->SetConstBufVSMatrix(1,parent_.GetLightProjectionMatrix());
	normalMaterial_->SetTextureBuf(8, parent_.GetShadowMapTex());
	normalRenderer_->Draw();
}