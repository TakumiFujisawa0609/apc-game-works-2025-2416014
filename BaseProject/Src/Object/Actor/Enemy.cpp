#include <DxLib.h>
#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../../Utility/MatrixUtility.h"
#include "../../Common/AnimationController.h"
#include "./Player.h"
#include "Enemy.h"

Enemy::Enemy()
{
}

Enemy::~Enemy(void)
{
}

void Enemy::SetTarget(Player* player)
{
	player_ = player;
}

void Enemy::Update()
{
	if (isAlive_)
	{
		ActorBase::Update();

		LookPlayer();
		// 索敵
		Search();
		// 攻撃
		Attack();

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

		if (isNotice_)
		{
			// 視野範囲内：ディフューズカラーを赤色にする
			MV1SetMaterialDifColor(modelId_, 0, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
		}
		else if (isHear_)
		{
			// 聴覚範囲内：ディフューズカラーを黄色にする
			MV1SetMaterialDifColor(modelId_, 0, GetColorF(1.0f, 1.0f, 0.0f, 1.0f));
		}
		else if (isAttack_)
		{
			// 聴覚範囲内：ディフューズカラーを蒼色にする
			MV1SetMaterialDifColor(modelId_, 0, GetColorF(0.0f, 0.0f, 0.5f, 1.0f));

		}
		else
		{
			// 視野範囲外：ディフューズカラーを灰色にする
			MV1SetMaterialDifColor(modelId_, 0, GetColorF(0.5f, 0.5f, 0.5f, 1.0f));
		}

	//}
	//
	//DrawFormatString(
	//	0, 90, 0xffffff,
	//	"エネミー角度　 ：(% .1f, % .1f, % .1f)",
	//	AsoUtility::Rad2DegF(angles_.x),
	//	AsoUtility::Rad2DegF(angles_.y),
	//	AsoUtility::Rad2DegF(angles_.z)
	//);

	//DrawFormatString(
	//	0, 110, 0xffffff,
	//	"エネミー座標　 ：(% .1f, % .1f, % .1f)",
	//	pos_.x,
	//	pos_.y,
	//	pos_.z
	//);

	//当たり判定デバッグ表示
	DrawSphere3D(VGet(pos_.x,pos_.y,pos_.z), collisionRadius_, 50, 0x0000ff, 0x0000ff, true);

	// 視野描画
	DrawViewRange();

	// 聴覚範囲描画
	const int DIV = 32;
	for (int i = 0; i < DIV; i++)
	{
		float angle1 = (float)i / DIV * DX_TWO_PI;
		float angle2 = (float)(i + 1) / DIV * DX_TWO_PI;

		VECTOR p1 = VAdd(pos_, VGet(cosf(angle1) * HEAR_RANGE, 0.0f, sinf(angle1) * HEAR_RANGE));
		VECTOR p2 = VAdd(pos_, VGet(cosf(angle2) * HEAR_RANGE, 0.0f, sinf(angle2) * HEAR_RANGE));

		DrawLine3D(p1, p2, GetColor(255, 255, 0));
	}

	// 攻撃範囲描画
	const int ATA = 32;
	for (int i = 0; i < DIV; i++)
	{
		float angle1 = (float)i / DIV * DX_TWO_PI;
		float angle2 = (float)(i + 1) / DIV * DX_TWO_PI;

		VECTOR p1 = VAdd(pos_, VGet(cosf(angle1) * ATTACK_RANGE, 0.0f, sinf(angle1) * ATTACK_RANGE));
		VECTOR p2 = VAdd(pos_, VGet(cosf(angle2) * ATTACK_RANGE, 0.0f, sinf(angle2) * ATTACK_RANGE));

		DrawLine3D(p1, p2, GetColor(255, 0, 0));
	}

	DrawFormatString(0, 150, 0xffffff, "ヒットポイント: %d", hp_);
	DrawFormatString(0, 180, 0xffffff, "攻撃間隔カウンタ: %d", cntAttack_);

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

int Enemy::GetHp(void)
{
	return hp_;
}

void Enemy::Damage(int damage)
{
	hp_ -= damage;
	if (hp_ < 0)
	{
		hp_ = 0;
	}

	if (hp_ <= 0)
	{
		// 撃破状態
		ChangeState(STATE::DEAD);
	}
	else
	{
		// 被ダメ状態
		ChangeState(STATE::DAMAGE);
	}

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
	pos_ = VGet(0.0f, 0.0f, 0.0f);

	// モデルの大きさ設定
	scales_ = { 1.0f, 1.0f, 1.0f };

	// モデルの初期速度
	speed_ = SPEED_MOVE;

	// 移動・攻撃判定用フラグ
	isMove_ = false;
	isAlive_ = true;
	isAttack_ = false;
}

void Enemy::InitAnimation(void)
{
	//モデルアニメーション制御の初期化
	animationController_ = new AnimationController(modelId_);
	for (int i = 0; i < static_cast<int>(ANIM_TYPE::MAX); i++)
	{
		animationController_->AddInFbx(i, 30.0f, i);
	}
}

void Enemy::InitPost(void)
{
	// ここに個別の初期化処理を追加できる

	// 衝突判定用半径
	collisionRadius_ = ENEMY_DEMON_RADIUS;
	
	// 持ちHP
	hp_ = ENEMY_HP;

	// 初期アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE));

	// 初期状態
	ChangeState(STATE::IDLE);
}

void Enemy::DrawViewRange(void)
{

	// 三角形の視野
	float viewRad = AsoUtility::Deg2RadF(VIEW_ANGLE);

	// 向き角度から方向を取得
	MATRIX mat = MGetIdent();
	VECTOR totalAngles = VAdd(angles_, localAngles_);
	mat = MatrixUtility::GetMatrixRotateXYZ(totalAngles);

	// 前方方向
	VECTOR forward = VTransform(AsoUtility::DIR_F, mat);

	// 右方向
	MATRIX rightMat = MMult(mat, MGetRotY(AsoUtility::Deg2RadF(VIEW_ANGLE)));
	VECTOR right = VTransform(AsoUtility::DIR_F, rightMat);

	// 左方向
	MATRIX leftMat = MMult(mat, MGetRotY(AsoUtility::Deg2RadF(-VIEW_ANGLE)));
	VECTOR left = VTransform(AsoUtility::DIR_F, leftMat);

	// 自分の位置
	VECTOR pos0 = pos_;

	// 正面の位置
	VECTOR pos1 = VAdd(pos0, VScale(forward, VIEW_RANGE));

	// 正面から半時計周り
	VECTOR pos2 = VAdd(pos0, VScale(left, VIEW_RANGE));

	// 正面から時計回り
	VECTOR pos3 = VAdd(pos0, VScale(right, VIEW_RANGE));

	// 視野の描画
	pos0.y = pos1.y = pos2.y = pos3.y = 10.0f;	// 地面の少し上
	DrawTriangle3D(pos0, pos2, pos1, 0x0000ff, true);
	DrawTriangle3D(pos0, pos1, pos3, 0x0000ff, true);
	DrawLine3D(pos0, pos1, 0xffff00);
	DrawLine3D(pos0, pos2, 0xffff00);
	DrawLine3D(pos0, pos3, 0xffff00);
}

void Enemy::Search(void)
{
	// プレイヤーの位置
	VECTOR playerPos = player_->GetPos();
	VECTOR toPlayer = VSub(playerPos, pos_);
	// プレイヤーまでの距離を求める
	float distance = VSize(toPlayer);

	// 聴覚範囲内か
	if (distance < HEAR_RANGE)
	{
		isHear_ = true;
	}
	else
	{
		isHear_ = false;
	}

	// 視野範囲
	if (distance < VIEW_RANGE)
	{
		// 単位ベクトルにする
		toPlayer = VNorm(toPlayer);
		//向き角度から方向を取得
		MATRIX mat = MGetIdent();
		mat = MatrixUtility::GetMatrixRotateXYZ(angles_);
		VECTOR forward = VTransform(AsoUtility::DIR_F, mat);
		// 内積を計算
		float dot = VDot(forward, toPlayer);
		// 角度に変換
		float angle = acosf(dot);

		isNotice_ = true;
		//return;

	}

	// 攻撃範囲内か
	if (distance < ATTACK_RANGE)
	{
		isHear_ = false;
		isNotice_ = false;
		isMove_ = false;
		isAttack_ = true;
	}
	else if(distance < VIEW_RANGE)
	{
		isHear_ = false;
		isMove_ = true;
		isNotice_ = true;
		isAttack_ = false;
	}
	else if(distance < HEAR_RANGE)
	{
		isHear_ = true;
		isMove_ = false;
		isNotice_ = false;
		isAttack_ = false;

	}
	else
	{
		isHear_ = false;
		isMove_ = false;
		isNotice_ = false;
		isAttack_ = false;

	}

	if (isAttack_)
	{
		isMove_ = false;
		cntAttack_++;
		// 一定間隔で攻撃させる
		if (cntAttack_ % TERM_ATTACK == 0)
		{
			cntAttack_ = 0;
			ChangeState(STATE::ATTACK);
		}
	}
	else if (isNotice_)
	{
		isMove_ = true;
		ChangeState(STATE::WALK);  
	}
	else if (isHear_)
	{
		isMove_ = false;
		ChangeState(STATE::IDLE);
	}
	else
	{
		isMove_ = false;
		ChangeState(STATE::IDLE);
	}
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
	}

}

void Enemy::Attack(void)
{
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
	// 攻撃アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::ATTACK),false);
}

void Enemy::ChangeDamage(void)
{
	isAttack_ = false;

	cntDamaged_ = CNT_HIT_REACT;

	// ダメージアニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::DAMAGE), false);

}

void Enemy::ChangeDead(void)
{
	cntDamaged_ = CNT_DEAD_REACT;

	// 死亡アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::DEAD),false);
}

void Enemy::ChangeEnd(void)
{
	isAlive_ = false;
}

void Enemy::UpdateIdle(void)
{

	//移動
	Move();

}

void Enemy::UpdateWalk(void)
{
	//攻撃アニメーションが終わったら通常状態に戻る
	if (animationController_->IsEnd())
	{
		ChangeState(STATE::IDLE);
	}

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
	cntDamaged_--;
	//ダメージアニメーションが終わったら通常状態に戻る
	if (cntDamaged_ < 0)
	{
		ChangeState(STATE::IDLE);
	}
}

void Enemy::UpdateDead(void)
{
	cntDamaged_--;
	//死亡アニメーションが終わったら終了
	if (cntDamaged_ < 0 && animationController_->IsEnd())
	{
		ChangeState(STATE::END);
	}
}

void Enemy::UpdateEnd(void)
{
	//停止
	isMove_ = false;
	isAttack_ = false;
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

