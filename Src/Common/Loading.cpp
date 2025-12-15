#include <DxLib.h>
#include "../Application.h"
#include "../Manager/Common/SceneManager.h"
#include "../Manager/Common/ResourceManager.h"
#include "../Manager/Common/SoundManager.h"
#include "../Manager/Common/FontManager.h"
#include "../Utility/UtilityCommon.h"
#include "Loading.h"

void Loading::Init()
{
	//// シーン内のリソースを読み込む
	//ResourceManager::GetInstance().SceneChangeResource(static_cast<int>(SceneManager::GetInstance().GetSceneID()));

	//// サウンドのリソースの切り替え
	//SoundManager::GetInstance().SceneChangeResources();

	//ローディング用文字列設定
	const std::wstring& fontName = ResourceManager::GetInstance().GetFontName("fontHanazome");
	constexpr int FONT_SIZE = 32;
	loadingString_.fontHandle = FontManager::GetInstance().CreateMyFont(fontName, FONT_SIZE, 0);
	loadingString_.color = UtilityCommon::BLACK;
	loadingString_.pos = { LOADING_STRING_POS_X, LOADING_STRING_POS_Y };
	loadingString_.string = L"読み込んでま～す";
}

void Loading::Update()
{
	// ローディング経過時間更新
	bool loadTimeOver = UtilityCommon::IsTimeOver(loadingTime_, LOADING_TIME);

	//ロードが完了したか判断
	if (GetASyncLoadNum() == 0 && loadTimeOver)
	{
		//非同期処理を無効にする
		SetUseASyncLoadFlag(false);		
		
		// 非同期読み込みの終了判定
		isLoading_ = false;
	}
}

void Loading::Draw()
{
	// NowLoadingの描画
	DrawNowLoading();
}

void Loading::StartASyncLoad() 
{
	// ローディング中フラグを立てる
	isLoading_ = true;

	// 非同期読み込みを有効にする
	//SetUseASyncLoadFlag(true);

	// ローディング時間の初期化
	loadingTime_ = 0.0f;
}

void Loading::DrawNowLoading(void)
{
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0xffffff, true);

	//ロード中
	auto time = SceneManager::GetInstance().GetTotalTime();
	int count = static_cast<int>(time / COMMA_TIME);
	count %= COMMA_MAX_NUM;

	loadingString_.string = L"読み込んでま～す";
	std::wstring dotStr = L".";

	//テキストの設定
	for (int i = 0; i < count; i++)
	{
		loadingString_.string += dotStr;
	}

	//文字の描画
	loadingString_.Draw();
}

Loading::Loading()
{
	loadingTime_ = 0.0f;
	isLoading_ = false;
}

Loading::~Loading()
{
}