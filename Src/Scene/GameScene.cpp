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
#include "../Object/Actor/RangeAttack.h"
#include "../Object/Actor/EnemyAttack.h"
#include "../Object/Actor/EnemyMagicAttack.h"
#include "../Ui/HpManager.h"
#include "../Ui/EnemyIcon.h"
#include "../Ui/CountIcon.h"
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
	enemy_ = new Enemy();
	enemy_->Init();

	// 範囲攻撃初期化
	rangeAttack_ = player_->GetRangeAttack();

	// エネミー攻撃初期化
	enemyAttack_ = enemy_->GetEnemyAttack();
	// エネミー魔法攻撃初期化
	enemyMagicAttack_ = enemy_->GetEnemyMagicAttack();

	// お互いの参照を設定
	enemy_->SetTarget(player_);
	player_->SetEnemy(enemy_);

	// Ui初期化
	hpManager_ = new HpManager(player_);
	hpManager_->Init();

	// エネミーアイコン
	enemyIcon_ = new EnemyIcon(enemy_);
	enemyIcon_->Init();
	// 数字アイコン
	countIcon_ = new CountIcon(enemy_);
	countIcon_->Init();

	// カメラモード変更
	Camera* camera = SceneManager::GetInstance().GetCamera();
	camera->SetFollow(player_);
	camera->ChangeMode(Camera::MODE::FOLLOW);

	// 当たり判定
	Collision::GetInstance()->CreatInstance();
	Collision::GetInstance()->SetStage(stage_);
}

void GameScene::Update()
{
	if (hitStopCnt_ > 0)
	{
		hitStopCnt_--;
		return;
	}

	// ヒットストップで処理を中断
	if (hitStopCnt_ > 0)
	{
		hitStopCnt_--;
		return;
	}

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

	// エネミーアイコン
	enemyIcon_->Update();
	// 数字アイコン
	countIcon_->Update();

	// 範囲攻撃更新
	rangeAttack_->Update();

	// エネミー攻撃
	enemyAttack_->Update();
	
	// エネミー魔法攻撃
	enemyMagicAttack_->Update();

	//エネミーHP0になったらゲームクリアシーン遷移
	if (enemy_->GetHp() == 0)
	{
		//ゲームオーバーシーンに遷移する
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMECLEAR);
		DxLib_End();
		exit(0);  // ゲームを完全に終了する
	}

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

	// エネミー描画
	enemy_->Draw();

	// プレイヤー描画
	player_->Draw();

	// Ui描画
	hpManager_->Draw();

	// エネミーアイコン描画
	enemyIcon_->Draw();
	// 数字アイコン描画
	countIcon_->Draw();

	if (player_->GetGuard())
	{
		// デバッグ表示
		DrawFormatString(10, 330, 0x00FF00, "Shield Blocked Attack!");

	}
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

	//// Ui解放
	//hpManager_->Release();
	//delete hpManager_;

	// エネミーアイコン
	enemyIcon_->Release();
	delete enemyIcon_;
	// 数字アイコン
	countIcon_->Release();
	delete countIcon_;
	
	// 範囲攻撃解放
	rangeAttack_->Release();
	delete rangeAttack_;

	//// エネミー攻撃解放
	//enemyAttack_->Release();
	//delete enemyAttack_;

	//// エネミー魔法攻撃開放
	//enemyMagicAttack_->Release();
	//delete enemyMagicAttack_;

	// 当たり判定の解放
	Collision::GetInstance()->DeleteInstance();
}

void GameScene::Collision(void)
{

	bool isShieldBlocking = CollisionShield();

	if (player_->GetShieldAlive())
	{
		// 盾でブロックしていなければエネミー攻撃のダメージ判定
		if (!isShieldBlocking)
		{
			CollisionEnemy();
			CollisionEnemyAttack();
			CollisionEnemyMagic();

		}
	}
	else
	{
		// 盾がない場合は通常通りダメージ判定
		CollisionEnemy();
		CollisionEnemyAttack();
		CollisionEnemyMagic();
	}

	CollisionPlayerMagic();
	CollisionWeapon();
	CollisionStage();
}

void GameScene::CollisionEnemy(void)
{
	// ヒットストップ中や無敵時間中は衝突判定をスキップ
	if (hitStopCnt_ > 0 || player_->IsInvincible()) 
	{
		return;
	}

	// エネミーとプレイヤーの衝突判定
	VECTOR playerPos = player_->GetPos();

	VECTOR enemyPos = enemy_->GetPos();

	//エネミーとプレイヤーの衝突判定
	if (AsoUtility::IsHitSpheres(playerPos, player_->GetcollisionRadius(), enemyPos, enemy_->GetcollisionRadius()))
	{

		// 押し戻し処理
		VECTOR diff = VSub(playerPos, enemyPos);
		diff.y = 0.0f;
		float distance = VSize(diff);
		VECTOR dir = VNorm(diff);

		// 重なっている分だけ押し戻す
		float overlap = player_->GetcollisionRadius() + enemy_->GetcollisionRadius() - distance;
		VECTOR pushBack = VScale(dir, overlap);
		player_->SetPos(VAdd(playerPos, pushBack));

		player_->Damage(Enemy::ENEMY_DAMAGE);

		// プレイヤーに無敵時間を付与
		player_->SetInvincible(30);  // 例: 60フレーム無敵
	}

}

void GameScene::CollisionWeapon(void)
{
	// ヒットストップ中や無敵時間中は衝突判定をスキップ
	if (hitStopCnt_ > 0 || enemy_->IsInvincible())
	{
		return;
	}

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

		rangeAttack_->SetSlashAlive(true);
		rangeAttack_->SetSlashPos(VGet(enemyPos.x, enemyPos.y + 150.0f,enemyPos.z));

		if (enemy_->GetNormal())
		{
			enemy_->Damage(Player::PLAYER_NORMAL_DAMAGE);
		}
		else if (enemy_->GetHard())
		{
			enemy_->Damage(Player::PLAYER_HARD_DAMAGE);
		}
		else if (enemy_->GetSoft())
		{
			enemy_->Damage(Player::PLAYER_SOFT_DAMAGE);
		}

		player_->SetAttackAlive(false);

		hitStopCnt_ = HIT_STOP_CNT_ATTACK;

		// プレイヤーに無敵時間を付与
		player_->SetInvincible(120);

	}
}

void GameScene::CollisionPlayerMagic(void)
{
	// ヒットストップ中や無敵時間中は衝突判定をスキップ
	if (hitStopCnt_ > 0 || enemy_->IsInvincible())
	{
		return;
	}

	// エネミーと攻撃座標の衝突判定
	VECTOR rangePos = rangeAttack_->GetLightningPos();

	VECTOR enemyPos = enemy_->GetPos();

	//エネミーと武器の衝突判定
	if (AsoUtility::IsHitSpheres(rangePos, rangeAttack_->GetLightningCollisionRadius(), enemyPos, enemy_->GetcollisionRadius()) && rangeAttack_->GetLightningAlive())
	{
		//ベクトルを求める
		VECTOR diff = VSub(rangePos, enemyPos);
		diff.y = 0.0f;

		//ベクトルを正規化(これで方向を取得する)
		VECTOR dir = VNorm(diff);

		rangeAttack_->SetLightningAlive(true);
		rangeAttack_->SetLightningPos(VGet(enemyPos.x, enemyPos.y + 150.0f, enemyPos.z));

		if (enemy_->GetNormal())
		{
			enemy_->Damage(Player::PLAYER_NORMAL_MAGIC);
		}
		else if (enemy_->GetHard())
		{
			enemy_->Damage(Player::PLAYER_HARD_MAGIC);
		}
		else if (enemy_->GetSoft())
		{
			enemy_->Damage(Player::PLAYER_SOFT_MAGIC);
		}

		//rangeAttack_->SetLightningAlive(false);

		hitStopCnt_ = 5;

		// エネミー無敵時間を付与
		enemy_->SetInvincible(90);

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

void GameScene::CollisionEnemyAttack(void)
{
	// ヒットストップ中や無敵時間中は衝突判定をスキップ
	if (hitStopCnt_ > 0 || player_->IsInvincible())
	{
		return;
	}

	VECTOR playerPos = player_->GetPos();

	VECTOR enemyAttackPos = enemyAttack_->GetPos();

	if (player_->GetInvincible() == 0)
	{

		//エネミーとプレイヤーの衝突判定
		if (AsoUtility::IsHitSpheres(playerPos, player_->GetcollisionRadius(), enemyAttackPos, enemyAttack_->GetCollisionRadius()) && enemyAttack_->GetAlive())
		{
			//ベクトルを求める
			VECTOR diff = VSub(playerPos, enemyAttackPos);
			diff.y = 0.0f;

			//ベクトルを正規化(これで方向を取得する)
			VECTOR dir = VNorm(diff);

			//プレイヤーがダメージを食らう
			player_->Damage(1);
			//enemy_->SetAlive(false);

			hitStopCnt_ = HIT_STOP_CNT_DAMAGE;
			// プレイヤーに無敵時間を付与
			player_->SetInvincible(60);
		}
	}
}

void GameScene::CollisionEnemyMagic(void)
{
	// ヒットストップ中や無敵時間中は衝突判定をスキップ
	if (hitStopCnt_ > 0 || player_->IsInvincible())
	{
		return;
	}

	VECTOR playerPos = player_->GetPos();

	VECTOR enemyMagicPos = enemyMagicAttack_->GetPos();

	if (player_->GetInvincible() == 0)
	{

		//エネミー魔法攻撃とプレイヤーの衝突判定
		if (AsoUtility::IsHitSpheres(playerPos, player_->GetcollisionRadius(), enemyMagicPos, enemyMagicAttack_->GetCollisionRadius()) && enemyMagicAttack_->GetAlive())
		{
			//ベクトルを求める
			VECTOR diff = VSub(playerPos, enemyMagicPos);
			diff.y = 0.0f;

			//ベクトルを正規化(これで方向を取得する)
			VECTOR dir = VNorm(diff);

			//プレイヤーがダメージを食らう
			player_->Damage(2);

			hitStopCnt_ = HIT_STOP_CNT_ATTACK;

			// プレイヤーに無敵時間を付与
			player_->SetInvincible(120);

		}
	}

}

bool GameScene::CollisionShield(void)
{
	// ヒットストップ時や無敵時間の時は処理を無視
	if (hitStopCnt_ > 0 || player_->IsInvincible())
	{
		return false;
	}

	VECTOR enemyAttackPos = enemyAttack_->GetPos();
	VECTOR enemyMagicPos = enemyMagicAttack_->GetPos();
	VECTOR enemyPos = enemy_->GetPos();
	VECTOR shieldPos = player_->GetShieldPos();

	player_->SetGuard(false);

	if (player_->GetInvincible() == 0)
	{
		// エネミー本体と盾の衝突判定
		if (AsoUtility::IsHitSpheres(enemyMagicPos, enemyMagicAttack_->GetCollisionRadius(), shieldPos, player_->GetShieldCollisionRadius()) && player_->GetShieldAlive())
		{
			{
				//ベクトルを求める
				VECTOR diff = VSub(enemyMagicPos, shieldPos);
				diff.y = 0.0f;

				//ベクトルを正規化(これで方向を取得する)
				VECTOR dir = VNorm(diff);
					
				// シールドで防いだ
				player_->SetGuard(true);

				//hitStopCnt_ = HIT_STOP_CNT_SHIELD;

				//// プレイヤーに無敵時間を付与
				//player_->SetInvincible(90);

			}
		}

		// エネミー魔法攻撃と盾の衝突判定
		if (AsoUtility::IsHitSpheres(enemyPos, enemy_->GetcollisionRadius(), shieldPos, player_->GetShieldCollisionRadius()) && player_->GetShieldAlive())
		{
			{
				//ベクトルを求める
				VECTOR diff = VSub(enemyPos, shieldPos);
				diff.y = 0.0f;

				//ベクトルを正規化(これで方向を取得する)
				VECTOR dir = VNorm(diff);

				// シールドで防いだ
				player_->SetGuard(true);	
				
				//hitStopCnt_ = HIT_STOP_CNT_SHIELD;

				//// プレイヤーに無敵時間を付与
				//player_->SetInvincible(90);

			}
		}

		if (player_->GetInvincible() == 0)
		{
			// エネミー攻撃と盾の衝突判定
			if (AsoUtility::IsHitSpheres(enemyAttackPos, enemyAttack_->GetCollisionRadius(), shieldPos, player_->GetShieldCollisionRadius()) && enemyAttack_->GetAlive() && player_->GetShieldAlive())
			{

				//ベクトルを求める
				VECTOR diff = VSub(enemyAttackPos, shieldPos);
				diff.y = 0.0f;

				//ベクトルを正規化(これで方向を取得する)
				VECTOR dir = VNorm(diff);

				// 攻撃を無効化
				enemyAttack_->SetAlive(false);

				// シールドで防いだ
				player_->SetGuard(true);

				//hitStopCnt_ = HIT_STOP_CNT_SHIELD;

				//// プレイヤーに無敵時間を付与
				//player_->SetInvincible(90);

			}
		}

		return player_->IsGuard();
	}
}