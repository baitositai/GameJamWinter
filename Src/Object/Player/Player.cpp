#include "Player.h"
#include "../Pitfall/Pitfall.h"

Player::Player(const Transform& _transform)
{
    transform_ = _transform;
}

void Player::Init(void)
{
}

void Player::Update(void)
{
}

void Player::Draw(void)
{
    MV1DrawModel(transform_.modelId);
}

const std::vector<std::weak_ptr<Pitfall>> Player::GetPitfalls(void)
{
    
}

void Player::SetPitfall(const Transform& _transform)
{
    Pitfall* pitfall = new Pitfall(_transform);
    pitfalls_.emplace_back(pitfall);
}
