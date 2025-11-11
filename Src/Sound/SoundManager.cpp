#include "SoundManager.h"

SoundManager::SoundManager(void)
{
}

SoundManager::~SoundManager(void)
{
}

void SoundManager::Init(void)
{
	// リソースロード
	InitLoad();
	// 初期化後の個別処理
	InitPost();

}

void SoundManager::Update(void)
{
}

void SoundManager::Draw(void)
{
}

void SoundManager::Release(void)
{
}

const bool SoundManager::GetAlive(void) const
{
    return isAlive_;
}

void SoundManager::Setlive(bool isAlive)
{
	isAlive_ = isAlive;
}

