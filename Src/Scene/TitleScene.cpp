#include <DxLib.h>
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
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
	// モデル読み込み
	modelId_ = MV1LoadModel((Application::PATH_MODEL + "Player/Hero.mv1").c_str());
	pos_ = AsoUtility::VECTOR_ZERO;
	rot_ = { 0.0f,AsoUtility::Deg2RadF(90.0f),0.0f };
	scl_ = AsoUtility::VECTOR_ONE;

}

void TitleScene::Update(void)
{
	MV1SetPosition(modelId_, pos_);
	MV1SetRotationXYZ(modelId_, rot_);
	MV1SetScale(modelId_, scl_);

	// スペースキーが押下されたら、チュートリアルシーンへ遷移する
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TUTORIAL);
	}
}

void TitleScene::Draw(void)
{
	DrawGraph(0, 0, imgTitle_, true);

	MV1DrawModel(modelId_);
}

void TitleScene::Release(void)
{
	DeleteGraph(imgTitle_);

	MV1DeleteModel(modelId_);
}