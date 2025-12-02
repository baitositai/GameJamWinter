#include "../Application.h"
#include "../Manager/Common/SceneManager.h"
#include "../Manager/Common/ResourceManager.h"
#include "../Render/PixelMaterial.h"
#include "../Render/PixelRenderer.h"
#include "PostEffectCracks.h"

PostEffectCracks::PostEffectCracks()
{
	step_ = 0.0f;
}

PostEffectCracks::~PostEffectCracks()
{
}

void PostEffectCracks::Init()
{
	// マテリアルの生成
	material_ = std::make_unique<PixelMaterial>(resMng_.GetHandle("cracksEffect"), CONST_BUFFER_SIZE);

	// レンダラーの生成
	renderer_ = std::make_unique<PixelRenderer>(*material_);

	// バッファー追加
	material_->AddConstBuf(FLOAT4{ 0.0f,0.0f,0.0f,0.0f });

	// テクスチャ設定
	material_->AddTextureBuf(scnMng_.GetMainScreen());

	// ひび割れのノーマルマップ画像を追加
	material_->AddTextureBuf(resMng_.GetHandle("cracksNormalMap"));

	// 親クラスの初期化処理
	PostEffectBase::Init();
}

void PostEffectCracks::Draw()
{
	// ステップの更新
	step_ += scnMng_.GetDeltaTime();

	// バッファーの更新
	material_->SetConstBuf(0, FLOAT4{ step_, 0.0f, 0.0f, 0.0f });

	// 基底クラスの処理
	PostEffectBase::Draw();
}