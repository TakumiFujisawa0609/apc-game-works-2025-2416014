#include <DxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Object/Actor/Enemy.h"
#include "EnemyIcon.h"

EnemyIcon::EnemyIcon(Enemy* enemy) : targetEnemy_(enemy)
{
}

EnemyIcon::~EnemyIcon(void)
{
}

void EnemyIcon::Update()
{
    if (targetEnemy_->GetNormal())
    {
        SetState(STATE::NORMAL);
    }
    else if (targetEnemy_->GetHard())
    {
        SetState(STATE::HARD);
    }
    else
    {
        SetState(STATE::SOFT);
    }
    
    if (isAlive_ && targetEnemy_ != nullptr)
    {
        // エネミーの座標を取得して、頭上に配置
        VECTOR enemyPos = targetEnemy_->GetPos();
        pos_ = VAdd(enemyPos, VGet(0.0f, 530.0f, 0.0f));  // 頭上100上

        // アニメーション更新
        cntAnimation_++;
        int frame = cntAnimation_ / 7;
        currentFrame_ = frame % DEVIL_ICON_NUM;
        if (cntAnimation_ >= 56)
        {
            cntAnimation_ = 0;
        }
    }
}

void EnemyIcon::Draw(void)
{
    if (isAlive_)
    {
        // 状態に応じて色を設定
        switch (currentState_)
        {
        case STATE::NORMAL:
            SetDrawBright(255, 255, 255);  // 白（元の色）
            break;
        case STATE::HARD:
            SetDrawBright(255, 100, 100);  // 赤っぽく
            break;
        case STATE::SOFT:
            SetDrawBright(100, 100, 255);  // 青っぽく
            break;
        }

        int img = devilIcons_[currentFrame_];
        DrawBillboard3D(pos_, 0.5f, 0.5f, IMG_SCALE, 0.0f, img, true);

        // 色設定を戻す
        SetDrawBright(255, 255, 255);
    }
}

void EnemyIcon::Release(void)
{
    // 画像の解放
    for (int i = 0; i < DEVIL_ICON_NUM; i++)
    {
        DeleteGraph(devilIcons_[i]);
    }
}

const VECTOR& EnemyIcon::GetPos(void) const
{
	return pos_;
}

void EnemyIcon::SetPos(const VECTOR& pos)
{
	pos_ = pos;
}

void EnemyIcon::InitLoad(void)
{
    // モデル読み込み
    // 一旦全体を読み込んで、2行目だけ切り出す
    int tempImg = LoadGraph((Application::PATH_IMAGE + "State.png").c_str());

    for (int i = 0; i < DEVIL_ICON_NUM; i++)
    {
        devilIcons_[i] = DerivationGraph(
            i * 512,      // X座標
            512,          // Y座標（2行目なので32px下）
            512,          // 幅
            512,          // 高さ
            tempImg
        );
    }

    DeleteGraph(tempImg);
}

void EnemyIcon::InitTransform(void)
{
    // モデルの位置設定
    pos_ = AsoUtility::VECTOR_ONE;
    scl_ = VGet(IMG_SCALE,IMG_SCALE,IMG_SCALE);
    rot_ = AsoUtility::VECTOR_ZERO;

    isAlive_ = true;
}

void EnemyIcon::InitPost(void)
{
    cntAnimation_ = 0;

    SetState(STATE::SOFT);
}
