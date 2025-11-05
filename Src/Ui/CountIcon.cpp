#include <DxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Object/Actor/Enemy.h"
#include "CountIcon.h"

CountIcon::CountIcon(Enemy* enemy) : targetEnemy_(enemy)
{
}

CountIcon::~CountIcon(void)
{
}

void CountIcon::Update()
{
    if (targetEnemy_ == nullptr)
    {
        isAlive_ = false;
        return;
    }

    // 状態更新
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

    if (isAlive_)
    {
        VECTOR enemyPos = targetEnemy_->GetPos();
        pos_ = VAdd(enemyPos, VGet(70.0f, 310.0f, 0.0f));
    }
}

void CountIcon::Draw(void)
{
    if (isAlive_)
    {
        // 状態に応じて色を設定
        switch (countState_)
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

        int img = countIcons_[currentCount_];
        DrawBillboard3D(pos_, 0.5f, 0.5f, IMG_SCALE, 0.0f, img, true);

        // 色設定を戻す
        SetDrawBright(255, 255, 255);
    }

}

void CountIcon::Release(void)
{
    // 画像の解放
    for (int i = 0; i < COUNT_ICON_NUM; i++)
    {
        DeleteGraph(countIcons_[i]);
    }
}

const VECTOR& CountIcon::GetPos(void) const
{
    return pos_;
}

void CountIcon::SetPos(const VECTOR& pos)
{
    pos_ = pos;
}

void CountIcon::SetCount(int count)
{
    if (count >= 0 && count <= 9)
    {
        currentCount_ = count;
        isAlive_ = true;  // カウント設定されたら表示
    }
    else
    {
        isAlive_ = false;  // 範囲外なら非表示
    }
}

void CountIcon::InitLoad(void)
{
    // モデル読み込み
    // 一旦全体を読み込んで、2行目だけ切り出す
    int tempImg = LoadGraph((Application::PATH_IMAGE + "Count.png").c_str());

    for (int i = 0; i < COUNT_ICON_NUM; i++)
    {
        countIcons_[i] = DerivationGraph(
            i * 16,       // X座標（16ピクセルずつ）
            0,            // Y座標（1行だけだから0）
            16,           // 幅
            32,           // 高さ
            tempImg
        );
    }

    DeleteGraph(tempImg);
}

void CountIcon::InitTransform(void)
{
    // モデルの位置設定
    pos_ = AsoUtility::VECTOR_ONE;
    scl_ = VGet(IMG_SCALE, IMG_SCALE, IMG_SCALE);
    rot_ = AsoUtility::VECTOR_ZERO;

    isAlive_ = true;
}

void CountIcon::InitPost(void)
{
    currentCount_ = 0;
    isAlive_ = false;

    SetState(STATE::SOFT);
}
