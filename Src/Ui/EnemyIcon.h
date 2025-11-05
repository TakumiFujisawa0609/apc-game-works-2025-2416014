#pragma once
#include "UiManager.h"
class Enemy;

class EnemyIcon : public UiManager
{
public:

	enum class STATE
	{
		NORMAL,   // 通常
		HARD,    // 怒り
		SOFT,    // 疲労
	};

	// 悪魔アイコンの数
	static constexpr int DEVIL_ICON_NUM = 8; 

	// 画像の大きさ倍率
	static constexpr float IMG_SCALE = 100.0f;

	// コンストラクタ
	EnemyIcon(Enemy* enemy);
	~EnemyIcon(void) override;

	// 更新・描画・解放
	void Update() override;
	void Draw(void) override;
	void Release(void) override;

	// 座標を取得・設定
	const VECTOR& GetPos(void) const;
	void SetPos(const VECTOR& pos);

	void SetState(STATE state) { currentState_ = state; }

protected:

	// リソースロード
	void InitLoad(void) override;

	// 大きさ、回転、座標の初期化
	void InitTransform(void) override;

	// 初期化後の個別処理
	void InitPost(void) override;

private:

	// 標的のエネミー
	Enemy* targetEnemy_;
	
	STATE currentState_;

	// 画像ハンドル配列
	int devilIcons_[DEVIL_ICON_NUM];  

	// アニメーションカウンタ
	int cntAnimation_;

	int currentFrame_;
};