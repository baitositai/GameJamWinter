#include "../Application.h"
#include "../Manager/Common/SceneManager.h"
#include "../Render/PixelMaterial.h"
#include "../Render/PixelRenderer.h"
#include "PostEffectBase.h"

PostEffectBase::PostEffectBase()
{
	material_ = nullptr;
	renderer_ = nullptr;
}

PostEffectBase::~PostEffectBase()
{

}

void PostEffectBase::Load()
{
}

void PostEffectBase::Init()
{
	//矩形の設定
	renderer_->MakeSquereVertex(
		Vector2(0, 0),
		Vector2(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y));
}

void PostEffectBase::Draw()
{
	//スクリーンの設定
	material_->SetTextureBuf(0, scnMng_.GetMainScreen());

	// 描画
	renderer_->Draw();
}