#include<DxLib.h>
#include<EffekseerForDXLib.h>
#include<Effekseer.h>
#include "../../Utility/Utility3D.h"
#include "ControllerEffect.h"

ControllerEffect::ControllerEffect(const int handle) :
	resourceId_(handle)
{
	playId_ = -1;
	isLoop_ = false;
	pos_ = Utility3D::VECTOR_ZERO;
	scl_ = Utility3D::VECTOR_ZERO;
	quaRot_ = Quaternion();
	speed_ = 0.0f;
}

ControllerEffect::~ControllerEffect()
{
}

void ControllerEffect::Play(const VECTOR& pos, const Quaternion& quaRot, const VECTOR scl, const float speed, const bool isLoop)
{
	// 再生IDの取得
	playId_ = PlayEffekseer3DEffect(resourceId_);

	// ループ設定
	isLoop_ = isLoop;

	// 各種パラメーター設定
	SetPos(pos);
	SetQuaternion(quaRot);
	SetScale(scl);
	SetSpeed(speed);
}

void ControllerEffect::Stop()
{
	// ストップ
	StopEffekseer3DEffect(playId_);
}

bool ControllerEffect::IsEnd() const
{
	//再生が終わっている場合
	if (IsEffekseer3DEffectPlaying(playId_) == -1)
	{
		// 終了判定
		return true;
	}

	// 実行中
	return false;
}

void ControllerEffect::Update(const VECTOR& pos, const Quaternion& quaRot, const VECTOR scl, const float speed)
{

	//再生が終わった場合
	if (IsEffekseer3DEffectPlaying(playId_) == -1)
	{
		//ループする場合
		if (isLoop_)
		{
			// 再生
			Play(pos, quaRot, scl, speed);
			return;
		}
	}

	// 各種パラメーター設定
	SetPos(pos);
	SetQuaternion(quaRot);
	SetScale(scl);
	SetSpeed(speed);
}

void ControllerEffect::SetPos(const VECTOR& pos)
{
	pos_ = pos;
	SetPosPlayingEffekseer3DEffect(playId_, pos_.x, pos_.y, pos_.z);
}

void ControllerEffect::SetQuaternion(const Quaternion& quaRot)
{
	quaRot_ = quaRot;
	const VECTOR rot = quaRot_.ToEuler();
	SetRotationPlayingEffekseer3DEffect(playId_, rot.x, rot.y, rot.z);
}

void ControllerEffect::SetScale(const VECTOR& scl)
{
	scl_ = scl;
	SetScalePlayingEffekseer3DEffect(playId_, scl_.x, scl_.y, scl_.z);
}

void ControllerEffect::SetSpeed(const float speed)
{
	speed_ = speed;
	SetSpeedPlayingEffekseer3DEffect(playId_, speed_);
}
