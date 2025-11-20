#pragma once
#include "ActorBase.h"
class MatrixUtility;
class Player;
class EnemyAttack;
class EnemyMagicAttack;
class EnemyRockAttack;

class Enemy : public ActorBase
{

public:

	// エネミーのアニメーション種別
	enum class ANIM_TYPE
	{
		ATTACK,
		DEAD,
		DAMAGE,
		IDLE,
		JUMP,
		MAGIC,
		ROCK,
		WALK,
		MAX,
	};

	// 状態変化パターン
	enum class STATE
	{
		IDLE,
		WALK,
		ATTACK,
		MAGIC,
		ROCK,
		DAMAGE,
		DEAD,
		END,
	};

	// エネミー防御力変化
	enum class ARMORSTATE
	{
		NORMAL,	//弱点露出
		HARD,	//硬化状態
		SOFT,	//軟化状態
	};

	// モデルの大きさ
	static constexpr VECTOR SCALES = { 1.0f, 1.0f, 1.0f };

	// 初期位置
	static constexpr VECTOR DEFAULT_POS = { 300.0f, 10.0f, 200.0f };

	//リスポーン位置
	static constexpr VECTOR RESPAWN_POS = { 1000.0, 800.0f, 1000.0f };

	// 標準の自己発光色
	static constexpr COLOR_F COLOR_EMI_DEFAULT = { 0.6f, 0.6f, 0.6f, 1.0f };

	// 移動スピード
	static constexpr float SPEED_MOVE = 3.0f;

	// 衝突判定用半径
	static constexpr float ENEMY_DEMON_RADIUS = 80.0f;

	// 視野の広さ
	static constexpr float VIEW_RANGE = 500.0f;

	// 視野角
	static constexpr float VIEW_ANGLE = 30.0f;

	// 聞こえる範囲
	static constexpr float HEAR_RANGE = 700.0f;

	// エネミー与ダメージ
	static constexpr int ENEMY_DAMAGE = 1.0f;

	// 攻撃範囲
	static constexpr float ATTACK_RANGE = 300.0f;

	// 攻撃範囲補正値
	static constexpr float ATTACK_COL_OFFSET = -200.0f;
	
	// HP
	static constexpr int ENEMY_HP = 1000;

	// ダメージアニメーション用カウンタ
	static constexpr int CNT_HIT_REACT = 40;

	// 死亡アニメーション用カウンタ
	static constexpr int CNT_DEAD_REACT = 80;

	// 攻撃間隔用
	static constexpr int TERM_ATTACK = 200;

	// 魔法攻撃用
	static constexpr int MAGIC_TERM_ATTACK = 1;

	// 魔法攻撃用
	static constexpr int ROCK_TERM_ATTACK = 1;

	// Normalタイマー
	static constexpr int NORMAL_TIMER = 500;
	// Hardタイマー
	static constexpr int HARD_TIMER = 500;
	// Softタイマー
	static constexpr int SOFT_TIMER = 500;

	// コンストラクタ
	Enemy();
	~Enemy(void) override;

	// ターゲット
	void SetTarget(Player* player);

	// 更新・描画・解放
	void Update() override;
	void Draw(void) override;
	void Release(void) override;

	// 状態遷移
	void ChangeState(STATE state);
	// 防御力遷移
	void ChangeArmorState(ARMORSTATE armorState);

	//衝突用半径
	float GetcollisionRadius(void);

	// 生存フラグ
	bool IsAlive(void) const;
	void SetAlive(bool isAlive);

	// HPの取得
	int GetHp(void);

	// ダメージを与える
	void Damage(int damage);


	// それぞれの防御力判定
	const bool GetNormal(void) const;
	void SetNormal(bool isNormal);
	const bool GetHard(void) const;
	void SetHard(bool isHard);
	const bool GetSoft(void) const;
	void SetSoft(bool isSoft);

	// 攻撃カウンタゲッター
	const int GetCount(void) const;

	// 範囲攻撃取得
	EnemyAttack* GetEnemyAttack(void) { return enemyAttack_; }
	EnemyMagicAttack* GetEnemyMagicAttack(void) { return enemyMagicAttack_; }
	EnemyRockAttack* GetEnemyRockAttack(void) { return enemyRockAttack_; }

	// ダメージを受けない
	bool IsInvincible() const { return invincibleTimer_ > 0; }
	void UpdateInvincible() { if (invincibleTimer_ > 0) invincibleTimer_--; }
	void SetInvincible(int time) { isInvincible_ = true;  invincibleTimer_ = time; }
	int GetInvincible(void) const;

protected:

	// リソースロード
	void InitLoad(void) override;

	// 大きさ、回転、座標の初期化
	void InitTransform(void) override;

	// アニメーションの初期化
	void InitAnimation(void) override;

	// 初期化後の個別処理
	void InitPost(void) override;

	// 視野描画
	void DrawViewRange(void);

private:

	// プレイヤー
	Player* player_;

	// matrixUtility制御
	MatrixUtility* matrixUtility_;

	// 攻撃
	EnemyAttack* enemyAttack_;

	// 魔法攻撃
	EnemyMagicAttack* enemyMagicAttack_;

	// 岩攻撃
	EnemyRockAttack* enemyRockAttack_;

	// 状態変化ステート
	STATE state_;

	// 防御力ステート
	ARMORSTATE armorState_;

	// 移動速度
	float speed_;

	// HP
	int hp_;

	// 移動・攻撃判定用フラグ
	bool isMove_;
	bool isAttack_;
	//生存判定
	bool isAlive_;

	//攻撃間隔用のカウンタ
	int cntAttack_;

	// 魔法攻撃用のカウンタ
	int magicCount_;

	// 岩攻撃用のカウンタ
	int rockCount_;

	// 衝突判定用半径
	float collisionRadius_;

	// 検知フラグ
	bool isNotice_;

	// 聴覚フラグ
	bool isHear_;

	// 被ダメージ間隔用カウンタ
	int cntDamaged_;


	// Normalタイマー
	int normalTimer_;

	// Hardタイマー
	int hardTimer_;

	// Softタイマー
	int softTimer_;

	// 防御力判定
	bool isNormal_;
	bool isHard_;
	bool isSoft_;

	//無敵時間
	int invincibleTimer_;
	bool isInvincible_;

	// 索敵
	void Search(void);

	// 敵を探して攻撃
	void AttackSearch(void);

	// プレイヤー監視
	void LookPlayer(void);

	// 移動
	void Move(void);

	// ロール攻撃
	void RollAttack(void);

	// 状態遷移
	void ChangeIdle(void);
	void ChangeWalk(void);
	void ChangeAttack(void);
	void ChangeMagic(void);
	void ChangeRock(void);
	void ChangeDamage(void);
	void ChangeDead(void);
	void ChangeEnd(void);

	void UpdateIdle(void);
	void UpdateWalk(void);
	void UpdateAttack(void);
	void UpdateMagic(void);
	void UpdateRock(void);
	void UpdateDamage(void);
	void UpdateDead(void);
	void UpdateEnd(void);

	void DrawIdle(void);
	void DrawWalk(void);
	void DrawAttack(void);
	void DrawMagic(void);
	void DrawRock(void);
	void DrawDamage(void);
	void DrawDead(void);
	void DrawEnd(void);



	// 防御力遷移
	void ChangeNormal(void);
	void ChangeHard(void);
	void ChangeSoft(void);

	void UpdateNormal(void);
	void UpdateHard(void);
	void UpdateSoft(void);
};

