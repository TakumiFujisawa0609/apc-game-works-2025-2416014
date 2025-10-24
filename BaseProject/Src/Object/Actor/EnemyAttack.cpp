#include <DxLib.h>
#include "../../Manager/InputManager.h"
#include "../../Utility/AsoUtility.h"
#include "../../Application.h"
#include "EnemyAttack.h"

EnemyAttack::EnemyAttack(void)
{
}

EnemyAttack::~EnemyAttack(void)
{
}

void EnemyAttack::Init(void)
{
	// モデルの位置設定
	pos_ = AsoUtility::VECTOR_ZERO;

	// 衝突判定用半径
	collisionRadius_ = ENEMY_ATTACK_RADIUS;

	isAlive_ = true;
}

void EnemyAttack::Update(void)
{
}

void EnemyAttack::Draw(void)
{
	if (isAlive_)
	{
		//デバッグ用の球体描画
		DrawSphere3D(pos_, collisionRadius_, 50, 0xff00ff, 0xff00ff, true);

	}

}

void EnemyAttack::Release(void)
{
}

float EnemyAttack::GetCollisionRadius(void) const
{
	return collisionRadius_;
}

const VECTOR& EnemyAttack::GetPos(void) const
{
	return pos_;
}

void EnemyAttack::SetPos(const VECTOR& pos)
{
	pos_ = pos;
}

const bool EnemyAttack::GetAlive(void) const
{
	return isAlive_;
}

void EnemyAttack::SetAlive(bool isAlive)
{
	isAlive_ = isAlive;
}
