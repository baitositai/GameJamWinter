#include<DxLib.h>
#include<EffekseerForDXLib.h>
#include<Effekseer.h>
#include "ControllerEffect.h"

ControllerEffect::ControllerEffect(const int handle)
{
}

ControllerEffect::~ControllerEffect()
{
}

void ControllerEffect::Play(const VECTOR& pos, const Quaternion& quaRot, const VECTOR scl, const float speed, const bool isLoop)
{
}

void ControllerEffect::Stop()
{
}

bool ControllerEffect::IsEnd() const
{
	//çƒê∂Ç™èIÇÌÇ¡ÇƒÇ¢ÇÈèÍçá
	if (IsEffekseer3DEffectPlaying(handle_) == -1)
	{
		// èIóπîªíË
		return true;
	}

	// é¿çsíÜ
	return false;
}

void ControllerEffect::Update(const VECTOR& pos, const Quaternion& quaRot, const VECTOR scl, const float speed)
{
}

void ControllerEffect::SetPos(const VECTOR& pos)
{
	pos_ = pos;
	SetPosPlayingEffekseer3DEffect(playId_, pos_.x, pos_.y, pos_.z);
}

void ControllerEffect::SetQuaternion(const Quaternion& quaRot)
{
	quaRot_ = quaRot_;
	const VECTOR rot = quaRot_.ToEuler();
	SetRotationPlayingEffekseer3DEffect(playId_, rot.x, rot.y, rot.z);
}

void ControllerEffect::SetScale(const VECTOR& scl)
{
	scl_ = scl;
	SetScalePlayingEffekseer3DEffect
}

void ControllerEffect::SetSpeed(const float speed)
{
}
