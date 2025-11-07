#pragma once
class Player;

class HpManager
{
public:

	enum class HP_STATE
	{
		HIGH,
		MEDIUM,
		LOW,
	};

	// コンストラクタ
	HpManager(Player* player);

	// デストラクタ
	~HpManager(void);

	void Init(void);

	void Update(void);

	void Draw(void);

	void Release(void);

	// 状態遷移
	void ChangeHpState(HP_STATE state);

private:

	// プレイヤー情報
	Player* player_;

	//Hpの状態
	HP_STATE state_;

	int imgHp_;
	int imgBerCover_;
	int imgBerFrame_;

	// HPレート
	float hpRate_;

	float currentHp_;

	// HP画像幅
	int imgWidth_, imgHeight_;
	int drawWidth_;
};
