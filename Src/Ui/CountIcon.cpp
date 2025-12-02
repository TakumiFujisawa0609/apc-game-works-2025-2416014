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

    // 敵の攻撃カウントを取得
    cntAttack_ = targetEnemy_->GetCount() / 20;

    // 10から減らす（20フレームごとに1減る）
    currentCount_ = 10 - (cntAttack_ % 10);

    // 範囲制限（0未満にならないように）
    if (currentCount_ < 0) currentCount_ = 0;
    if (currentCount_ > 10) currentCount_ = 10;

    if (isAlive_)
    {
        VECTOR enemyPos = targetEnemy_->GetPos();
        pos_ = VAdd(enemyPos, VGet(0, 360.0f, 0.0f));
    }
}

void CountIcon::Draw(void)
{

    int img = countIcons_[currentCount_];
    DrawBillboard3D(pos_, 0.5f, 0.5f, IMG_SCALE, 0.0f, img, true);

    // 色設定を戻す
    SetDrawBright(255, 255, 255);

    //// デバッグ表示を追加
    //DrawFormatString(0, 560, 0xffffff, "currentCount: %d", currentCount_);
    //DrawFormatString(0, 580, 0xffffff, "cntAttack: %d", cntAttack_);
    //DrawFormatString(0, 500, 0xffffff, "isAlive: %d", isAlive_);
    //DrawFormatString(0, 520, 0xffffff, "img handle: %d", countIcons_[currentCount_]);

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
    currentCount_ = 9;
    cntAttack_ = 0;
    isAlive_ = true;
}
