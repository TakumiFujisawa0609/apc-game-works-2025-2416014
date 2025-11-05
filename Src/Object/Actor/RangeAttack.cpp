#include <DxLib.h>
#include "../../Manager/InputManager.h"
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

	// 範囲攻撃エフェクト画像のロード
	LoadDivGraph(
		(Application::PATH_IMAGE + "Lightning.png").c_str(),
		NUM_SPRITE_ALL,
		NUM_SPRITE_X, NUM_SPRITE_Y,
		SIZE_XSPRITE_X, SIZE_XSPRITE_Y,
		imgs_);

	// 通常攻撃エフェクト画像のロード
	LoadDivGraph(
		(Application::PATH_IMAGE + "PlayerSlash.png").c_str(),
		SLASH_NUM_SPRITE_ALL,
		SLASH_NUM_SPRITE_X, SLASH_NUM_SPRITE_Y,
		SLASH_SIZE_SPRITE_X, SLASH_SIZE_SPRITE_X,
		slashImgs_);

	// モデルの位置設定
	lightningPos_ = AsoUtility::VECTOR_ZERO;

	slashPos_ = AsoUtility::VECTOR_ZERO;

	// モデルの角度
	lightningAngles_ = { 0.0f, 0.0f, 0.0f };
	lightningLocalAngles_ = { 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f };

	// モデルの大きさ設定
	lightningScales_ = { 1.0f, 1.0f, 1.0f };

	// 衝突判定用半径
	lightningCollisionRadius_ = LIGHTNING_RADIUS;

	// 範囲攻撃の生存期間
	lightningPoint_ = LIGHTNING_POINT;

	//isLightningAlive_ = false;

	cntAnimation_ = 0;
	cntSlashAnimation_ = 0;

	isSlashAlive_ = false;

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

	// アニメーション更新
	if (isSlashAlive_)
	{
		cntSlashAnimation_++;
		if (cntSlashAnimation_ >= SLASH_NUM_SPRITE_ALL)
		{
			cntSlashAnimation_ = 0;
		}
	}

}

void RangeAttack::Draw(void)
{
	if (isLightningAlive_)
	{
		////デバッグ用の球体描画
		//DrawSphere3D(lightningPos_, lightningCollisionRadius_, 50, 0xffff00, 0xffff00, true);

		// ビルボード描画
		int img = imgs_[cntAnimation_];
		DrawBillboard3D(lightningPos_, 0.5f, 0.5f, IMG_SCALE, 0.0f, img, true);

	}

	if (isSlashAlive_)
	{
		int slashImg = slashImgs_[cntSlashAnimation_];
		DrawBillboard3D(slashPos_, 0.5f, 0.5f, SLASH_IMG_SCALE, 0.0f, slashImg, true);
	}

	//DrawFormatString(150, 170, 0xffffff, "生存中: %s", isLightningAlive_ ? "YES" : "NO");
	DrawFormatString(0, 70, 0xffffff, "MP: %d", lightningPoint_);

}

void RangeAttack::Release(void)
{
	// エフェクト画像のメモリの解放
	for (int i = 0; i < NUM_SPRITE_ALL; i++)
	{
		DeleteGraph(imgs_[i]);
	}

	// エフェクト画像のメモリの解放
	for (int j = 0; j < SLASH_NUM_SPRITE_ALL; j++)
	{
		DeleteGraph(slashImgs_[j]);
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

const bool RangeAttack::GetLightningAlive(void) const
{
	return isLightningAlive_;
}

void RangeAttack::SetLightningAlive(bool isLightningAlive)
{
	isLightningAlive_ = isLightningAlive;
}

int RangeAttack::GetLightningPoint(void) const
{
	return lightningPoint_;
}

void RangeAttack::SetLightningPoint(int lightningPoint)
{
	lightningPoint_ = lightningPoint;
}

void RangeAttack::RangeAttackTime(void)
{
	if (isLightningAlive_)
	{
		lightningPoint_--;
		if (lightningPoint_ <= 0)
		{
			//isLightningAlive_ = false;
		}
	}
}

const VECTOR& RangeAttack::GetSlashPos(void) const
{
	return slashPos_;
}

void RangeAttack::SetSlashPos(const VECTOR& slashPos)
{
	slashPos_ = slashPos;
}

const bool RangeAttack::GetSlashAlive(void) const
{
	return isSlashAlive_;
}

void RangeAttack::SetSlashAlive(bool isSlashAlive)
{
	isSlashAlive_ = isSlashAlive;
}
