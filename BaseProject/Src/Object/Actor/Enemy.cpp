#include <DxLib.h>
#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../../Utility/MatrixUtility.h"
#include "../../Common/AnimationController.h"
#include "./Player.h"
#include "Enemy.h"

Enemy::Enemy(Player* player)
{
	player_ = player;
}

Enemy::~Enemy(void)
{
}

void Enemy::Update(void)
{
	ActorBase::Update();

	switch (state_)
	{
	case STATE::IDLE:
		UpdateIdle();
		break;
	case STATE::WALK:
		UpdateWalk();
		break;
	case STATE::ATTACK:
		UpdateAttack();
		break;
	case STATE::DAMAGE:
		UpdateDamage();
		break;
	case STATE::DEAD:
		UpdateDead();
		break;
	case STATE::END:
		UpdateEnd();
		break;
	}

}

void Enemy::Draw(void)
{
	ActorBase::Draw();

	// 生存しているときだけ描画
	if (isAlive_)
	{
		switch (state_)
		{
		case STATE::IDLE:
			DrawIdle();
			break;
		case STATE::WALK:
			DrawWalk();
			break;
		case STATE::ATTACK:
			DrawAttack();
			break;
		case STATE::DAMAGE:
			DrawDamage();
			break;
		case STATE::DEAD:
			DrawDead();
			break;
		case STATE::END:
			DrawEnd();
			break;
		}

	}
	
	DrawFormatString(
		0, 90, 0xffffff,
		"エネミー角度　 ：(% .1f, % .1f, % .1f)",
		AsoUtility::Rad2DegF(angles_.x),
		AsoUtility::Rad2DegF(angles_.y),
		AsoUtility::Rad2DegF(angles_.z)
	);

	DrawFormatString(
		0, 110, 0xffffff,
		"エネミー座標　 ：(% .1f, % .1f, % .1f)",
		pos_.x,
		pos_.y,
		pos_.z
	);

	DrawSphere3D(VGet(pos_.x,pos_.y + 100,pos_.z), collisionRadius_, 50, 0x0000ff, 0x0000ff, true);

}

void Enemy::Release(void)
{
	ActorBase::Release();
}

void Enemy::ChangeState(STATE state)
{
	state_ = state;
	switch (state_)
	{
	case STATE::IDLE:
		ChangeIdle();
		break;
	case STATE::WALK:
		ChangeWalk();
		break;
	case STATE::ATTACK:
		ChangeAttack();
		break;
	case STATE::DAMAGE:
		ChangeDamage();
		break;
	case STATE::DEAD:
		ChangeDead();
		break;
	case STATE::END:
		ChangeEnd();
		break;
	}
}

float Enemy::GetcollisionRadius(void)
{
	return collisionRadius_;
}

bool Enemy::IsAlive(void) const
{
	return isAlive_;
}

void Enemy::SetAlive(bool isAlive)
{
	isAlive_ = isAlive;
}

void Enemy::InitLoad(void)
{
	// モデル読み込み
	modelId_ = MV1LoadModel((Application::PATH_MODEL + "Enemy/Demon.mv1").c_str());

}

void Enemy::InitTransform(void)
{
	// モデルの初期角度
	angles_ = { 0.0f, 0.0f, 0.0f };
	localAngles_ = { 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f };

	// モデルの位置設定
	pos_ = { 200.0f, 0.0f, 150.0f };

	// モデルの大きさ設定
	scales_ = { 1.0f, 1.0f, 1.0f };

	// モデルの初期速度
	speed_ = SPEED_MOVE;

	// 移動・攻撃判定用フラグ
	isMove_ = true;
	isAlive_ = true;
	isAttack_ = false;
}

void Enemy::InitAnimation(void)
{

	// モデルアニメーション制御の初期化
	animationController_ = new AnimationController(modelId_);

	// アニメーション追加
	animationController_->Add(
		0, 30.0f, Application::PATH_MODEL + "Enemy/Idle.mv1");
	animationController_->Add(
		1, 30.0f, Application::PATH_MODEL + "Enemy/Walk.mv1");

	// 初期アニメーション再生
	animationController_->Play(0);

}

void Enemy::InitPost(void)
{
	// ここに個別の初期化処理を追加できる

	//モデルアニメーション制御の初期化
	animationController_ = new AnimationController(modelId_);
	for (int i = 0; i < static_cast<int>(ANIM_TYPE::MAX); i++)
	{
		animationController_->AddInFbx(i, 30.0f, i);
	}

	// 衝突判定用半径
	collisionRadius_ = ENEMY_DEMON_RADIUS;
	
	// 初期アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::WALK));

	//プレイヤーの方向を向く
	LookPlayer();

	// 初期状態
	ChangeState(STATE::WALK);
}

void Enemy::LookPlayer(void)
{
	//プレイヤー（相手）の座標を取得
	VECTOR playerPos = player_->GetPos(); //プレイヤー座標

	//ベクトルを求める
	VECTOR diff = VSub(playerPos, pos_);
	diff.y = 0.0f;

	//ベクトルを正規化(これで方向を取得する)
	moveDir_ = VNorm(diff);

	//Y軸回転の計算（XZ平面上の角度を求める）
	angles_.y = atan2(moveDir_.x, moveDir_.z);

	//モデルの方向が正の負の方向を向いているので補正する
	angles_.y += AsoUtility::Deg2RadF(180.0f);

	//回転はY軸のみ
	angles_.x = angles_.z = 0.0f;

	//向きを設定
	MV1SetRotationXYZ(modelId_, angles_);

}

void Enemy::Move(void)
{
	if (isMove_ == true)
	{
		//移動量(方向＊スピード)
		VECTOR movePow = VScale(moveDir_, speed_);

		//移動処理
		pos_ = VAdd(pos_, movePow);

		//モデルに座標を設定する
		MV1SetPosition(modelId_, pos_);

		//プレイヤーの方向を向く
		LookPlayer();
	}

}

void Enemy::ChangeIdle(void)
{
	// 待機アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);

}

void Enemy::ChangeWalk(void)
{
	// 歩くアニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::WALK), true);

}

void Enemy::ChangeAttack(void)
{
	// 攻撃間隔用のカウンタをリセット
	cntAttack_ = 0;

	// 攻撃アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::ATTACK), true);
}

void Enemy::ChangeDamage(void)
{
	// ダメージアニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::DAMAGE), true);

}

void Enemy::ChangeDead(void)
{
	// 死亡アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::DEAD), false);
}

void Enemy::ChangeEnd(void)
{

}

void Enemy::UpdateIdle(void)
{

	//プレイヤーの方向を向く
	LookPlayer();

	//移動
	Move();
}

void Enemy::UpdateWalk(void)
{

}

void Enemy::UpdateAttack(void)
{
	//攻撃アニメーションが終わったら通常状態に戻る
	if (animationController_->IsEnd())
	{
		ChangeState(STATE::IDLE);
	}
}

void Enemy::UpdateDamage(void)
{
	//攻撃アニメーションが終わったら通常状態に戻る
	if (animationController_->IsEnd())
	{
		ChangeState(STATE::IDLE);
	}
}

void Enemy::UpdateDead(void)
{
	//死亡アニメーションが終わったら終了状態にする
	if (animationController_->IsEnd())
	{
		ChangeState(STATE::END);
	}
}

void Enemy::UpdateEnd(void)
{
	//停止
}

void Enemy::DrawIdle(void)
{
	MV1DrawModel(modelId_);
}

void Enemy::DrawWalk(void)
{
	MV1DrawModel(modelId_);
}

void Enemy::DrawAttack(void)
{
	MV1DrawModel(modelId_);
}

void Enemy::DrawDamage(void)
{
	MV1DrawModel(modelId_);
}

void Enemy::DrawDead(void)
{
	MV1DrawModel(modelId_);
}

void Enemy::DrawEnd(void)
{
	MV1DrawModel(modelId_);
}

