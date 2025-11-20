#include "EnemyAttackBase.h"

EnemyAttackBase::EnemyAttackBase(void)
{
}

EnemyAttackBase::~EnemyAttackBase(void)
{
}

void EnemyAttackBase::Init(void)
{
	// リソースロード
	InitLoad();
	// 初期化後の個別処理
	InitPost();
}

void EnemyAttackBase::Update(void)
{
}

void EnemyAttackBase::Draw(void)
{
}

void EnemyAttackBase::Release(void)
{
}

float EnemyAttackBase::GetCollisionRadius(void) const
{
	return collisionRadius_;
}

const VECTOR& EnemyAttackBase::GetPos(void) const
{
	return pos_;
}

void EnemyAttackBase::SetPos(const VECTOR& pos)
{
	pos_ = pos;
}

const bool EnemyAttackBase::GetAlive(void) const
{
	return isAlive_;
}

void EnemyAttackBase::SetAlive(bool isAlive)
{
	isAlive_ = isAlive;
}
