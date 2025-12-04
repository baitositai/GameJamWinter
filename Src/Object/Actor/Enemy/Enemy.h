#pragma once
#include "../ActorBase.h"
class Enemy :
    public ActorBase
{
public:

    Enemy(void);
    ~Enemy(void);

protected:

	// トランスフォームの初期設定
	void InitTransform()override;

	// メインの更新処理
	void UpdateBody()override;

	// 処理の適用
	void UpdateApply()override;

	// メインの描画
	void DrawMain()override;

	// 半透明の描画
	void DrawTranslucent()override;

	// UIの描画
	void DrawUI()override;

	// コライダーの追加
	void AddCollider()override;

	// デバッグ時の更新
	void DebugUpdate()override;

	// デバッグ時の描画
	void DebugDraw()override;

};

