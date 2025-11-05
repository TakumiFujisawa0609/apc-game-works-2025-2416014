#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Utility/MatrixUtility.h"
#include "UiManager.h"

UiManager::UiManager(void)
{
}

UiManager::~UiManager(void)
{
}

void UiManager::Init(void)
{
	// リソースロード
	InitLoad();
	// Transform初期化
	InitTransform();
	// 初期化後の個別処理
	InitPost();
}

void UiManager::Update(void)
{
}

void UiManager::Draw(void)
{
	if (isAlive_) 
	{
		DrawRotaGraph(pos_.x,pos_.y, scl_.x,scl_.y, 0.0, img_, true);
	}
}

void UiManager::Release(void)
{
	// ２Ｄ画像のメモリ解放
	DeleteGraph(img_);
}

const VECTOR& UiManager::GetPos(void) const
{
	return pos_;
}

void UiManager::SetPos(const VECTOR& pos)
{
	pos_ = pos;
}
