#include <DxLib.h>
#include "Shadow.h"

Shadow::Shadow()
{
	shadowMapHandle_ = -1;
}

Shadow::~Shadow()
{
	// シャドウマップの削除
	DeleteShadowMap(shadowMapHandle_);
}

void Shadow::Init()
{
	// シャドウマップのハンドルを生成
	shadowMapHandle_ = MakeShadowMap(SHADOW_SCALE_X, SHADOW_SCALE_Y);

	// シャドウマップが想定するライトの方向を設定
	SetShadowMapLightDirection(shadowMapHandle_, GetLightDirection());

	// シャドウマップに描画する範囲を設定
	SetShadowMapDrawArea(shadowMapHandle_, SHADOW_MAP_MIN_AREA, SHADOW_MAP_MAX_AREA);
}

void Shadow::SetUp()
{
	// シャドウマップへの描画の準備
	ShadowMap_DrawSetup(shadowMapHandle_);
}

void Shadow::Terminate()
{
	// シャドウマップへの描画を終了
	ShadowMap_DrawEnd();
}