#pragma once
#include "SceneBase.h"
#include "../Common/CharacterString.h"
#include "../Common/Image.h"

class PostEffectCracks;

class SceneTitle : public SceneBase
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneTitle();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SceneTitle();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

private:

	// エフェクト用スクリーン
	int effectScreen_;

	// ポストエフェクト
	std::unique_ptr<PostEffectCracks> postEffect_;

	// テキスト
	CharacterString text_;

	// タイトル画像
	Image imgTitle_;

	// 更新関数
	void NormalUpdate() override;

	// 描画関数
	void NormalDraw() override;

	// 処理の変更
	void ChangeNormal() override;
};