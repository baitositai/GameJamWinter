#include "../../Manager/Common/SceneManager.h"
#include "../../Manager/Common/Camera.h"
#include "../../Utility/Utility3D.h"
#include "../../Utility/UtilityCommon.h"
#include "ControllerCamera.h"

ControllerCamera::ControllerCamera() :
	camera_(mainCamera),
	scnMng_(SceneManager::GetInstance())
{
	step_ = 0.0f;
	transitionTime_ = 0.0f;
	isEnd_ = false;
	startCameraPos_ = Utility3D::VECTOR_ZERO;
	startTargetPos_ = Utility3D::VECTOR_ZERO;
	goalCameraPos_ = Utility3D::VECTOR_ZERO;
	goalTargetPos_ = Utility3D::VECTOR_ZERO;
}

ControllerCamera::~ControllerCamera()
{
}

void ControllerCamera::Init()
{
	step_ = 0.0f;
	transitionTime_ = 0.0f;
	isEnd_ = false;
	startCameraPos_ = Utility3D::VECTOR_ZERO;
	startTargetPos_ = Utility3D::VECTOR_ZERO;
	goalCameraPos_ = Utility3D::VECTOR_ZERO;
	goalTargetPos_ = Utility3D::VECTOR_ZERO;
}

void ControllerCamera::Update()
{
	// 終了済みの場合
	if (isEnd_)
	{
		// 処理を飛ばす
		return;
	}

	// ステップ更新
	step_ += scnMng_.GetDeltaTime();

	// 進行度の計算
	float t = step_ / transitionTime_;

	// 進行度が終了値を超えている場合
	if (t >= 1.0f)
	{
		t = 1.0f;		// 値を固定
		isEnd_ = true;	// 終了判定を立てる
	}

	// イーズアウトでスピードを決定
	float speed = 1.0f - t;

	// 3次間数で計算
	float easeOutSpeed = 1.0f - speed * speed * speed;

	// 位置・ターゲット位置の線形補間
	VECTOR currentPos = UtilityCommon::Lerp(startCameraPos_, goalCameraPos_, easeOutSpeed);
	VECTOR currentTarget = UtilityCommon::Lerp(startTargetPos_, goalTargetPos_, easeOutSpeed);

	// 角度の線形補間の計算
	float currentAngle = UtilityCommon::Lerp(startDeg_, goalDeg_, easeOutSpeed);

	// RADに変換
	float rad = UtilityCommon::Deg2RadF(currentAngle);

	// マトリックス変換
	MATRIX rotMat = MGetRotAxis(rotAxis_, rad);

	// カメラ回転処理
	VECTOR currentUpVec = VTransform(startCameraUpVec_, rotMat);

	// カメラの設定
	camera_.SetPos(currentPos);
	camera_.SetTargetPos(currentTarget);
	camera_.SetCameraUpVector(currentUpVec);
}

void ControllerCamera::Set(const VECTOR& goalCameraPos, const VECTOR& goalTargetPos, const VECTOR& rotAxis, const float goalDeg, const float transitionTime)
{	
	// カメラを固定する
	camera_.ChangeMode(Camera::MODE::FIXED_POINT);

	// 時間の設定
	transitionTime_ = transitionTime;

	// 回転軸の設定
	rotAxis_ = rotAxis;

	// 開始角度の取得
	startDeg_ = GetStartRotDeg();

	// 終了位置の設定
	goalCameraPos_ = goalCameraPos;
	goalTargetPos_ = goalTargetPos;
	goalDeg_ = startDeg_ + goalDeg;

	// 開始位置の設定
	startCameraPos_ = camera_.GetPos();
	startTargetPos_ = camera_.GetTargetPos();
	startCameraUpVec_ = camera_.GetCameraUpVector();

	// 初期化
	isEnd_ = false;
	step_ = 0.0f;
}

float ControllerCamera::GetStartRotDeg() const
{
	// カメラ上方向を取得
	VECTOR upVec = GetCameraUpVector();

	// ラジアンの取得
	const float rad = atan2f(-upVec.x, upVec.y);

	// ラジアンを度に変換して返す
	return UtilityCommon::Deg2RadF(rad);
}