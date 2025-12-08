#pragma once
#include <functional>
#include "../CoreBase.h"
#include "../../Common/Image.h"

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

private:

	InputManager& inputMng_;

	int isRev_;
	float alpha_;

	Image imgLogo_;
	Image imgPush_;

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