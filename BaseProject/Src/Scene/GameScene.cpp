#include <cmath>
#include <DxLib.h>
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Object/Grid.h"
#include "../Object/Stage/Stage.h"
#include "../Object/Actor/Player.h"
#include "../Object/Actor/Enemy.h"
#include "GameScene.h"

GameScene::GameScene(void) : SceneBase()
{
	grid_ = nullptr;
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{

	// グリッド初期化
	grid_ = new Grid();
	grid_->Init();

	// ステージ初期化
	stage_ = new Stage();
	stage_->Init();

	// プレイヤー初期化
	player_ = new Player();
	player_->Init();

	// エネミー初期化
	enemy_ = new Enemy();
	enemy_->Init();

	// カメラモード変更
	Camera* camera = SceneManager::GetInstance().GetCamera();
	camera->SetFollow(player_);
	camera->ChangeMode(Camera::MODE::FOLLOW);

}

void GameScene::Update(void)
{
	// グリッド更新
	grid_->Update();

	// ステージ更新
	stage_->Update();

	// プレイヤー更新
	player_->Update();

	// エネミー更新
	enemy_->Update();

	// デバッグ
	// スペースキーとCキーが押下されたら、ゲームクリアへ遷移する
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_SPACE) && InputManager::GetInstance().IsTrgDown(KEY_INPUT_C))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMECLEAR);
	}

	// スペースキーとOキーが押下されたら、ゲームオーバーへ遷移する
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_SPACE) && InputManager::GetInstance().IsTrgDown(KEY_INPUT_O))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
	}
}

void GameScene::Draw(void)
{
	// グリッド描画
	grid_->Draw();

	// ステージ描画
	stage_->Draw();

	// プレイヤー描画
	player_->Draw();

	// エネミー描画
	enemy_->Draw();
}

void GameScene::Release(void)
{
	// グリッド解放
	grid_->Release();
	delete grid_;

	// ステージ解放
	stage_->Release();
	delete stage_;

	// プレイヤー解放
	player_->Release();
	delete player_;

	// エネミー解放
	enemy_->Release();
	delete enemy_;
}
