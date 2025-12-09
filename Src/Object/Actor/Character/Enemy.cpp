#include "../../../Scene/SceneGame.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../../Manager/Common/ResourceManager.h"
#include "../../Common/AnimationController.h"
#include "Enemy.h"

namespace
{
	static constexpr float RIGHT_GOAL = SceneGame::MOVE_LIMIT_MAX_X + 500;
	static constexpr float LEFT_GOAL = SceneGame::MOVE_LIMIT_MIN_X - 500;
}

Enemy::Enemy(const VECTOR& initPos) :
	CharacterBase(initPos)
{
	movePowX_ = 0.0f;
}

Enemy::~Enemy()
{
}

void Enemy::Init()
{
	// モデルリソースの設定
	transform_.SetModel(resMng_.GetHandle("enemy"));

	// 基底クラスの処理
	CharacterBase::Init();

	// 初期位置に応じて処理の反映
	if (transform_.pos.x > SceneGame::MOVE_LIMIT_MAX_X)
	{
		movePowX_ = -MOVE_SPEED; 
		transform_.quaRotLocal = Quaternion::Euler({ 0.0f,UtilityCommon::Deg2RadF(90.0f), 0.0f });
		checkGoalMove_ = std::bind(&Enemy::IsGoalByMoveLeft, this);
	}
	else
	{
		movePowX_ = MOVE_SPEED;
		transform_.quaRotLocal = Quaternion::Euler({ 0.0f,UtilityCommon::Deg2RadF(90.0f), 0.0f });
		checkGoalMove_ = std::bind(&Enemy::IsGoalByMoveRight, this);
	}
}

void Enemy::Draw()
{
	CharacterBase::Draw();
}

void Enemy::InitAnimation()
{
	anim_ = std::make_shared<AnimationController>(transform_.modelId);

	for (int i = 0; i < static_cast<int>(ANIM_TYPE::MAX); i++)
	{
		anim_->Add(i, 30.0f, transform_.modelId);
	}

	// 初期アニメーション再生
	anim_->Play(static_cast<int>(ANIM_TYPE::WALK));
}

void Enemy::UpdateAction()
{
	// 移動処理
	transform_.pos.x += movePowX_;

	// 目的地にたどり着いた場合
	if (checkGoalMove_())
	{
		isDelete_ = true;
	}
}

void Enemy::UpdateFall()
{
	// 基底クラスの処理を実行
	CharacterBase::UpdateFall();

	// Y値が一定以下になったら削除
	if (transform_.pos.y < DELETE_LILNE_Y)
	{
		isDelete_ = true;
	}
}

bool Enemy::IsGoalByMoveRight() const
{
	return transform_.pos.x > RIGHT_GOAL;
}

bool Enemy::IsGoalByMoveLeft() const
{
	return transform_.pos.x < LEFT_GOAL;
}