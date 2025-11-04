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

	std::string path;

	// ２Ｄ画像のロード
	path = Application::PATH_IMAGE + "Heart.png";

	imgHeart_ = LoadGraph(path.c_str());

	path = Application::PATH_IMAGE + "HeartLost.png";

	imgHeartLost_ = LoadGraph(path.c_str());


}
void HpManager::Update(void)
{
}

void HpManager::Draw(void)
{

	// プレイヤーのＨＰ情報
	int max = Player::PLAYER_HP;

	int playerHp = player_->GetHp();

	for (int i = 0; i < max; i++)
	{
		int img = -1;

		if ((i + 1) > playerHp)
		{
			img = imgHeartLost_;
		}
		else
		{
			img = imgHeart_;
		}

		//SetDrawBright(0, 255, 0); // 赤色にする
		// プレイヤーのＨＰ残量に応じて、ハート画像を切り替える
		DrawRotaGraph(START_X + i * HP_ICON_WIDTH, START_Y, 1.0, 0.0, img, true);
		//SetDrawBright(255, 255, 255); // 元に戻す
		//// プレイヤーのＨＰ残量に応じて、ハート画像を切り替える
		//DrawRotaGraph(START_X + i * HP_ICON_WIDTH, START_Y, 1.0, 0.0, img, true);

	}

}

void HpManager::Release(void)
{

	// ２Ｄ画像のメモリ解放
	DeleteGraph(imgHeart_);

	DeleteGraph(imgHeartLost_);

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
