#pragma once
#include <unordered_map>
#include "../CoreBase.h"

class Score : public CoreBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Score();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Score();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 各プレイヤーのスコアを設定
	/// </summary>
	/// <param name="playerNo">プレイヤーの番号</param>
	/// <param name="score">スコア量</param>
	void SetScore(const int playerNo, const int score);

	/// <summary>
	/// スコア管理を返す
	/// </summary>
	/// <returns></returns>
	const std::unordered_map<int, int>& GetScoreMap() const { return playerScoreMap_; }

private:

	// プレイヤースコアの管理マップ
	std::unordered_map<int, int> playerScoreMap_;
};

