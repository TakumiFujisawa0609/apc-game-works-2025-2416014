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
}
void HpManager::Update(void)
{
}

void HpManager::Draw(void)
{
	DrawBox(30, 40, 300, 60, 0x00ff00,true);
	DrawBox(30, 40, 300, 60, 0xff0000, true);
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
