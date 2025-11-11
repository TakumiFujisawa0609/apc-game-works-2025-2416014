#pragma once
#pragma once
#include <string>
#include <DxLib.h>

class SoundManager
{
public:

	// コンストラクタ
	SoundManager(void);
	// デストラクタ
	virtual ~SoundManager(void);

	// 初期化
	void Init(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void Release(void);
	
	// 判定を取得
	const bool GetAlive(void) const;
	void Setlive(bool isAlive);

protected:

	// サウンドハンドル
	int soundHandle_;

	// 生存判定フラグ
	bool isAlive_;

	// リソースロード
	virtual void InitLoad(void) = 0;

	// 初期化後の個別処理
	virtual void InitPost(void) = 0;

};

