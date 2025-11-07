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
}

void HpManager::Update(void)
{
}

void HpManager::Draw(void)
{
	DrawRotaGraph(175, 32, 1.0f, 0.0f, imgBerFrame_, true);
	DrawRotaGraph(200, 32, 0.93f, 0.0f, imgHp_, true);
	DrawRotaGraph(200, 30, 1.0f, 0.0f, imgBerCover_, true);
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
