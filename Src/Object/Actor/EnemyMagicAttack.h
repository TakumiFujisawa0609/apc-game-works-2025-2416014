#pragma once
#include <DxLib.h>
#include "./EnemyAttackBase.h"

class EnemyMagicAttack : public EnemyAttackBase
{
public:

    // 横5枚
    static constexpr int NUM_SPRITE_X = 4;

    // 縦1枚  
    static constexpr int NUM_SPRITE_Y = 4;

    // 合計5枚
    static constexpr int NUM_SPRITE_ALL = 16;

    // エフェクト画像の分割された大きさ
    static constexpr int SIZE_SPRITE_X = 256;
    static constexpr int SIZE_SPRITE_Y = 256;

    // 画像の大きさ倍率
    static constexpr float IMG_SCALE = 500.0f;

    // 攻撃判定用半径
    static constexpr float ENEMY_ATTACK_RADIUS = 100.0f;

    // アニメーション速度（フレーム単位）
    static constexpr int ANIM_SPEED = 1;

    // コンストラクタ
    EnemyMagicAttack(void);
    // デストラクタ
    ~EnemyMagicAttack(void);

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

