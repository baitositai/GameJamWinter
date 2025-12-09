#pragma once
#include <memory>
#include <functional>
#include <functional>
#include "../CoreBase.h"
#include "../../Common/Image.h"
#include "../../Common/Sprite.h"

class Timer;
class Score;

class GameEnd : public CoreBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameEnd(const Score& score);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameEnd();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了判定を返す
	/// </summary>
	/// <returns>終了判定</returns>
	const bool IsEnd() const { return isEnd_; }


private:

	enum class STATE
	{
		MOVE,
		WAIT,
		RESULT,
	};

	const Score& score_;

	STATE state_;

	Image finish_;

	Image message_;

	Image scoreMes_;

	Sprite playerPlate_;

	Sprite numbers_;

	float moveStep_;

	bool isEnd_;

	bool isDraw_;

	int displayScore_;

	int numberDigit_;

	std::function<void()> update_;

	std::function<void()> draw_;

	std::unordered_map<STATE, std::function<void()>> changeStateMap_;

	std::unique_ptr<Timer> timer_;

	void UpdateMove();
	void UpdateWait();
	void UpdateResult();

	void ChangeState(const STATE state);
	void ChangeStateMove();
	void ChangeStateWait();
	void ChangeStateResult();

	void DrawFinish();
	void DrawResult();

};

