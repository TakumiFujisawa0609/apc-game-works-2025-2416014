#include <DxLib.h>
#include "./Enemy.h"
#include "../../Utility/AsoUtility.h"
#include "../../Application.h"
#include "RangeAttack.h"

RangeAttack::RangeAttack(void)
{
}

RangeAttack::~RangeAttack(void)
{
}

void RangeAttack::Init(void)
{

	// エフェクト画像(横)のロード
	LoadDivGraph(
		(Application::PATH_IMAGE + "Lightning.png").c_str(),
		NUM_SPRITE_ALL,
		NUM_SPRITE_X, NUM_SPRITE_Y,
		SIZE_XSPRITE_X, SIZE_XSPRITE_Y,
		imgs_);

	// モデルの位置設定
	lightningPos_ = VGet(0.0f, 400.0f, 0.0f);

	// モデルの角度
	lightningAngles_ = { 0.0f, 0.0f, 0.0f };
	lightningLocalAngles_ = { 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f };

	// モデルの大きさ設定
	lightningScales_ = { 1.0f, 1.0f, 1.0f };

	// 衝突判定用半径
	lightningCollisionRadius_ = LIGHTNING_RADIUS;

	// 範囲攻撃の生存期間
	cntLightning_ = 500;

	isLightningAlive_ = false;

}

void RangeAttack::Update(void)
{
	RangeAttackTime();

	// アニメーション更新
	if (isLightningAlive_)
	{
		cntAnimation_++;
		if (cntAnimation_ >= NUM_SPRITE_ALL)
		{
			cntAnimation_ = 0; 
		}
	}

}

void RangeAttack::Draw(void)
{
	if (!isLightningAlive_)
	{
		return;
	}

	// デバッグ用の球体描画
	DrawSphere3D(lightningPos_, lightningCollisionRadius_, 50, 0xffff00, 0xffff00, true);

	// ビルボード描画
	int img = imgs_[cntAnimation_];
	DrawBillboard3D(lightningPos_, 0.5f, 0.5f, IMG_SCALE, 0.0f, img, true);

	DrawFormatString(150, 150, 0xffffff, "雷時間: %d", cntAnimation_);
	DrawFormatString(150, 170, 0xffffff, "生存中: %s", isLightningAlive_ ? "YES" : "NO");
	DrawFormatString(150, 190, 0xffffff, "生存カウント: %d", cntLightning_);

}

void RangeAttack::Release(void)
{
	// エフェクト画像のメモリの解放
	for (int i = 0; i < NUM_SPRITE_ALL; i++)
	{
		DeleteGraph(imgs_[i]);
	}

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

void RangeAttack::RangeAttackTime(void)
{
	if (isLightningAlive_)
	{
		cntLightning_--;
		if (cntLightning_ <= 0)
		{
			isLightningAlive_ = false;
		}
	}
}
