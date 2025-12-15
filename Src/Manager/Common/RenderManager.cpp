#include "RenderManager.h"

void RenderManager::Init()
{
}

void RenderManager::Draw()
{
	// モデルレンダラーの描画
	for (const auto& modelRenderer : modelRenderers_)
	{
		modelRenderer->Draw();
	}

	// ピクセルレンダラーの描画
	for (const auto& pixelRenderer : pixelRenderers_)
	{
		pixelRenderer->Draw();
	}
}

void RenderManager::AddPixelRenderer(const std::shared_ptr<PixelRenderer> renderer)
{
	pixelRenderers_.push_back(renderer);
}

void RenderManager::AddModelRenderer(const std::shared_ptr<ModelRenderer> renderer)
{
	modelRenderers_.push_back(renderer);
}
