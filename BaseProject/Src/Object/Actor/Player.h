#pragma once
#include <DxLib.h>
#include "ActorBase.h"
class AnimationController;
class MatrixUtility;
class Camera;

class Player : public ActorBase
{
public:

	// アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		WALK,
		RUN,
		DODGE,
		ATTACK,
		DAMAGE,
		MAX,
	};

	// 状態
	enum class STATE
	{
		IDLE,
		WALK,
		RUN,
		DODGE,
		ATTACK,
		DEAD,
		END,
		VICTORY,
	};

	// アニメーションの再生速度
	static constexpr float SPEED_ANIM = 40.0f;

	// 攻撃判定用半径
	static constexpr float ATTACK_RADIUS = 20.0f;

	// プレイヤー通常速度
	static constexpr float PLAYER_MOVE = 3.0f;

	// プレイヤーダッシュ速度
	static constexpr float PLAYER_DASH_MOVE = 6.0f;

	// プレイヤー回避距離
	static constexpr float DODGE_SPEED = 3.0f;

	// 衝突判定用半径
	static constexpr float PLAYER_RADIUS = 40.0f;

	// HP
	static constexpr int PLAYER_HP = 3;

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

private:

	// カメラ
	Camera* camera_;

	// matrixUtility制御
	MatrixUtility* matrixUtility_;

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
	// 衝突判定用半径
	float attackCollisionRadius_;

	void InitSword(void);
	void SyncSword(void);

	//void InitDice(void);
	//void SyncDice(void);

	void PlayerAttack(void);
	//void playerDamage(void);
	//void playerJump(void);
	void PlayerDodge(void);

	// 状態遷移
	void ChangeIdle(void);
	void ChangeWalk(void);
	void ChangeRun(void);
	void ChangeDodge(void);
	void ChangeAttack(void);
	void ChangeDead(void);
	void ChangeEnd(void);
	void ChangeVictory(void);

	void UpdateIdle(void);
	void UpdateWalk(void);
	void UpdateRun(void);
	void UpdateDodge(void);
	void UpdateAttack(void);
	void UpdateDead(void);
	void UpdateEnd(void);
	void UpdateVictory(void);

	void DrawIdle(void);
	void DrawWalk(void);
	void DrawRun(void);
	void DrawDodge(void);
	void DrawAttack(void);
	void DrawDead(void);
	void DrawEnd(void);
	void DrawVictory(void);
};