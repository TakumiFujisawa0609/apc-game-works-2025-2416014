#pragma once
#include "./SoundManager.h"

class PlayerSound : public SoundManager
{

public:

	// コンストラクタ
	PlayerSound(void);
	~PlayerSound(void) override;

	void Update(void) override;

	// 解放
	void Release(void) override;

	// 判定を取得
	const bool GetAlive(void) const;
	void Setlive(bool isAlive);

	// 再生
	void PlayAttack(void);
	void PlayLightning(void);
	void PlayDodge(void);
	void PlayDamage(void);
	void PlayJump(void);

protected:

	// リソースロード
	void InitLoad(void) override;
	void InitPost(void) override;

private:

	int attackHandle_;
	int lightningHandle_;
	int dodgeHandle_;
	int damageHandle_;
	int jumpHandle_;


};

