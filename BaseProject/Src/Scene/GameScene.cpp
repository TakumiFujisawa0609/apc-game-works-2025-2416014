#include <cmath>
#include <DxLib.h>
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Object/Grid.h"
#include "../Object/Stage/Stage.h"
#include "../Object/Actor/Player.h"
#include "../Object/Actor/Enemy.h"
#include "../Object/Actor/Collision.h"
#include "../Ui/HpManager.h"
#include "../Utility/AsoUtility.h"
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
	enemy_ = new Enemy(player_);
	enemy_->Init();

	// Ui初期化
	hpManager_ = new HpManager(player_);
	hpManager_->Init();

	// カメラモード変更
	Camera* camera = SceneManager::GetInstance().GetCamera();
	camera->SetFollow(player_);
	camera->ChangeMode(Camera::MODE::FOLLOW);

	// 当たり判定
	Collision::GetInstance()->CreatInstance();
	Collision::GetInstance()->SetStage(stage_);
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

	// Ui更新
	hpManager_->Update();

	// デバッグ
	// 敵が倒されたらゲームクリアへ遷移する
	if (!enemy_->IsAlive())
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMECLEAR);
	}

	// スペースキーとOキーが押下されたら、ゲームオーバーへ遷移する
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_SPACE) && InputManager::GetInstance().IsTrgDown(KEY_INPUT_O))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
	}

	// 衝突判定
	Collision();
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

	// Ui描画
	hpManager_->Draw();

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

	// Ui解放
	hpManager_->Release();
	delete hpManager_;

	// 当たり判定の解放
	Collision::GetInstance()->DeleteInstance();
}

void GameScene::Collision(void)
{
	CollisionEnemy();
	CollisionWeapon();
	CollisionStage();
}

void GameScene::CollisionEnemy(void)
{
	//if (player_->IsInvincible())
	//{
	//	return;
	//}

	// エネミーとプレイヤーの衝突判定
	VECTOR playerPos = player_->GetPos();

	VECTOR enemyPos = enemy_->GetPos();

	//エネミーとプレイヤーの衝突判定
	if (AsoUtility::IsHitSpheres(playerPos, player_->GetcollisionRadius(), enemyPos, enemy_->GetcollisionRadius()))
	{
		//ベクトルを求める
		VECTOR diff = VSub(playerPos, enemyPos);
		diff.y = 0.0f;

		//ベクトルを正規化(これで方向を取得する)
		VECTOR dir = VNorm(diff);

		////プレイヤーがノックバックする
		//player_->KnockBack(dir, 20.0f);

		////プレイヤーがダメージを食らう
		//player_->Damage(1);
		//enemy_->SetAlive(false);

		//std::vector<ShotBase*> shots = enemy->GetShots();


		//for (ShotBase* shot : shots)
		//{
		//	if (player_->IsInvincible())
		//	{
		//		return;
		//	}

		//	VECTOR shotPos = shot->GetPos();

		//	//ベクトルを求める
		//	VECTOR diff = VSub(playerPos, shotPos);
		//	diff.y = 0.0f;

		//	//ベクトルを正規化(これで方向を取得する)
		//	VECTOR dir = VNorm(diff);

		//	//エネミー弾とプレイヤーの当たり判定
		//	if (AsoUtility::IsHitSpheres(shotPos, shot->GetCollisionRadius(), playerPos, enemy->GetcollisionRadius()))
		//	{
		//		//プレイヤーをノックバックさせる
		//		player_->KnockBack(dir, 20.0f);
		//		//プレイヤーがダメージを食らう
		//		player_->Damage(1);
		//		shot->SetAlive(false);
		//	}
		//}
	}

}

void GameScene::CollisionWeapon(void)
{

	// エネミーと攻撃座標の衝突判定
	VECTOR attackPos = player_->GetAttackPos();

	VECTOR enemyPos = enemy_->GetPos();

	//エネミーと武器の衝突判定
	if (AsoUtility::IsHitSpheres(attackPos, player_->GetAttackCollisionRadius(), enemyPos, enemy_->GetcollisionRadius()) && player_->GetAttackAlive())
	{
		//ベクトルを求める
		VECTOR diff = VSub(attackPos, enemyPos);
		diff.y = 0.0f;

		//ベクトルを正規化(これで方向を取得する)
		VECTOR dir = VNorm(diff);

		////プレイヤーがノックバックする
		//player_->KnockBack(dir, 20.0f);

		//エネミーがダメージを食らう
		enemy_->Damage(1);

		player_->SetAttackAlive(false);

		int a = 1;
	}
}

void GameScene::CollisionStage(void)
{
	// ステージブロックとプレイヤーの衝突
	VECTOR playerPos = player_->GetPos();

	// 線分の上座標
	VECTOR topPos = playerPos;

	topPos.y = playerPos.y + (Player::COLLISION_LEN * 2.0f);

	lineTopPos_ = topPos;

	// 線分の下座標
	VECTOR downPos = playerPos;

	downPos.y = playerPos.y - Player::COLLISION_LEN;

	lineDownPos_ = downPos;

	// 線分とブロックモデルの衝突判定
	MV1_COLL_RESULT_POLY result;
	result = MV1CollCheck_Line(stage_->GetModelId(), -1, topPos, downPos);

	if (result.HitFlag == 1)
	{
		player_->CollisionStage(result.HitPosition);
	}

}