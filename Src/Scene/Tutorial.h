#pragma once
#include "SceneBase.h"

class Tutorial : public SceneBase
{
public:
	// コンストラクタ
	Tutorial(void);
	// デストラクタ
	~Tutorial(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:
	// ゲームオーバー画像
	int imgTutorial_;
};