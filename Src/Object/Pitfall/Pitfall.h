#pragma once
#include "../Actor/ActorBase.h"
#include "../Common/Transform.h"
#include <DxLib.h>

class Pitfall : public ActorBase
{
public:

	enum class STATE
	{
		NONE,
		CREATE,
		ACTIVE,
	};
	Pitfall(const Transform& _pos);

	~Pitfall(void) = default;

	void Init()override;

	void Update()override;

	void Draw()override;

protected:

	STATE state_;
};