#pragma once
#include "SceneBase.h"
class Grid;
class Stage;
class Player;
class Enemy;
class HpManager;

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

<<<<<<< HEAD
	// HP
	HpManager* hpManager_;

=======
>>>>>>> 76444a7717c703a94b3ddfb0af416f923a0cb40f
	// 衝突判定
	void Collision(void);

// エネミーとプレイヤーの衝突
	void CollisionEnemy(void);

	// 武器とエネミーの衝突
	void CollisionWeapon(void);

};
