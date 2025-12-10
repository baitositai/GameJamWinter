#include "../../../Manager/Common/Camera.h"
#include "../../../Manager/Common/SceneManager.h"
#include "../../../Manager/Common/SoundManager.h"
#include "../../../Utility/Utility3D.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../../Scene/SceneGame.h"
#include "../../../Core/Common/Timer.h"
#include "../../../Manager/Common/ResourceManager.h"
#include "../../Common/AnimationController.h"
#include "Player.h"

Player::Player(const VECTOR& initPos, const Input::JOYPAD_NO padNo, const COLOR_F& color) :
    CharacterBase(initPos),
    MY_PAD_NO(padNo),
	COLOR(color)
{
	movedPos_ = Utility3D::VECTOR_ZERO;
	movePow_ = Utility3D::VECTOR_ZERO;
	moveDir_ = Utility3D::VECTOR_ZERO;
	createPitFallPos_ = Utility3D::VECTOR_ZERO;
	playerRotY_ = Quaternion();
	goalQuaRot_ = Quaternion();
	stepRotTime_ = 0.0f;
	moveSpeed_ = 0.0f;
	actionState_ = ACTION_STATE::NONE;
	isCreatePitFall_ = false;
	isInvincible_ = false;

	// 状態遷移関数の登録
	changeActionStateMap_.emplace(ACTION_STATE::NONE, std::bind(&Player::ChangeActionStateNone, this));
	changeActionStateMap_.emplace(ACTION_STATE::MOVE, std::bind(&Player::ChangeActionStateMove, this));
	changeActionStateMap_.emplace(ACTION_STATE::SET_PITFALL, std::bind(&Player::ChangeActionStateSetPitFall, this));
}

Player::~Player()
{
}

void Player::Init(void)
{
	// モデルリソースの設定
	transform_.SetModel(resMng_.GetHandle("player"));

    CharacterBase::Init();

	// リスポン用タイマーの生成
	timer_ = std::make_unique<Timer>(RESPOWN_TIME);
	timer_->InitCountUp();

	// 無敵時間タイマーの生成
	invincibleTimer_ = std::make_unique<Timer>(INVINCIBLE);
	invincibleTimer_->InitCountUp();

	// アクションの初期状態
	ChangeActionState(ACTION_STATE::MOVE);

	// 生成可能状態
	isCreatePitFall_ = true;

	// 初期方向
	moveDir_ = mainCamera.GetQuaRotOutX().GetForward();
}

void Player::Draw(void)
{
	MV1SetDifColorScale(transform_.modelId, COLOR);
	MV1DrawModel(transform_.modelId);
}

void Player::UpdateAction()
{
    actionUpdate_();
}

void Player::UpdateFall()
{
	// 基底クラスの処理
	CharacterBase::UpdateFall();

	// 復帰処理
	if (timer_->CountUp())
	{
		// 初期位置に戻す
		transform_.pos = INIT_POS;

		// 状態遷移
		ChangeState(STATE::ACTION);

		// アクションの状態も移動に変更
		ChangeActionState(ACTION_STATE::MOVE);

		// しばらくの間無敵設定
		isInvincible_ = true;
	}
}

void Player::ChangeStateFall()
{
	CharacterBase::ChangeStateFall();

	anim_->Play(static_cast<int>(ANIM_TYPE::FALL));
}

void Player::ChangeActionState(const ACTION_STATE state)
{
    actionState_ = state;

    changeActionStateMap_[actionState_]();
}

void Player::ChangeActionStateNone()
{
    actionUpdate_ = std::bind(&Player::UpdateNone, this);
}

void Player::ChangeActionStateMove()
{
    actionUpdate_ = std::bind(&Player::UpdateActionMove, this);

	isCreatePitFall_ = true;

	anim_->Play(static_cast<int>(ANIM_TYPE::IDLE));
}

void Player::ChangeActionStateSetPitFall()
{
    actionUpdate_ = std::bind(&Player::UpdateActionSetPitFall, this);

	anim_->Play(static_cast<int>(ANIM_TYPE::SET), false);

	sndMng_.PlaySe(SoundType::SE::SET_PIT_FALL);
}

void Player::InitAnimation()
{
	constexpr float ANIMSPEED = 30.0f;

	anim_ = std::make_shared<AnimationController>(transform_.modelId);

	// アニメーションの追加
	anim_->Add(static_cast<int>(ANIM_TYPE::IDLE), ANIMSPEED, transform_.modelId);
	anim_->Add(static_cast<int>(ANIM_TYPE::WALK), ANIMSPEED, transform_.modelId);
	anim_->Add(static_cast<int>(ANIM_TYPE::SET), ANIMSPEED, transform_.modelId);
	anim_->Add(static_cast<int>(ANIM_TYPE::FALL), ANIMSPEED, transform_.modelId);

	// 初期アニメーション再生
	anim_->Play(static_cast<int>(ANIM_TYPE::IDLE));
}

void Player::UpdateActionMove()
{
	// 移動操作処理
	ProcessMove();

	// 移動制限
	MoveLimit();

	// 生成操作処理
	ProcessCreatePitFall();

	// 回転の反映
	Rotate();

	// 無敵処理
	Invincible();

	// 移動座標の反映
	transform_.pos = movedPos_;

	// 回転の反映
	transform_.quaRot = Quaternion();
	transform_.quaRot = transform_.quaRot.Mult(playerRotY_);
}

void Player::UpdateActionSetPitFall()
{
	if (anim_->IsEnd())
	{
		ChangeActionState(ACTION_STATE::MOVE);
	}
}

void Player::ProcessMove()
{
	auto& ins = InputManager::GetInstance();
	movePow_ = Utility3D::VECTOR_ZERO;			//移動量
	VECTOR dir = Utility3D::VECTOR_ZERO;		//方向
	Quaternion cameraRot = mainCamera.GetQuaRotOutX();
	double rotRad = 0.0f;

	//操作
	if (ins.IsNew(InputManager::TYPE::PLAYER_MOVE_RIGHT, MY_PAD_NO)) { dir = cameraRot.GetLeft(); rotRad = UtilityCommon::Deg2RadD(ROT_DEG_R); }
	else if (ins.IsNew(InputManager::TYPE::PLAYER_MOVE_LEFT, MY_PAD_NO)) { dir = cameraRot.GetRight(); rotRad = UtilityCommon::Deg2RadD(ROT_DEG_L); }
	else if (ins.IsNew(InputManager::TYPE::PLAYER_MOVE_DOWN, MY_PAD_NO)) { dir = cameraRot.GetForward(); rotRad = UtilityCommon::Deg2RadD(ROT_DEG_B); }
	else if (ins.IsNew(InputManager::TYPE::PLAYER_MOVE_UP, MY_PAD_NO)) { dir = cameraRot.GetBack(); rotRad = UtilityCommon::Deg2RadD(ROT_DEG_F); }

	if (!Utility3D::EqualsVZero(dir))
	{
		// 移動処理
		moveSpeed_ = SPEED_MOVE;
		moveDir_ = dir;						//移動方向
		movePow_ = VScale(dir, moveSpeed_);	//移動量

		// 回転処理
		SetGoalRotate(rotRad);

		// アニメーション処理
		anim_->Play((int)ANIM_TYPE::WALK);
	}
	else
	{
		// アニメーション処理
		anim_->Play((int)ANIM_TYPE::IDLE);
	}

	// 移動後座標を設定
	movedPos_ = VAdd(transform_.pos, movePow_);
}

void Player::ProcessCreatePitFall()
{
	auto& ins = InputManager::GetInstance();

	// 特定のキーを入力したかつ生成可能の場合
	if (ins.IsTrgDown(InputManager::TYPE::PLAYER_CREATE_PIT_FALL, MY_PAD_NO) && isCreatePitFall_)
	{
		// 生成不可にする
		isCreatePitFall_ = false;

		// 生成位置までの移動量を計算
		VECTOR movePow = VScale(moveDir_, DISTANCE);

		// 格納
		createPitFallPos_ = VAdd(transform_.pos, movePow);

		// 状態変更
		ChangeActionState(ACTION_STATE::SET_PITFALL);
	}
}

void Player::MoveLimit()
{
	// X方向の移動制限
	if (movedPos_.x > SceneGame::MOVE_LIMIT_MAX_X)
	{
		movedPos_.x = SceneGame::MOVE_LIMIT_MAX_X;
	}
	else if (movedPos_.x < SceneGame::MOVE_LIMIT_MIN_X)
	{
		movedPos_.x = SceneGame::MOVE_LIMIT_MIN_X;
	}

	// Z方向の移動制限
	if (movedPos_.z > SceneGame::MOVE_LIMIT_MAX_Z)
	{
		movedPos_.z = SceneGame::MOVE_LIMIT_MAX_Z;
	}
	else if (movedPos_.z < SceneGame::MOVE_LIMIT_MIN_Z)
	{
		movedPos_.z = SceneGame::MOVE_LIMIT_MIN_Z;
	}
}

void Player::Invincible()
{
	// 無敵の場合
	if (isInvincible_)
	{
		// タイマーを更新
		if (invincibleTimer_->CountUp())
		{
			// 無敵終了
			isInvincible_ = false;
		}
	}
}

void Player::SetGoalRotate(double rotRad)
{
	VECTOR cameraRot = mainCamera.GetAngles();
	Quaternion axis =
		Quaternion::AngleAxis(
			(double)cameraRot.y + rotRad, Utility3D::AXIS_Y);

	// 現在設定されている回転との角度差を取る
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// しきい値
	if (angleDiff > 0.1)
	{
		stepRotTime_ = TIME_ROT;
	}
	goalQuaRot_ = axis;
}

void Player::Rotate()
{	
	stepRotTime_ -= scnMng_.GetDeltaTime();

	// 回転の球面補間
	playerRotY_ = Quaternion::Slerp(
		playerRotY_, goalQuaRot_,
		(TIME_ROT - stepRotTime_) / TIME_ROT);
}