#include <DxLib.h>
#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../../Utility/MatrixUtility.h"
#include "../../Common/AnimationController.h"
#include "Enemy.h"

Enemy::Enemy(void)
{
}

Enemy::~Enemy(void)
{
}

void Enemy::Update(void)
{
	ActorBase::Update();
}

void Enemy::Draw(void)
{
	ActorBase::Draw();
}

void Enemy::Release(void)
{
	ActorBase::Release();
}


void Enemy::InitLoad(void)
{
	// モデル読み込み
	modelId_ = MV1LoadModel((Application::PATH_MODEL + "Player/Player.mv1").c_str());

}

void Enemy::InitTransform(void)
{
	// モデルの初期角度
	angles_ = { 0.0f, AsoUtility::Deg2RadF(45.0f), 0.0f };
	localAngles_ = { 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f };

	// モデルの位置設定
	pos_ = { 200.0f, 0.0f, 150.0f };

	// モデルの大きさ設定
	scales_ = { 1.0f, 1.0f, 1.0f };
}

void Enemy::InitAnimation(void)
{

	// モデルアニメーション制御の初期化
	animationController_ = new AnimationController(modelId_);

	// アニメーション追加
	animationController_->Add(
		0, 30.0f, Application::PATH_MODEL + "Player/Idle.mv1");
	animationController_->Add(
		1, 30.0f, Application::PATH_MODEL + "Player/Walk.mv1");

	// 初期アニメーション再生
	animationController_->Play(0);

}

void Enemy::InitPost(void)
{
	// ここに個別の初期化処理を追加できる

}

