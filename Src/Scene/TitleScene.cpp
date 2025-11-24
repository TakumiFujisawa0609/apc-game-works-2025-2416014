#include <DxLib.h>
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Common/AnimationController.h"
#include "../Utility/AsoUtility.h"
#include "../Application.h"
#include "TitleScene.h"

TitleScene::TitleScene(void)
{
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{
	imgTitle_ = LoadGraph((Application::PATH_IMAGE + "Title.png").c_str());

	imgBack_ = LoadGraph((Application::PATH_IMAGE + "Back.png").c_str());

	// モデル読み込み
	modelId_ = MV1LoadModel((Application::PATH_MODEL + "Player/Hero.mv1").c_str());

	// カメラ設定を追加
	VECTOR cameraPos = VGet(0.0f, 100.0f, -300.0f);  // カメラ位置
	VECTOR targetPos = VGet(0.0f, 50.0f, 0.0f);       // 注視点
	SetCameraPositionAndTarget_UpVecY(cameraPos, targetPos);

	// モデルの位置設定
	pos_ = VGet(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 6,0.0f);
	rot_ = VGet(0.0f,AsoUtility::Deg2RadF(0.0f),0.0f);
	scl_ = { 2.5f,2.5f,2.5f };

	MV1SetPosition(modelId_, pos_);
	MV1SetRotationXYZ(modelId_, rot_);
	MV1SetScale(modelId_, scl_);

	// モデルアニメーション制御の初期化
	animationController_ = new AnimationController(modelId_);
	animationController_->Add(
		static_cast<int>(ANIM_TYPE::WALK), 30.0f, Application::PATH_MODEL + "Player/Walk.mv1");

}

void TitleScene::Update(void)
{
	rot_.y += AsoUtility::Deg2RadF(0.5f);
	MV1SetRotationXYZ(modelId_, rot_);

	// 歩くアニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::WALK), true);

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
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}

	animationController_->Update();
}

void TitleScene::Draw(void)
{
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.0f, 0.0f, imgBack_, true);
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2 - 150, 1.5f, 0.0f, imgTitle_, true);

	MV1DrawModel(modelId_);


}

void TitleScene::Release(void)
{
	DeleteGraph(imgTitle_);

	MV1DeleteModel(modelId_);
}