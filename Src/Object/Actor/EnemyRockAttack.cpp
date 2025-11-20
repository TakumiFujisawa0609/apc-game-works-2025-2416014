	#include <DxLib.h>
	#include "../../Utility/AsoUtility.h"
	#include "../../Application.h"
	#include "EnemyRockAttack.h"

	EnemyRockAttack::EnemyRockAttack(void)
	{
	}

	EnemyRockAttack::~EnemyRockAttack(void)
	{
	}

	void EnemyRockAttack::Update(void)
	{
		// アニメーション更新
		if (isAlive_)
		{
			cntAnimation_++;
			if (cntAnimation_ >= NUM_SPRITE_ALL)
			{
				cntAnimation_ = 0;
			}
		}
	}

	void EnemyRockAttack::Draw(void)
	{
		if (isAlive_)
		{
			////デバッグ用の球体描画
			//DrawSphere3D(pos_, collisionRadius_, 50, 0xff00ff, 0xff00ff, true);

			// ビルボード描画
			int img = imgs_[cntAnimation_];
			DrawBillboard3D(pos_, 0.5f, 0.5f, IMG_SCALE, 0.0f, img, true);

		}
	}

	void EnemyRockAttack::Release(void)
	{
		// エフェクト画像のメモリの解放
		for (int i = 0; i < NUM_SPRITE_ALL; i++)
		{
			DeleteGraph(imgs_[i]);
		}

	}

	void EnemyRockAttack::InitLoad(void)
	{
		// エフェクト画像のロード
		LoadDivGraph(
			(Application::PATH_IMAGE + "Smoke.png").c_str(),
			NUM_SPRITE_ALL,
			NUM_SPRITE_X, NUM_SPRITE_Y,
			SIZE_SPRITE_X, SIZE_SPRITE_Y,
			imgs_);

	}

	void EnemyRockAttack::InitPost(void)
	{
		// モデルの位置設定
		pos_ = AsoUtility::VECTOR_ZERO;
		angles_ = { 0.0f, 0.0f, 0.0f };

		// 衝突判定用半径
		collisionRadius_ = ENEMY_ATTACK_RADIUS;

		isAlive_ = false;

		cntAnimation_ = 0;

	}
