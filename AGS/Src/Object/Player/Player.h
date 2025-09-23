#pragma once
#include <DxLib.h>
class AnimationController;
class MatrixUtility;
class Camera;

class Player
{
public:

	// アニメーションの再生速度
	static constexpr float SPEED_ANIM = 40.0f;

	// アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		WALK,
		MAX,
	};

	// コンストラクタ
	Player(void);
	// デストラクタ
	~Player(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

	// アニメーション制御
	AnimationController* animationController_;

	// カメラ
	Camera* camera_;

	// matrixUtility制御
	MatrixUtility* matrixUtility_;

	// アニメーション種別
	ANIM_TYPE animType_;

	// モデル情報
	int modelId_;
	VECTOR pos_;
	VECTOR angles_;
	VECTOR scales_;

	// ローカル角度
	VECTOR localAngles_;

	// サイコロモデル情報
	int diceModelId_;

	VECTOR dicePos_;
	VECTOR diceAngles_;
	VECTOR diceScales_;
	VECTOR diceLocalPos_;
	VECTOR diceLocalAngles_;

	void InitDice(void);

	void SyncDice(void);

	void Move(void);


};