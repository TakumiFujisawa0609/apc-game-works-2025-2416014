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
class EnemyMagicAttack;
class EnemyIcon;
class CountIcon;
class PlayerSound;

class GameScene : public SceneBase
{

public:

	// プレイヤー攻撃ダメージ
	static constexpr int PLAYER_ATTACK_DAMAGE = 20;

	// ヒットストップカウンタ
	static constexpr int HIT_STOP_CNT_ENEMY = 1;
	static constexpr int HIT_STOP_CNT_SHIELD = 2;
	static constexpr int HIT_STOP_CNT_ATTACK = 2;
	static constexpr int HIT_STOP_CNT_DAMAGE = 1;

	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void) override;

	void Init(void) override;
	void Update() override;
	void Draw(void) override;
	void Release(void) override;

	bool IsInvincible() const { return invincibleCnt_ > 0; }
	void SetInvincible(int frames) { invincibleCnt_ = frames; }

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

	// エネミー魔法攻撃
	EnemyMagicAttack* enemyMagicAttack_;

	// エネミーアイコン
	EnemyIcon* enemyIcon_;
	CountIcon* countIcon_;

	// サウンド
	PlayerSound* playerSound_;


	// 地面との衝突用線分
	VECTOR lineTopPos_;
	VECTOR lineDownPos_;

	// ヒットストップカウンタ
	int hitStopCnt_;

	// 無敵カウンタ
	int invincibleCnt_;

	// 衝突判定
	void Collision(void);

	// エネミーとプレイヤーの衝突
	void CollisionEnemy(void);

	// 武器とエネミーの衝突
	void CollisionWeapon(void);
	
	// 魔法攻撃とエネミーの衝突
	void CollisionPlayerMagic(void);

	// ステージブロックとプレイヤーの衝突
	void CollisionStage(void);

	// エネミーの攻撃とプレイヤーの衝突
	void CollisionEnemyAttack(void);

	void CollisionEnemyMagic(void);

	// 盾とエネミー（攻撃とエネミーオブジェクト）の衝突判定
	bool CollisionShield(void);

};
