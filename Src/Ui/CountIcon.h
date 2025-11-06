#pragma once
#include "UiManager.h"
class Enemy;

class CountIcon : public UiManager
{
public:

	// 数字アイコンの数
	static constexpr int COUNT_ICON_NUM = 10;

	// 画像の大きさ倍率
	static constexpr float IMG_SCALE = 20.0f;

	// コンストラクタ
	CountIcon(Enemy* enemy);
	~CountIcon(void) override;

	// 更新・描画・解放
	void Update() override;
	void Draw(void) override;
	void Release(void) override;

	// 座標を取得・設定
	const VECTOR& GetPos(void) const;
	void SetPos(const VECTOR& pos);

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

	// 画像ハンドル配列
	int countIcons_[COUNT_ICON_NUM];

	int currentCount_;

};