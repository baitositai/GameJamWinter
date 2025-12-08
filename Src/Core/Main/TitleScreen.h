#pragma once
#include "../CoreBase.h"
#include "../../Common/Image.h"

class TitleScreen : public CoreBase
{
public:

	enum class STATE
	{
		MAIN,
		NUM_SELECT,

	};

	TitleScreen();
	~TitleScreen() override;

	void Init() override;
	void Update() override;
	void Draw() override;

private:

	int isRev_;
	float alpha_;

	Image imgLogo_;
	Image imgPush_;
};

