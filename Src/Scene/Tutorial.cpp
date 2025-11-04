#include <DxLib.h>
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "Tutorial.h"

Tutorial::Tutorial(void)
{
}

Tutorial::~Tutorial(void)
{
}

void Tutorial::Init(void)
{
	imgTutorial_ = LoadGraph((Application::PATH_IMAGE + "Tutorial.png").c_str());
}

void Tutorial::Update(void)
{
	// スペースキーが押下されたら、ゲームシーンへ遷移する
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}
}

void Tutorial::Draw(void)
{
	DrawGraph(0, 0, imgTutorial_, true);
}

void Tutorial::Release(void)
{
	DeleteGraph(imgTutorial_);
}