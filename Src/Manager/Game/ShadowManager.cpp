#include <DxLib.h>
#include "../../Render/ModelRenderer.h"
#include "../Common/SceneManager.h"
#include "ShadowManager.h"

ShadowManager::ShadowManager()
{
	shadowMapScreen_ = -1;
}

ShadowManager::~ShadowManager()
{
	DeleteGraph(shadowMapScreen_);
}

void ShadowManager::Init()
{
	// 影用深度記録画像の作成
	shadowMapScreen_ = MakeScreen(4096, 4096, FALSE);
}

void ShadowManager::Draw()
{
	int i;
	VECTOR LightDirection;
	VECTOR LightPosition;
	VECTOR LightTarget;


	// 描画先を影用深度記録画像にする
	SetDrawScreen(shadowMapScreen_);

	// 影用深度記録画像を真っ白にクリア
	SetBackgroundColor(255, 255, 255);
	ClearDrawScreen();
	SetBackgroundColor(0, 0, 0);


	// カメラのタイプを正射影タイプにセット、描画範囲も指定
	SetupCamera_Ortho(13250.0f);

	// 描画する奥行き範囲をセット
	SetCameraNearFar(10.0f, 13050.0f);

	// カメラの向きはライトの向き
	LightDirection = GetLightDirection();

	// カメラの位置と注視点はステージ全体が見渡せる位置
	LightTarget.x = 3620.0f;
	LightTarget.y = 0.0f;
	LightTarget.z = 3830.0f;
	LightPosition = VAdd(LightTarget, VScale(LightDirection, -12400.0f));
	SetCameraPositionAndTarget_UpVecY(LightPosition, LightTarget);

	//// 設定したカメラのビュー行列と射影行列を取得しておく
	//LightCamera_ViewMatrix = GetCameraViewMatrix();
	//LightCamera_ProjectionMatrix = GetCameraProjectionMatrix();

	for (const auto& renderer : renderers_)
	{
		renderer->Draw();
	}

	// 描画先を裏画面に戻す
	SetDrawScreen(SceneManager::GetInstance().GetMainScreen());
}

void ShadowManager::Add(const std::shared_ptr<ModelRenderer>& renderer)
{
	renderers_.push_back(renderer);
}
