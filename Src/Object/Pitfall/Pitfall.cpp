#include "Pitfall.h"
#include <DxLib.h>
#include "../Actor/ActorBase.h"
#include "../Common/Transform.h"
#include "../Manager/Common/ResourceManager.h"

Pitfall::Pitfall(const Transform& _pos):
	ActorBase::ActorBase()
{
	transform_ = _pos;
	Init();
}

void Pitfall::Init()
{
	
}

void Pitfall::Update()
{
	switch (state_)
	{
		case STATE::ACTIVE:
		break;

		case STATE::CREATE:
		break;
	}
}

void Pitfall::Draw()
{
	const float RADIUS = 30.0f;
	const unsigned int COLOR = 0x0;
	DrawSphere3D(transform_.pos, RADIUS, 16, COLOR, COLOR, true);
}
