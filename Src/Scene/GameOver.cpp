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
	auto& ins = InputManager::GetInstance();

	// シーン遷移の入力判定
	bool isSceneChangeInput = false;

	// キーボード操作
	bool isSceneChangeKeyboard = ins.IsTrgDown(KEY_INPUT_SPACE);

	// ゲームパッド操作
	bool isSceneChangePad = false;
	if (GetJoypadNum() > 0)
	{
		// 接続されているゲームパッド１の情報を取得
		InputManager::JOYPAD_IN_STATE padState =
			ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

		// どのボタンでシーン遷移するか選んで（例：STARTボタン）		
		isSceneChangePad = ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1,
			InputManager::JOYPAD_BTN::DOWN);  // Aボタン（DOWN）
	}

	// どちらかが押されたらシーン遷移
	if (isSceneChangeKeyboard || isSceneChangePad)
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