#pragma once
#include <DxLib.h>
#include "ActorBase.h"
class AnimationController;
class MatrixUtility;
class Camera;
class RangeAttack;

class Player : public ActorBase
{
public:

	// アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		WALK,
		RUN,
		JUMP,
		DODGE,
		ATTACK,
		COMBO,
		DAMAGE,
		MAX,
	};

	// 状態
	enum class STATE
	{
		IDLE,
		WALK,
		RUN,
		JUMP,
		DODGE,
		ATTACK,
		COMBO,
		DEAD,
		END,
		VICTORY,
	};

	// アニメーションの再生速度
	static constexpr float SPEED_ANIM = 40.0f;

	// 攻撃判定用半径
	static constexpr float ATTACK_RADIUS = 20.0f;

	// プレイヤー通常速度
	static constexpr float PLAYER_MOVE = 5.0f;

	// プレイヤーダッシュ速度
	static constexpr float PLAYER_DASH_MOVE = 10.0f;

	//ジャンプ力
	static constexpr float JUMP_POW = 15.0f;

	// プレイヤー回避距離
	static constexpr float DODGE_SPEED = 10.0f;

	//重力
	static constexpr float GRAVITY_POW = 0.8f;

	// 衝突判定用半径
	static constexpr float PLAYER_RADIUS = 40.0f;

	// HP
	static constexpr int PLAYER_HP = 1;

	// 地面衝突用線分の長さ
	static constexpr float COLLISION_LEN = 10.0f;

	// 視野の広さ
	static constexpr float VIEW_RANGE = 600.0f;

	// 視野角
	static constexpr float VIEW_ANGLE = 30.0f;

	// 当たり判定の範囲
	static constexpr float COLL_CAPSULE_UP_POS = 110.0f;	// カプセルの当たり判定(上)
	static constexpr float COLL_CAPSULE_DOWN_POS = 30.0f;	// カプセルの当たり判定(下)
	static constexpr float COLL_CAPSULE_RADIUS = 20.0f;		// カプセルの半径

	// コンストラクタ
	Player(void);
	// デストラクタ
	~Player(void) override;

	// 更新
	void Update(void) override;
	// 描画
	void Draw(void) override;
	void Release(void) override;

	// 状態遷移
	void ChangeState(STATE state);

	// 衝突用半径
	float GetcollisionRadius(void) const;

	// HPの取得
	int GetHp(void) const;

	// 攻撃判定用半径
	float GetAttackCollisionRadius(void) const;

	// 座標を取得・設定
	const VECTOR& GetAttackPos(void) const;
	void SetAttackPos(const VECTOR& pos);

	// 生存フラグ
	const bool IsAttackAlive(void) const;

	// 攻撃判定を取得
	const bool GetAttackAlive(void) const;
	void SetAttackAlive(bool isAttackAlive);

	// 衝突判定
	void CollisionStage(VECTOR pos);

	// ダメージを与える
	void Damage(int damage);

protected:

	// リソースロード
	void InitLoad(void) override;

	// 大きさ、回転、座標の初期化
	void InitTransform(void) override;

	// アニメーションの初期化
	void InitAnimation(void) override;

	// 初期化後の個別処理
	void InitPost(void) override;

	// 移動処理
	void Move(void) override;

	// 視野描画
	void DrawViewRange(void);

private:

	// カメラ
	Camera* camera_;

	// matrixUtility制御
	MatrixUtility* matrixUtility_;
	
	// 範囲攻撃
	RangeAttack* rangeAttack_;

	// 状態
	STATE state_;

	// 回避変数
	// 回避方向
	VECTOR dodgeDir_; 
	// 回避速度
	float dodgeSpeed_; 
	// 回避の経過時間
	float dodgeTimer_;     

	// 衝突判定用半径
	float collisionRadius_;

	//// サイコロモデル情報
	//int diceModelId_;
	//VECTOR dicePos_;
	//VECTOR diceAngles_;
	//VECTOR diceScales_;
	//VECTOR diceLocalPos_;
	//VECTOR diceLocalAngles_;

	// 武器モデル情報
	int swordModelId_;
	VECTOR swordPos_;
	VECTOR swordAngles_;
	VECTOR swordScales_;
	VECTOR swordLocalPos_;
	VECTOR swordLocalAngles_;

	// 武器攻撃判定用座標
	VECTOR attackPos_;
	VECTOR attackAngles_;
	VECTOR attackScales_;
	VECTOR attackLocalPos_;
	VECTOR attackLocalAngles_;

	// ジャンプ力
	float jumpPow_;

	// ジャンプ判定
	bool isJump_;

	// 攻撃判定フラグ
	bool isAttackAlive_;
	// 攻撃判定時間
	int cntAttackAlive_;

	// 衝突判定用半径
	float attackCollisionRadius_;

	// コンボ変数
	// コンボ方向
	VECTOR comboDir_;
	// コンボ速度
	float comboSpeed_;
	// コンボの経過時間
	float comboTimer_;

	// 武器
	void InitSword(void);
	void SyncSword(void);

	// 攻撃判定生存期間の減少
	void ReduceCntAlive(void);

	//void InitDice(void);
	//void SyncDice(void);

	// プレイヤー行動制御
	void PlayerJump(void);
	void PlayerDodge(void);

	void PlayerAttack(void);
	void PlayerCombo(void);

	//void playerDamage(void);

	// 当たり判定
	void Collision(void);

	// ステージとの当たり判定(カプセル)
	void CollisionStageCapsule(void);

	// 状態遷移
	void ChangeIdle(void);
	void ChangeWalk(void);
	void ChangeRun(void);
	void ChangeJump(void);
	void ChangeDodge(void);
	void ChangeAttack(void);
	void ChangeCombo(void);
	void ChangeDead(void);
	void ChangeEnd(void);
	void ChangeVictory(void);

	void UpdateIdle(void);
	void UpdateWalk(void);
	void UpdateRun(void);
	void UpdateJump(void);
	void UpdateDodge(void);
	void UpdateAttack(void);
	void UpdateCombo(void);
	void UpdateDead(void);
	void UpdateEnd(void);
	void UpdateVictory(void);

	void DrawIdle(void);
	void DrawWalk(void);
	void DrawRun(void);
	void DrawJump(void);
	void DrawDodge(void);
	void DrawAttack(void);
	void DrawCombo(void);
	void DrawDead(void);
	void DrawEnd(void);
	void DrawVictory(void);
};