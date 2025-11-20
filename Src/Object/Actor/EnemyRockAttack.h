#pragma once
#include <DxLib.h>
#include "./EnemyAttackBase.h"

class EnemyRockAttack : public EnemyAttackBase
{
public:

	// 横5枚
	static constexpr int NUM_SPRITE_X = 7;

	// 縦1枚  
	static constexpr int NUM_SPRITE_Y = 7;

	// 合計5枚
	static constexpr int NUM_SPRITE_ALL = 49;

	// エフェクト画像の分割された大きさ
	static constexpr int SIZE_SPRITE_X = 256;
	static constexpr int SIZE_SPRITE_Y = 256;

	// 画像の大きさ倍率
	static constexpr float IMG_SCALE = 10.0f;

	// 攻撃判定用半径
	static constexpr float ENEMY_ATTACK_RADIUS = 300.0f;

	// コンストラクタ
	EnemyRockAttack(void);
	// デストラクタ
	~EnemyRockAttack(void);

	// 更新
	void Update(void) override;
	// 描画
	void Draw(void) override;
	// 解放
	void Release(void) override;

protected:

	// リソースロード
	void InitLoad(void) override;
	// 初期化後の個別処理
	void InitPost(void) override;

private:

	int imgs_[NUM_SPRITE_ALL];


};

