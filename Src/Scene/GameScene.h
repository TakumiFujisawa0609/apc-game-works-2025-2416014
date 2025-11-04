#pragma once
#include "SceneBase.h"
#include <DxLib.h>
class Grid;
class Stage;
class Player;
class Enemy;
class HpManager;
class RangeAttack;
class EnemyAttack;

class GameScene : public SceneBase
{

public:


	// プレイヤー攻撃ダメージ
	static constexpr int PLAYER_ATTACK_DAMAGE = 20.0f;

	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void) override;

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// グリッド線
	Grid* grid_;

	// ステージ
	Stage* stage_;

	// プレイヤー
	Player* player_;

	// エネミー
	Enemy* enemy_;

	// HP
	HpManager* hpManager_;

	// 範囲攻撃
	RangeAttack* rangeAttack_;

	// エネミー攻撃
	EnemyAttack* enemyAttack_;

	// 地面との衝突用線分
	VECTOR lineTopPos_;
	VECTOR lineDownPos_;

	// 衝突判定
	void Collision(void);

	// エネミーとプレイヤーの衝突
	void CollisionEnemy(void);

	// 武器とエネミーの衝突
	void CollisionWeapon(void);

	// ステージブロックとプレイヤーの衝突
	void CollisionStage(void);

	// エネミーの攻撃とプレイヤーの衝突
	void CollisionEnemyAttack(void);

	// 盾とエネミー（攻撃とエネミーオブジェクト）の衝突判定
	bool CollisionShield(void);

};
