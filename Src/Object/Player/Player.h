#pragma once
#include "../Actor/ActorBase.h"
#include "../Common/Transform.h"
#include <memory>
#include <vector>
class Pitfall;

class Player : public ActorBase
{
public:

	enum class STATE
	{
		NONE,
		MOVE,
		SET_PITFALL,
		FALL
	};

	Player();

	~Player() = default;

	void Init(void)override;

	void Update(void)override;

	void Draw(void)override;

	bool GetIsPitfall(void) { return isPitfall_; };

protected:

	bool isPitfall_;
};