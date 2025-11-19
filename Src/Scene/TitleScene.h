#pragma once
#include "SceneBase.h"
class AnimationController;

class TitleScene : public SceneBase
{
public:

	// アニメーション種別
	enum class ANIM_TYPE
	{
		WALK,
		MAX,
	};

	// コンストラクタ
	TitleScene(void);
	// デストラクタ
	~TitleScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// アニメーション制御
	AnimationController* animationController_;

	// アニメーション種別
	int animType_;

	// タイトル画像
	int imgTitle_;

	// タイトル背景
	int imgBack_;

	// モデル
	int modelId_;
	VECTOR pos_;
	VECTOR rot_;
	VECTOR scl_;

};