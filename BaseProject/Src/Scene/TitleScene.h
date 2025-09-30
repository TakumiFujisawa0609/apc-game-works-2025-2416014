#pragma once
#include "SceneBase.h"

class TitleScene : public SceneBase
{
public:
	// コンストラクタ
	TitleScene(void);
	// デストラクタ
	~TitleScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:
	// ゲームオーバー画像
	int imgTitle_;
};