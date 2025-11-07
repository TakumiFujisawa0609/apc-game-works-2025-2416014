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

	currentHp_ = 10;
}

void HpManager::Update(void)
{
	// ƒvƒŒƒCƒ„[HP‚É•ÏŠ·
	hpRate_ = currentHp_ / player_->GetHp();
	GetGraphSize(imgHp_, &imgWidth_, &imgHeight_);
	drawWidth_ = imgWidth_ * hpRate_;
}

void HpManager::Draw(void)
{
	//DrawRotaGraph(175, 32, 1.0f, 0.0f, imgBerFrame_, true);
	//DrawRectGraph(205 - imgWidth_ / 2, 32 - imgHeight_ / 2, 0, 0, drawWidth_, imgHeight_, imgHp_, true);
	//DrawRotaGraph(200, 30, 1.0f, 0.0f, imgBerCover_, true);




	//DrawGraph(100, 32, imgHp_, true);
	DrawRectGraph(320, 100, 320, 22, 320, 22, imgHp_, TRUE,FALSE);
}

void HpManager::Release(void)
{
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
