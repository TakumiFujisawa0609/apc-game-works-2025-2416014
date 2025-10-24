#pragma once
#include <vector>
#include <memory>
#include <DxLib.h>

// 範囲攻撃クラス
class EnemyAttack
{
public:

    // 攻撃判定用半径
    static constexpr float ENEMY_ATTACK_RADIUS = 40.0f;

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
    void SetAlive(bool isAlive);

private:

    // 攻撃判定用座標
    VECTOR pos_;

    // 移動方向
    VECTOR moveDir_;

    // 衝突判定用半径
    float collisionRadius_;

    // 攻撃判定フラグ
    bool isAlive_;

};