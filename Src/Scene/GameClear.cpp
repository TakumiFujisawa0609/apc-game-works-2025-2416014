#include <DxLib.h>
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "GameClear.h"

GameClear::GameClear(void)
{
}

GameClear::~GameClear(void)
{
}

void GameClear::Init(void)
{
	imgGameClear_ = LoadGraph((Application::PATH_IMAGE + "GameClear.png").c_str());
}

void GameClear::Update(void)
{
	// スペースキーが押下されたら、タイトルシーンへ遷移する
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
}

void GameClear::Draw(void)
{
	DrawGraph(0, 0, imgGameClear_, true);
}

void GameClear::Release(void)
{
	DeleteGraph(imgGameClear_);
}