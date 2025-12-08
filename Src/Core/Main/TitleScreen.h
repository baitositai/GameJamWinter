#pragma once
#include <functional>
#include "../CoreBase.h"
#include "../../Common/Image.h"
#include "../../Common/Sprite.h"

class InputManager;

class TitleScreen : public CoreBase
{
public:

	enum class STATE
	{
		MAIN,
		UI_MOVE,
		NUM_SELECT,

	};

	TitleScreen();
	~TitleScreen() override;

	void Init() override;
	void Update() override;
	void Draw() override;

	const int GetPlayerNum() const { return playerNum_; }

private:

	InputManager& inputMng_;

	float step_;

	int playerNum_;
	int isRev_;
	float alpha_;
	bool isEnd_;

	Image imgLogo_;
	Image imgPush_;
	Sprite sprNumbers_;
	Image imgSelect_;

	STATE state_;

	std::unordered_map<STATE, std::function<void()>> changeStateMap_;

	std::function<void()> update_;

	void ChangeState(const STATE state);
	void ChangeStateMain();
	void ChangeStateUiMove();
	void ChangeStateNumSelect();

	void UpdateMain();
	void UpdateUiMove();
	void UpdateNumSelect();
};