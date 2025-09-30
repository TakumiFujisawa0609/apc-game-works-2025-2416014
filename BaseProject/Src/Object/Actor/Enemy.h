#pragma once
#include "ActorBase.h"

class Enemy : public ActorBase
{

public:

	enum class ANIM_TYPE
	{
		IDLE,
		WALK,
		MAX,
	};

	Enemy(void);
	~Enemy(void) override;

	void Update(void) override;
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

};

