#include "../../../Scene/SceneGame.h"
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
	// 基底クラスの処理
	CharacterBase::Init();

	// 初期位置に応じて処理の反映
	if (transform_.pos.x > SceneGame::MOVE_LIMIT_MAX_X)
	{
		movePowX_ = -MOVE_SPEED; 
		checkGoalMove_ = std::bind(&Enemy::IsGoalByMoveLeft, this);
	}
	else
	{
		movePowX_ = MOVE_SPEED;
		checkGoalMove_ = std::bind(&Enemy::IsGoalByMoveRight, this);
	}
}

void Enemy::Draw()
{
	CharacterBase::Draw();
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