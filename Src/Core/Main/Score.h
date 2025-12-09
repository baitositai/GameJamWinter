#pragma once
#include <unordered_map>
#include "../CoreBase.h"
#include "../../Common/CharacterString.h"

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
	~Score() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 各プレイヤーのスコアを設定
	/// </summary>
	/// <param name="playerNo">プレイヤーの番号</param>
	/// <param name="score">スコア量</param>
	void SetScore(const int playerNo, const int score);

	/// <summary>
	/// スコアを返す
	/// </summary>
	/// <returns></returns>
	const std::vector<int>& GetScores() const { return scores_; }

	/// <summary>
	/// プレイヤー人数の設定
	/// </summary>
	/// <param name="playerNum"></param>
	void SetPlayerNum(const int playerNum);

private:

	static constexpr int FONT_SIZE = 48;
	static constexpr int FONT_THINESS = 3;

	int font_;

	// プレイヤースコアの管理マップ
	std::vector<int> scores_;

	std::vector<CharacterString> texts_;
};

