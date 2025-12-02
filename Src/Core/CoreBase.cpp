#include "CoreBase.h"
#include "../Manager/Common/SceneManager.h"
#include "../Manager/Common/ResourceManager.h"
#include "../Manager/Common/FontManager.h"

CoreBase::CoreBase() :
	resMng_(ResourceManager::GetInstance()),
	scnMng_(SceneManager::GetInstance()),
	fontMng_(FontManager::GetInstance())
{
}

CoreBase::~CoreBase()
{
}

void CoreBase::Load()
{
}

void CoreBase::Init()
{
}

void CoreBase::Update()
{
	UpdateMain();

	UpdateApply();

#ifdef _DEBUG
	UpdateDebug();
#endif
}

void CoreBase::Draw()
{
	DrawMain();

#ifdef _DEBUG
	DrawDebug();
#endif
}

void CoreBase::UpdateMain()
{
}

void CoreBase::UpdateApply()
{
}

void CoreBase::UpdateDebug()
{
}

void CoreBase::DrawMain()
{
}

void CoreBase::DrawDebug()
{
}
