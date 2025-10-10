#pragma once
#include <DxLib.h>
#include "ActorBase.h"
class AnimationController;
class MatrixUtility;
class Camera;

class Player : public ActorBase
{
public:

	// アニメーションの再生速度
	static constexpr float SPEED_ANIM = 40.0f;

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

	// 衝突判定用半径
	static constexpr float PLAYER_RADIUS = 30.0f;

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

	//衝突用半径
	float GetcollisionRadius(void);

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

	void InitSword(void);
	void SyncSword(void);

	//void InitDice(void);
	//void SyncDice(void);

	void playerAttack(void);
	//void playerDamage(void);
	//void playerJump(void);

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