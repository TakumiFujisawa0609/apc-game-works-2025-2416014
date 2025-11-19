#pragma once
#include "SceneBase.h"
class AnimationController;

class GameClear : public SceneBase
{
public:

	// アニメーション種別
	enum class ANIM_TYPE
	{
		DANCE,
		MAX,
	};

	// コンストラクタ
	GameClear(void);
	// デストラクタ
	~GameClear(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// アニメーション制御(プレイヤー、エネミー)
	AnimationController* playerAnimationController_;
	AnimationController* enemyAnimationController_;

	// アニメーション種別
	int animType_;

	// ゲームクリア画像
	int imgGameClear_;

	// ゲームクリア背景
	int imgBack_;

	// モデル
	int modelId_;
	VECTOR pos_;
	VECTOR rot_;
	VECTOR scl_;


	// エネミーモデル
	int enemyModelId_;
	VECTOR ePos_;
	VECTOR eRot_;
	VECTOR eScl_;

};