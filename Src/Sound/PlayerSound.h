#pragma once
#include "./SoundManager.h"
class Player;

class PlayerSound : public SoundManager
{

public:

	// コンストラクタ
	PlayerSound(Player* player);
	~PlayerSound(void) override;

	// 更新・描画・解放
	void Update() override;
	void Draw(void) override;
	void Release(void) override;

	// 判定を取得
	const bool GetAlive(void) const;
	void Setlive(bool isAlive);

	// 再生
	void PlayAttack(void);
	void PlayLightning(void);
	void PlayDodge(void);

protected:

	// リソースロード
	void InitLoad(void) override;

	// 初期化後の個別処理
	void InitPost(void) override;

private:

	// プレイヤーー
	Player* player_;

	int attackHandle_;
	int lightningHandle_;
	int dodgeHandle_;


};

