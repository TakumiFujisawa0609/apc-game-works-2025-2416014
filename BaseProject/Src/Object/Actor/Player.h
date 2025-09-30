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
		MAX,
	};

	// コンストラクタ
	Player(void);
	// デストラクタ
	~Player(void) override;

	// 更新
	void Update(void) override;
	// 描画
	void Draw(void) override;
	void Release(void) override;

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

	// サイコロモデル情報
	int diceModelId_;

	VECTOR dicePos_;
	VECTOR diceAngles_;
	VECTOR diceScales_;
	VECTOR diceLocalPos_;
	VECTOR diceLocalAngles_;

	void InitDice(void);
	void SyncDice(void);


};