#pragma once
#include <DxLib.h>
class Stage;

class Collision
{
public:
	// シングルトン(生成・取得・削除)
	static void CreatInstance(void) { if (instance_ == nullptr) { instance_ = new Collision(); } };
	static Collision* GetInstance(void) { return instance_; }
	static void DeleteInstance(void) {if (instance_ != nullptr) { delete instance_; instance_ = nullptr; }}

public:
	// ステージのポインタを取得する
	void SetStage(Stage* stage) { stage_ = stage; }

	// ステージとカプセルの当たり判定
	VECTOR CollCheckStageCapsule(VECTOR upPos, VECTOR downPos, float radius);

private:
	Collision();
	~Collision();

	// 静的インスタンス
	static Collision* instance_;

	// ステージクラス
	Stage* stage_;
};

