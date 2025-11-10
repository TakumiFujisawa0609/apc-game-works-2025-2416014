#pragma once
#include <vector>
#include <memory>
#include <DxLib.h>
class Enemy;

// 範囲攻撃クラス
class RangeAttack
{
public:

    // 範囲攻撃
    // 横8枚
    static constexpr int NUM_SPRITE_X = 8; 

    // 縦1枚  
    static constexpr int NUM_SPRITE_Y = 1;   

    // 合計8枚
    static constexpr int NUM_SPRITE_ALL = 8;    

    // エフェクト画像の分割された大きさ
    static constexpr int SIZE_XSPRITE_X = 80;   
    static constexpr int SIZE_XSPRITE_Y = 236;  

    // 画像の大きさ倍率
    static constexpr float IMG_SCALE = 280.0f;


    // 通常攻撃
    // 横5枚
    static constexpr int SLASH_NUM_SPRITE_X = 5;

    // 縦2枚  
    static constexpr int SLASH_NUM_SPRITE_Y = 2;

    // 合計10枚
    static constexpr int SLASH_NUM_SPRITE_ALL = 10;

    // エフェクト画像の分割された大きさ
    static constexpr int SLASH_SIZE_SPRITE_X = 128;
    static constexpr int SLASH_SIZE_SPRITE_Y = 128;

    // 画像の大きさ倍率
    static constexpr float SLASH_IMG_SCALE = 400.0f;





    // 範囲攻撃判定用半径
    static constexpr float LIGHTNING_RADIUS = 2000.0f;

    // 範囲攻撃使用ポイント
    static constexpr float LIGHTNING_POINT = 500.0f;



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

    // 範囲攻撃判定を取得
    const bool GetLightningAlive(void) const;
    void SetLightningAlive(bool isLightningAlive);

    // MPを取得・設定
    int GetLightningPoint(void) const;
    void SetLightningPoint(int lightningPoint);

    // 範囲攻撃生存時間
    void RangeAttackTime(void);

    // 座標を取得・設定
    const VECTOR& GetSlashPos(void) const;
    void SetSlashPos(const VECTOR& slashPos);

    // 通常攻撃判定を取得
    const bool GetSlashAlive(void) const;
    void SetSlashAlive(bool isSlashAlive);

private:

    Enemy* enemy_;

    // 範囲攻撃用座標
    VECTOR lightningPos_;
    VECTOR lightningAngles_;
    VECTOR lightningScales_;
    VECTOR lightningLocalPos_;
    VECTOR lightningLocalAngles_;

    // エフェクト用座標
    VECTOR slashPos_;
    // 生存判定フラグ
    bool isSlashAlive_;
    // アニメーションカウンタ
    int cntSlashAnimation_;
    // エフェクト画像
    int slashImgs_[SLASH_NUM_SPRITE_ALL];

    // 移動方向
    VECTOR moveDir_;

    // 衝突判定用半径
    float lightningCollisionRadius_;

    // 範囲攻撃判定フラグ
    bool isLightningAlive_;

    // 範囲攻撃MP
    int lightningPoint_;

    // アニメーションカウンタ
    int cntAnimation_;

    // エフェクト画像
    int imgs_[NUM_SPRITE_ALL];

};