#include <DxLib.h>
#include "../Object/Actor/Player.h"
#include "../Application.h"
#include "PlayerSound.h"

PlayerSound::PlayerSound(void)
{
}

PlayerSound::~PlayerSound(void)
{
}

void PlayerSound::Update(void)
{
}

void PlayerSound::Release(void)
{
	DeleteSoundMem(attackHandle_);
	DeleteSoundMem(lightningHandle_);
	DeleteSoundMem(dodgeHandle_);
	DeleteSoundMem(damageHandle_);
	DeleteSoundMem(dodgeHandle_);
}

const bool PlayerSound::GetAlive(void) const
{
	return isAlive_;
}

void PlayerSound::Setlive(bool isAlive)
{
	isAlive_ = isAlive;
}

void PlayerSound::PlayAttack(void)
{
	PlaySoundMem(attackHandle_, DX_PLAYTYPE_BACK);
}

void PlayerSound::PlayLightning(void)
{
	PlaySoundMem(lightningHandle_, DX_PLAYTYPE_BACK);
}

void PlayerSound::PlayDodge(void)
{
	PlaySoundMem(dodgeHandle_, DX_PLAYTYPE_BACK);
}

void PlayerSound::PlayDamage(void)
{
	PlaySoundMem(damageHandle_, DX_PLAYTYPE_BACK);
}

void PlayerSound::PlayJump(void)
{
	PlaySoundMem(jumpHandle_, DX_PLAYTYPE_BACK);
}

void PlayerSound::InitLoad(void)
{
	attackHandle_ = LoadSoundMem((Application::PATH_SOUND + "PlayerAttack.mp3").c_str());
	lightningHandle_ = LoadSoundMem((Application::PATH_SOUND + "Lightning1.mp3").c_str());
	dodgeHandle_ = LoadSoundMem((Application::PATH_SOUND + "Dodge1.mp3").c_str());
	damageHandle_ = LoadSoundMem((Application::PATH_SOUND + "PlayerDamage.mp3").c_str());
	jumpHandle_ = LoadSoundMem((Application::PATH_SOUND + "Jump.mp3").c_str());
}

void PlayerSound::InitPost(void)
{

}
