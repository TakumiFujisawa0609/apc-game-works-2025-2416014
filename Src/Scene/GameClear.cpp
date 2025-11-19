#include <DxLib.h>
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Common/AnimationController.h"
#include "../Utility/AsoUtility.h"
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
	imgBack_ = LoadGraph((Application::PATH_IMAGE + "WinBack.png").c_str());

	// モデル読み込み
	modelId_ = MV1LoadModel((Application::PATH_MODEL + "Player/Hero.mv1").c_str());
	// モデル読み込み
	enemyModelId_ = MV1LoadModel((Application::PATH_MODEL + "Enemy/Enemy.mv1").c_str());

	// カメラ設定を追加
	VECTOR cameraPos = VGet(0.0f, 100.0f, -300.0f);  // カメラ位置
	VECTOR targetPos = VGet(0.0f, 50.0f, 0.0f);       // 注視点
	SetCameraPositionAndTarget_UpVecY(cameraPos, targetPos);

	// モデルの位置設定
	pos_ = VGet(180.0f,0.0f, 0.0f);
	rot_ = VGet(0.0f, AsoUtility::Deg2RadF(0.0f), 0.0f);
	scl_ = { 2.5f,2.5f,2.5f };

	MV1SetPosition(modelId_, pos_);
	MV1SetRotationXYZ(modelId_, rot_);
	MV1SetScale(modelId_, scl_);

	// エネミーモデルの位置設定
	ePos_ = VGet(-180.0f, 0.0f, 0.0f);
	eRot_ = VGet(0.0f, AsoUtility::Deg2RadF(0.0f), 0.0f);
	eScl_ = { 2.5f,2.5f,2.5f };

	MV1SetPosition(enemyModelId_, ePos_);
	MV1SetRotationXYZ(enemyModelId_, eRot_);
	MV1SetScale(enemyModelId_, eScl_);

	// モデルアニメーション制御の初期化
	playerAnimationController_ = new AnimationController(modelId_);
	playerAnimationController_->Add(
		static_cast<int>(ANIM_TYPE::DANCE), 30.0f, Application::PATH_MODEL + "Player/Dance.mv1");

	enemyAnimationController_ = new AnimationController(enemyModelId_);
	enemyAnimationController_->Add(
		static_cast<int>(ANIM_TYPE::DANCE), 30.0f, Application::PATH_MODEL + "Player/Dance.mv1");

}

void GameClear::Update(void)
{
	rot_.y += AsoUtility::Deg2RadF(0.5f);
	MV1SetRotationXYZ(modelId_, rot_);

	eRot_.y += AsoUtility::Deg2RadF(0.5f);
	MV1SetRotationXYZ(enemyModelId_, eRot_);

	// ダンスアニメーション再生
	playerAnimationController_->Play(static_cast<int>(ANIM_TYPE::DANCE), true);
	enemyAnimationController_->Play(static_cast<int>(ANIM_TYPE::DANCE), true);

	// スペースキーが押下されたら、タイトルシーンへ遷移する
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	playerAnimationController_->Update();
	enemyAnimationController_->Update();
}

void GameClear::Draw(void)
{
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.0f, 0.0f, imgBack_, true);

	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, 150, 3.0f, 0.0f, imgGameClear_, true);

	MV1DrawModel(modelId_);
	MV1DrawModel(enemyModelId_);
}

void GameClear::Release(void)
{
	DeleteGraph(imgGameClear_);
}