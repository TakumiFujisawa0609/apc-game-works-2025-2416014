#pragma once
#include <DxLib.h>

class EnemyAttackBase
{
public:

	// コンストラクタ
	EnemyAttackBase(void);
	// デストラクタ
	~EnemyAttackBase(void);

	// 初期化
	void Init(void);
	// 更新
	virtual void Update(void);
	// 描画
	virtual void Draw(void);
	// 解放
	virtual void Release(void);

	// エネミー攻撃判定用半径
	float GetCollisionRadius(void) const;

	// 座標を取得・設定
	const VECTOR& GetPos(void) const;
	void SetPos(const VECTOR& pos);

	// 範囲攻撃判定を取得
	const bool GetAlive(void) const;
	void SetAlive(bool isAlive);

protected:

	// 攻撃判定用
	VECTOR pos_;
	VECTOR angles_;

	// 移動方向
	VECTOR moveDir_;

	// 衝突判定用半径
	float collisionRadius_;

	// 攻撃判定フラグ
	bool isAlive_;

	// 生存時間カウンタ
	int lifeTime_;

	// アニメーションカウンタ
	int cntAnimation_;

	// リソースロード
	virtual void InitLoad(void) = 0;

	// 初期化後の個別処理
	virtual void InitPost(void) = 0;


};

