#pragma once
#include <vector>
#include <memory>
class Enemy;

// 範囲攻撃クラス
class RangeAttack
{
public:

    // 範囲攻撃判定用半径
    static constexpr float LIGHTNING_RADIUS = 50.0f;

    // コンストラクタ
    RangeAttack(void);

    // デストラクタ
    ~RangeAttack(void);

    // 初期化
    void Init(void);

    // 更新
    void Update(void);

    // 描画
    void Draw(void);

    // 解放
    void Release(void);

    // 範囲攻撃判定用半径
    float GetLightningCollisionRadius(void) const;

    // 座標を取得・設定
    const VECTOR& GetLightningPos(void) const;
    void SetLightningPos(const VECTOR& pos);

    // 生存フラグ
    const bool IsLightningAlive(void) const;

    // 範囲攻撃判定を取得
    const bool GetLightningAlive(void) const;
    void SetLightningAlive(bool isLightningAlive);

    // 範囲攻撃生存時間
    void RangeAttackTime(void);

private:

    Enemy* enemy_;

    // 範囲攻撃用座標
    VECTOR lightningPos_;
    VECTOR lightningAngles_;
    VECTOR lightningScales_;
    VECTOR lightningLocalPos_;
    VECTOR lightningLocalAngles_;

    // 衝突判定用半径
    float lightningCollisionRadius_;

    // 範囲攻撃判定フラグ
    bool isLightningAlive_;

    // 範囲攻撃判定時間
    int cntLightning_;

};