#pragma once
#include <memory>
#include <functional>
#include <functional>
#include "../CoreBase.h"
#include "../../Common/Image.h"

class Timer;

class GameStart : public CoreBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameStart();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameStart();
	
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
		EXIT,
	};

	STATE state_;

	Image start_;

	float moveStep_;

	bool isEnd_;

	std::function<void()> update_;

	std::unordered_map<STATE, std::function<void()>> changeStateMap_;

	std::unique_ptr<Timer> timer_;

	void UpdateMove();
	void UpdateWait();
	void UpdateExit();

	void ChangeState(const STATE state);
	void ChangeStateMove();
	void ChangeStateWait();
	void ChangeStateExit();

};

