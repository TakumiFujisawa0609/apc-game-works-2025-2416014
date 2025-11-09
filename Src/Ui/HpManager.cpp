#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Object/Actor/Player.h"
#include "HpManager.h"

HpManager::HpManager(Player* player)
{
	player_ = player;

}

HpManager::~HpManager(void)
{
}

void HpManager::Init(void)
{
	imgHp_ = LoadGraph((Application::PATH_IMAGE + "HpBer.png").c_str());
	imgBerFrame_ = LoadGraph((Application::PATH_IMAGE + "BerFrame.png").c_str());
	imgBerCover_ = LoadGraph((Application::PATH_IMAGE + "BerCover.png").c_str());

	maxHp_ = 10;        // 最大HP
}

void HpManager::Update(void)
{
	// Player側のHPを取得
	int playerHp = player_->GetHp();

	// HP割合を計算（0.0〜1.0）
	hpRate_ = (float)playerHp / maxHp_;

	GetGraphSize(imgHp_, &imgWidth_, &imgHeight_);
	drawWidth_ = (int)(imgWidth_ * hpRate_);

	// HP状態の更新
	if (playerHp <= 0) {
		ChangeHpState(HP_STATE::LOW);
	}
	else if (playerHp <= 3) {
		ChangeHpState(HP_STATE::LOW);
	}
	else if (playerHp <= 6) {
		ChangeHpState(HP_STATE::MEDIUM);
	}
	else {
		ChangeHpState(HP_STATE::HIGH);
	}
}

void HpManager::Draw(void)
{
	DrawRotaGraph(275, 75, 1.1, 0, imgBerFrame_, true);
	DrawRectGraph(300 - imgWidth_ / 2, 72 - imgHeight_ / 2,
		0, 0, drawWidth_, imgHeight_,
		imgHp_, true, false);
	DrawRotaGraph(300, 70, 1.1, 0, imgBerCover_, true);

	// HP低い時に画面を赤くする
	if (player_->GetHp() <= 3) {
		// 画面端から中心に向けてグラデーション
		int vignetteSize = 40;  // グラデーションの幅
		int alpha = 30;

		
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

		// 上
		DrawBox(0, 0, Application::SCREEN_SIZE_X, vignetteSize,
			GetColor(255, 0, 0), TRUE);
		// 下
		DrawBox(0, Application::SCREEN_SIZE_Y - vignetteSize,
			Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y,
			GetColor(255, 0, 0), TRUE);
		// 左
		DrawBox(0, 0, vignetteSize, Application::SCREEN_SIZE_Y,
			GetColor(255, 0, 0), TRUE);
		// 右
		DrawBox(Application::SCREEN_SIZE_X - vignetteSize, 0,
			Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y,
			GetColor(255, 0, 0), TRUE);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void HpManager::Release(void)
{
	DeleteGraph(imgHp_);	
	DeleteGraph(imgBerCover_);
	DeleteGraph(imgBerFrame_);

}

void HpManager::ChangeHpState(HP_STATE state)
{
	state_ = state;

	switch (state)
	{
	case HpManager::HP_STATE::HIGH:
		break;
	case HpManager::HP_STATE::MEDIUM:
		break;
	case HpManager::HP_STATE::LOW:
		break;
	default:
		break;
	}
}
