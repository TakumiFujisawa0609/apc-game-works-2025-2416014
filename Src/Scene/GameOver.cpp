#include <DxLib.h>
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "GameOver.h"

GameOver::GameOver(void)
{
}

GameOver::~GameOver(void)
{
}

void GameOver::Init(void)
{
	imgGameOver_ = LoadGraph((Application::PATH_IMAGE + "Gameover.png").c_str());

}

void GameOver::Update(void)
{
	// スペースキーが押下されたら、タイトルシーンへ遷移する
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
}

void GameOver::Draw(void)
{
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2,1.0f,0.0f, imgGameOver_, true);
}

void GameOver::Release(void)
{
	DeleteGraph(imgGameOver_);
}