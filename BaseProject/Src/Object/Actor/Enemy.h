#pragma once
#include "ActorBase.h"
class MatrixUtility;
class Player;

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
		RUN,
		WALK,
		MAX,
	};

	enum class STATE
	{
		IDLE,
		WALK,
		ATTACK,
		DAMAGE,
		DEAD,
		END,
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
	static constexpr float SPEED_MOVE = 2.0f;

	// 衝突判定用半径
	static constexpr float ENEMY_DEMON_RADIUS = 60.0f;

<<<<<<< HEAD
	// 視野の広さ
	static constexpr float VIEW_RANGE = 500.0f;

	// 視野角
	static constexpr float VIEW_ANGLE = 30.0f;

	// 聞こえる範囲
	static constexpr float HEAR_RANGE = 700.0f;

	// 攻撃範囲
	static constexpr float ATTACK_RANGE = 300.0f;

	// HP
	static constexpr int ENEMY_HP = 10;

=======
>>>>>>> 76444a7717c703a94b3ddfb0af416f923a0cb40f
	// コンストラクタ
	Enemy(Player* player);
	~Enemy(void) override;

	// 更新・描画・解放
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

	// 状態遷移
	void ChangeState(STATE state);

	//衝突用半径
	float GetcollisionRadius(void);

	// 生存フラグ
	bool IsAlive(void) const;
	void SetAlive(bool isAlive);

<<<<<<< HEAD
	// HPの取得
	int GetHp(void);

=======
>>>>>>> 76444a7717c703a94b3ddfb0af416f923a0cb40f
protected:

	// リソースロード
	void InitLoad(void) override;

	// 大きさ、回転、座標の初期化
	void InitTransform(void) override;

	// アニメーションの初期化
	void InitAnimation(void) override;

	// 初期化後の個別処理
	void InitPost(void) override;

<<<<<<< HEAD
	// 視野描画
	void DrawViewRange(void);

=======
>>>>>>> 76444a7717c703a94b3ddfb0af416f923a0cb40f
private:

	// プレイヤー
	Player* player_;

	// matrixUtility制御
	MatrixUtility* matrixUtility_;

	STATE state_;

	// 移動速度
	float speed_;

	// 移動・攻撃判定用フラグ
	bool isMove_;
	bool isAttack_;
	//生存判定
	bool isAlive_;

	//攻撃間隔用のカウンタ
	int cntAttack_;

	// 衝突判定用半径
	float collisionRadius_;

<<<<<<< HEAD
	// 検知フラグ
	bool isNotice_;

	// 聴覚フラグ
	bool isHear_;

	// 索敵
	void Search(void);

	// プレイヤー監視
	void LookPlayer(void);

	// 移動
	void Move(void);

	// 攻撃
	void Attack(void);

=======
	//プレイヤー監視
	void LookPlayer(void);

	//移動
	void Move(void);

>>>>>>> 76444a7717c703a94b3ddfb0af416f923a0cb40f
	// 状態遷移
	void ChangeIdle(void);
	void ChangeWalk(void);
	void ChangeAttack(void);
	void ChangeDamage(void);
	void ChangeDead(void);
	void ChangeEnd(void);

	void UpdateIdle(void);
	void UpdateWalk(void);
	void UpdateAttack(void);
	void UpdateDamage(void);
	void UpdateDead(void);
	void UpdateEnd(void);

	void DrawIdle(void);
	void DrawWalk(void);
	void DrawAttack(void);
	void DrawDamage(void);
	void DrawDead(void);
	void DrawEnd(void);

};

