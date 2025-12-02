#include <DxLib.h>
#include <random>
#include <chrono>
#include "../../Manager/Common/SceneManager.h"
#include "../../Application.h"
#include "GlitchScreen.h"

// 乱数生成器
std::mt19937 mt(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));

GlitchScreen::GlitchScreen() :
	scnMng_(SceneManager::GetInstance())
{
	effectScreen_ = -1;
}

GlitchScreen::~GlitchScreen()
{
	DeleteGraph(effectScreen_);
}

void GlitchScreen::Init()
{
	effectScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
}

void GlitchScreen::Draw()
{
	// メインスクリーンの取得
	int mainScreen = scnMng_.GetMainScreen();

	// 現在の描画先を保存
	int originalTargetScreen = GetDrawScreen();

	SetDrawScreen(effectScreen_);
	DrawGraph(0, 0, mainScreen, FALSE); // 元の画面の内容をコピー

	// 描画先を元の画面に戻す
	SetDrawScreen(mainScreen);

	// 描画モードを通常に戻す（グリッチ描画前に設定が必要な場合がある）
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	// 画面全体を一旦、元の赤色で塗りつぶす (ズレた部分の背景として)
	// これにより、ズレた部分が真っ黒にならず、元の赤色が背景になる
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, GetColor(255, 0, 0), TRUE);

	// 水平方向へのブロック状のズレ
	int y = 0;
	while (y < Application::SCREEN_SIZE_Y)
	{
		int lineHeight = GLITCH_LINE_HEIGHT_MIN + (mt() % (GLITCH_LINE_HEIGHT_MAX - GLITCH_LINE_HEIGHT_MIN + 1));
		if (y + lineHeight > Application::SCREEN_SIZE_Y) {
			lineHeight = Application::SCREEN_SIZE_Y - y;
		}

		if (mt() % GLITCH_DENSITY == 0) // 一定の確率でグリッチを発生させる
		{
			int offsetX = (mt() % (GLITCH_OFFSET_MAX * 2 + 1)) - GLITCH_OFFSET_MAX; // -GLITCH_OFFSET_MAX から +GLITCH_OFFSET_MAX

			// 描画元の矩形 (コピーした画面のy座標と高さ)
			int srcX = 0;
			int srcY = y;
			int srcW = Application::SCREEN_SIZE_X;
			int srcH = lineHeight;

			// 描画先の矩形 (offsetXだけずらす)
			int destX = offsetX;
			int destY = y;

			// コピーした一時画面の該当部分を、ずらして描画
			DrawRectGraph(destX, destY, srcX, srcY, srcW, srcH, effectScreen_, FALSE);

			// ズレた部分に、わずかに異なる色やノイズを重ねる (オプション)
			// 例: 青みがかったノイズを重ねて色の分離感を出す
			if (mt() % 2 == 0) { // 50%の確率でノイズを重ねる
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64); // 半透明で重ねる
				int noiseColorR = 255 - (mt() % GLITCH_COLOR_MAGNITUDE);
				int noiseColorG = mt() % GLITCH_COLOR_MAGNITUDE;
				int noiseColorB = mt() % GLITCH_COLOR_MAGNITUDE;
				DrawBox(destX, destY, destX + srcW, destY + srcH, GetColor(noiseColorR, noiseColorG, noiseColorB), TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255); // 元に戻す
			}
		}
		else
		{
			// グリッチしない場合は、元の位置に描画
			DrawRectGraph(0, y, 0, y, Application::SCREEN_SIZE_X, lineHeight, effectScreen_, FALSE);
		}

		y += lineHeight;
	}

	// ランダムなピクセルノイズや小さなブロックノイズを重ねる
	SetDrawBlendMode(DX_BLENDMODE_ADD, 128); // 加算ブレンドでノイズを強調
	for (int i = 0; i < NOISE_PIXEL_COUNT; ++i)
	{
		int px = mt() % Application::SCREEN_SIZE_X;
		int py = mt() % Application::SCREEN_SIZE_Y;
		int noiseSize = 1 + (mt() % 5); // ノイズのサイズをランダムに
		int noiseColorR = 100 + (mt() % 155);
		int noiseColorG = mt() % 200;
		int noiseColorB = mt() % 200;
		DrawBox(px, py, px + noiseSize, py + noiseSize, GetColor(noiseColorR, noiseColorG, noiseColorB), TRUE);
	}
	
	// 元に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255); 

	// 描画先を元の画面に戻す (万が一関数呼び出し側で変更されていることを考慮)
	SetDrawScreen(originalTargetScreen);
}