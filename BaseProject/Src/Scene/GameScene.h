#pragma once
#include "SceneBase.h"
class Grid;
class Stage;
class Player;
class Enemy;


class GameScene : public SceneBase
{

public:

	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void) override;

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// グリッド線
	Grid* grid_;

	// ステージ
	Stage* stage_;

	// プレイヤー
	Player* player_;

	// エネミー
	Enemy* enemy_;

	// 衝突判定
	void Collision(void);

// エネミーとプレイヤーの衝突
	void CollisionEnemy(void);

	// 武器とエネミーの衝突
	void CollisionWeapon(void);

};
