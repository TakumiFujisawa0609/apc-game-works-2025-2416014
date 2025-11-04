#pragma once
#include <vector>
#include <memory>
#include <DxLib.h>

// 範囲攻撃クラス
class EnemyAttack
{
public:

    // 横5枚
    static constexpr int NUM_SPRITE_X = 5;

    // 縦1枚  
    static constexpr int NUM_SPRITE_Y = 1;

    // 合計5枚
    static constexpr int NUM_SPRITE_ALL = 5;

    // エフェクト画像の分割された大きさ
    static constexpr int SIZE_SPRITE_X = 64;
    static constexpr int SIZE_SPRITE_Y = 64;

    // 画像の大きさ倍率
    static constexpr float IMG_SCALE = 300.0f;

    // 攻撃判定用半径
    static constexpr float ENEMY_ATTACK_RADIUS = 300.0f;

    // コンストラクタ
    EnemyAttack(void);

    // デストラクタ
    ~EnemyAttack(void);

    // 初期化
    void Init(void);

    // 更新
    void Update(void);

    // 描画
    void Draw(void);

    // 解放
    void Release(void);

    // エネミー攻撃判定用半径
    float GetCollisionRadius(void) const;

    // 座標を取得・設定
    const VECTOR& GetPos(void) const;
    void SetPos(const VECTOR& pos);

    // 範囲攻撃判定を取得
    const bool GetAlive(void) const;
    void SetAlive(bool isImpuctAlive);

private:

    // 攻撃判定用
    VECTOR pos_;
    VECTOR angles_;

    // 移動方向
    VECTOR moveDir_;

    // 衝突判定用半径
    float collisionRadius_;

    // 攻撃判定フラグ
    bool isImpuctAlive_;

    // 生存時間カウンタ
    int lifeTime_;

    // アニメーションカウンタ
    int cntAnimation_;

    // エフェクト画像
    int imgs_[NUM_SPRITE_ALL];

};