#include <DxLib.h>
#include "../../Utility/AsoUtility.h"
#include "RangeAttack.h"

RangeAttack::RangeAttack(void)
{
}

RangeAttack::~RangeAttack(void)
{
}

void RangeAttack::Init(void)
{
	// モデルの位置設定
	lightningPos_ = VGet(0.0f, 0.0f, 0.0f);

	// モデルの角度
	lightningAngles_ = { 0.0f, 0.0f, 0.0f };
	lightningLocalAngles_ = { 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f };

	// モデルの大きさ設定
	lightningScales_ = { 1.0f, 1.0f, 1.0f };

	// 衝突判定用半径
	lightningCollisionRadius_ = LIGHTNING_RADIUS;

	// 範囲攻撃の生存期間
	cntLightningAlive_ = 100;

	isLightningAlive_ = false;

}

void RangeAttack::Update(void)
{

}

void RangeAttack::Draw(void)
{
	if (isLightningAlive_)
	{
		DrawSphere3D(lightningPos_, lightningCollisionRadius_, 50, 0x00ff00, 0x00ff00, true);
	}

}

void RangeAttack::Release(void)
{

}

float RangeAttack::GetLightningCollisionRadius(void) const
{
	return lightningCollisionRadius_;
}

const VECTOR& RangeAttack::GetLightningPos(void) const
{
	return lightningPos_;
}

void RangeAttack::SetLightningPos(const VECTOR& lightningPos)
{
	lightningPos_ = lightningPos;
}

const bool RangeAttack::IsLightningAlive(void) const
{
	return isLightningAlive_;
}

const bool RangeAttack::GetLightningAlive(void) const
{
	return isLightningAlive_;
}

void RangeAttack::SetLightningAlive(bool isLightningAlive)
{
	isLightningAlive_ = isLightningAlive;
}
