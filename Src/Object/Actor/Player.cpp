#include <DxLib.h>
#include "../../Application.h"
#include "../../Common/AnimationController.h"
#include "../../Manager/Camera.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/InputManager.h"
#include "./Enemy.h"
#include "./RangeAttack.h"
#include "./Collision.h"
#include "../../Utility/AsoUtility.h"
#include "../../Utility/MatrixUtility.h"
#include "Player.h"

Player::Player(void)
{
}

Player::~Player(void)
{
}

void Player::SetEnemy(Enemy* enemy)
{
	enemy_ = enemy;
}

void Player::Update(void)
{
	ActorBase::Update();

	rangeAttack_->Update();

	// 敵を探して攻撃
	AttackSearch();

	//// 重力（加速度を速度に加算していく）
	//jumpPow_ -= GRAVITY_POW;

	//// プレイヤーの座標に移動量を加算
	//pos_.y += jumpPow_;

	switch (state_)
	{
	case Player::STATE::IDLE:
		UpdateIdle();
		break;
	case Player::STATE::WALK:
		UpdateWalk();
		break;
	case Player::STATE::RUN:
		UpdateRun();
		break;
	case Player::STATE::JUMP:
		UpdateJump();
		break;
	case Player::STATE::DODGE:
		UpdateDodge();
		break;
	case Player::STATE::ATTACK:
		UpdateAttack();
		break;
	case Player::STATE::COMBO:
		UpdateCombo();
		break;
	case Player::STATE::DAMAGE:
		UpdateDamage();
		break;
	case Player::STATE::DEAD:
		UpdateDead();
		break;
	case Player::STATE::END:
		UpdateEnd();
		break;
	case Player::STATE::VICTORY:
		UpdateVictory();
		break;

	}

	// ジャンプ
	PlayerJump();

	// 武器モデルの位置、角度同期
	SyncSword();

	// 盾モデルの位置、角度同期
	SyncShield();

	// 当たり判定
	Collision();

	// 無敵時間減少
	if (invincibleTimer_ > 0) {
		invincibleTimer_--;
	}

}

void Player::Draw(void)
{
	ActorBase::Draw();

	rangeAttack_->Draw();

	switch (state_)
	{
	case Player::STATE::IDLE:
		DrawIdle();
		break;
	case Player::STATE::WALK:
		DrawWalk();
		break;
	case Player::STATE::RUN:
		DrawRun();
		break;
	case Player::STATE::JUMP:
		DrawJump();
		break;
	case Player::STATE::DODGE:
		DrawDodge();
		break;
	case Player::STATE::ATTACK:
		DrawAttack();
		break;
	case Player::STATE::COMBO:
		DrawCombo();
		break;
	case Player::STATE::DAMAGE:
		DrawDamage();
		break;
	case Player::STATE::DEAD:
		DrawDead();
		break;
	case Player::STATE::END:
		DrawEnd();
		break;
	case Player::STATE::VICTORY:
		DrawVictory();
		break;

	}

	if (IsInvincible()) {
		// 5フレームごとに表示／非表示切り替え（点滅）
		if ((invincibleTimer_ / 4) % 2 == 0) {
			// 点滅中は描画しない
			return;
		}
	}

	// 武器モデルの描画
	MV1DrawModel(swordModelId_);
	// シールドモデルの描画
	MV1DrawModel(shieldModelId_);

	//DrawFormatString(
	//	0, 50, 0xffffff,
	//	"キャラ角度　 ：(% .1f, % .1f, % .1f)",
	//	AsoUtility::Rad2DegF(angles_.x),
	//	AsoUtility::Rad2DegF(angles_.y),
	//	AsoUtility::Rad2DegF(angles_.z)
	//);

	//DrawFormatString(
	//	0, 70, 0xffffff,
	//	"キャラ座標　 ：(% .1f, % .1f, % .1f)",
	//	pos_.x,
	//	pos_.y,
	//	pos_.z
	//);

	//DrawFormatString(
	//	0, 140, 0xffffff,
	//	"攻撃角度　 ：(% .1f, % .1f, % .1f)",
	//	AsoUtility::Rad2DegF(attackAngles_.x),
	//	AsoUtility::Rad2DegF(attackAngles_.y),
	//	AsoUtility::Rad2DegF(attackAngles_.z)
	//);

	//DrawFormatString(
	//	0, 160, 0xffffff,
	//	"攻撃座標　 ：(% .1f, % .1f, % .1f)",
	//	attackPos_.x,
	//	attackPos_.y,
	//	attackPos_.z
	//);


	//DrawSphere3D(pos_, collisionRadius_, 50, 0x0000ff, 0x0000ff, true);

	//if (isAttackAlive_)
	//{
	//	DrawSphere3D(attackPos_, attackCollisionRadius_, 50, 0x00ff00, 0x00ff00, true);
	//
	//
	//}

	//DrawSphere3D(shieldPos_, shieldCollisionRadius_, 50, 0x00ff00, 0x00ff00, true);

	// 視野描画
	DrawViewRange();

	//DrawFormatString(0, 210, 0xffffff, "ジャンプタイマー: %.1f", jumpTimer_);

	//DrawFormatString(0, 300, 0xFFFFFF, "isShieldAlive_: %s", isShieldAlive_ ? "ON" : "OFF");
	//DrawFormatString(0, 360, 0xFFFFFF, "isRangeAttackActive_: %s", isRangeAttackActive_ ? "true" : "false");


}

void Player::Release(void)
{
	ActorBase::Release();

	rangeAttack_->Release();

	//// モデルの解放
	//MV1DeleteModel(diceModelId_);
	MV1DeleteModel(swordModelId_);
	MV1DeleteModel(shieldModelId_);

}

void Player::ChangeState(STATE state)
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
	case STATE::RUN:
		ChangeRun();
		break;
	case Player::STATE::JUMP:
		ChangeJump();
		break;
	case STATE::DODGE:
		ChangeDodge();
		break;
	case STATE::ATTACK:
		ChangeAttack();
		break;
	case STATE::COMBO:
		ChangeCombo();
		break;
	case Player::STATE::DAMAGE:
		ChangeDamage();
		break;
	case STATE::DEAD:
		ChangeDead();
		break;
	case STATE::END:
		ChangeEnd();
		break;
	case STATE::VICTORY:
		ChangeVictory();
		break;
	}
}


float Player::GetcollisionRadius(void) const
{
	return collisionRadius_;
}

int Player::GetHp(void) const
{
	return hp_;
}

float Player::GetAttackCollisionRadius(void) const
{
	return attackCollisionRadius_;
}

const VECTOR& Player::GetAttackPos(void) const
{
	return attackPos_;
}

void Player::SetAttackPos(const VECTOR& attackPos)
{
	attackPos_ = attackPos;
}

const bool Player::IsAttackAlive(void) const
{
	return isAttackAlive_;
}


void Player::SetAttackAlive(bool isAttackAlive)
{
	isAttackAlive_ = isAttackAlive;
}

const bool Player::GetRangeAttackActive(void) const
{
	return isRangeAttackActive_;
}

void Player::SetRangeAttackActive(bool isRangeAttackActive)
{
	isRangeAttackActive_ = isRangeAttackActive;
}

float Player::GetShieldCollisionRadius(void) const
{
	return shieldCollisionRadius_;
}

const VECTOR& Player::GetShieldPos(void) const
{
	return shieldPos_;
}

void Player::SetShieldPos(const VECTOR& shieldPos)
{
	shieldPos_ = shieldPos;
}

const bool Player::IsShieldAlive(void) const
{
	return isShieldAlive_;
}

const bool Player::GetShieldAlive(void) const
{
	return isShieldAlive_;
}

void Player::SetShieldAlive(bool isShieldAlive)
{
	isShieldAlive_ = isShieldAlive;
}

const bool Player::IsGuard(void) const
{
	return isGuard_;
}

const bool Player::GetGuard(void) const
{
	return isGuard_;
}

void Player::SetGuard(bool isGuard)
{
	isGuard_ = isGuard;
}

int Player::GetInvincible(void) const
{
	return invincibleTimer_;
}

void Player::CollisionStage(VECTOR pos)
{
	// 衝突判定に指定座標に押し戻す
	pos_ = pos;
	jumpPow_ = 0.0f;
	isJump_ = false;
}

const bool Player::GetAttackAlive(void) const
{
	return isAttackAlive_;
}

void Player::Damage(int damage)
{
	// 無敵状態ならダメージを受けない
	if (IsInvincible()) return;

	// ダメージタイマーが残っている場合も受けない
	if (invincibleTimer_ > 0.0f) return;

	hp_ -= damage;
	if (hp_ < 0) hp_ = 0;

	if (hp_ == 0)
	{
		ChangeState(STATE::DEAD);
	}
	else
	{
		// ダメージタイマーをセット
		invincibleTimer_ = DAMAGE_INVINCIBLE_TIME;

		// HPが残っている場合はダメージ状態へ
		ChangeState(STATE::DAMAGE);
	}

}

void Player::InitLoad(void)
{
	// モデル読み込み
	modelId_ = MV1LoadModel((Application::PATH_MODEL + "Player/Hero.mv1").c_str());

}

void Player::InitTransform(void)
{
	// モデルの位置設定
	pos_ = VGet(0.0f, 0.0f, -950.0f);

	// モデルの角度
	angles_ = { 0.0f, 0.0f, 0.0f };
	localAngles_ = { 0.0f, AsoUtility::Deg2RadF(180.0f),0.0f };

	// モデルの大きさ設定
	scales_ = { 1.0f, 1.0f, 1.0f };

	// 角度から方向に変換する
	moveDir_ = { sinf(angles_.y), 0.0f, cosf(angles_.y) };

	isAttackAlive_ = false;
	isRangeAttackActive_ = false;
	isJump_ = false;

}

void Player::InitAnimation(void)
{
	// モデルアニメーション制御の初期化
	animationController_ = new AnimationController(modelId_);

	animationController_->Add(
		static_cast<int>(ANIM_TYPE::IDLE), 30.0f, Application::PATH_MODEL + "Player/Idle.mv1");
	animationController_->Add(
		static_cast<int>(ANIM_TYPE::WALK), 30.0f, Application::PATH_MODEL + "Player/Walk.mv1");
	animationController_->Add(
		static_cast<int>(ANIM_TYPE::RUN), 30.0f, Application::PATH_MODEL + "Player/Run.mv1");
	animationController_->Add(
		static_cast<int>(ANIM_TYPE::JUMP), 30.0f, Application::PATH_MODEL + "Player/Jump.mv1");
	animationController_->Add(
		static_cast<int>(ANIM_TYPE::DODGE), 50.0f, Application::PATH_MODEL + "Player/Dodge.mv1");
	animationController_->Add(
		static_cast<int>(ANIM_TYPE::GUARD), 40.0f, Application::PATH_MODEL + "Player/Guard.mv1");
	animationController_->Add(
		static_cast<int>(ANIM_TYPE::ATTACK), 40.0f, Application::PATH_MODEL + "Player/Slash.mv1");
	animationController_->Add(
		static_cast<int>(ANIM_TYPE::COMBO), 50.0f, Application::PATH_MODEL + "Player/Combo.mv1");
	animationController_->Add(
		static_cast<int>(ANIM_TYPE::DAMAGE), 30.0f, Application::PATH_MODEL + "Player/Damage.mv1");
	animationController_->Add(
		static_cast<int>(ANIM_TYPE::DEAD), 30.0f, Application::PATH_MODEL + "Player/Dead.mv1");

}

void Player::InitPost(void)
{
	// 範囲攻撃初期化
	rangeAttack_ = new RangeAttack();
	rangeAttack_->Init();

	//ジャンプ力の初期化
	jumpPow_ = 0.0f;

	// 衝突判定用半径
	collisionRadius_ = PLAYER_RADIUS;

	// 持ちHP
	hp_ = PLAYER_HP;

	// 回避初期速度と時間
	dodgeSpeed_ = 0.0f;
	dodgeTimer_ = 0.0f;

	// 弾の生存期間
	cntAttackAlive_ = 100;

	// 攻撃判定用半径
	attackCollisionRadius_ = ATTACK_RADIUS;

	// ジャンプタイマー初期化
	jumpTimer_ = 0.0f;

	// 盾判定用半径
	shieldCollisionRadius_ = SHIELD_RADIUS;

	// 剣・盾モデルの初期化
	InitSword();
	InitShield();

	// 状態初期化
	ChangeState(STATE::IDLE);

}

void Player::InitSword(void)
{
	// 武器モデル読み込み
	swordModelId_ = MV1LoadModel((Application::PATH_MODEL + "Sword.mv1").c_str());

	swordPos_ = MV1GetFramePosition(modelId_, 44);

	swordLocalPos_ = VGet(10.0f, -10.0f, 0.0f);
	MV1SetPosition(swordModelId_, swordLocalPos_);

	swordAngles_ = { 0.0f, 50.0f, 0.0f };
	swordLocalAngles_ = VGet(
		AsoUtility::Deg2RadF(-20.0f),
		AsoUtility::Deg2RadF(120.0f),
		AsoUtility::Deg2RadF(0.0f)
	);

	// 武器の大きさ設定
	swordScales_ = { 0.2f, 0.2f, 0.2f };

	// 攻撃判定用デバッグ
	attackPos_ = MV1GetFramePosition(modelId_, 44);
	attackLocalPos_ = VGet(0.0f, -300.0f, 0.0f);
	attackLocalAngles_ = swordLocalAngles_;
	attackScales_ = { 1.0f, 1.0f, 1.0f };

}

void Player::SyncSword(void)
{

	// 剣(子)のローカル回転行列
	MATRIX swordMat = MatrixUtility::GetMatrixRotateXYZ(swordLocalAngles_);
	// 剣のローカル位置の変換行列
	MATRIX transMatPos = MGetTranslate(swordLocalPos_);
	// 剣のスケール変換行列
	MATRIX scaleMat = MGetScale(swordScales_);

	// 右手(親)の回転行列
	// プレイヤーの手の回転行列
	MATRIX handMat = MV1GetFrameLocalWorldMatrix(modelId_, 44);

	// 回転行列の合成
	// スケールの行列を剣と合成
	MATRIX localMat = MMult(scaleMat, swordMat);
	// 武器のローカル位置の変換行列を合成
	localMat = MMult(localMat, transMatPos);
	// 親子の回転行列を合成(子:武器, 親:手と指定すると親⇒子の順に適用される)
	MATRIX mat = MMult(localMat, handMat);

	//// 回転行列をモデルに反映
	MV1SetMatrix(swordModelId_, mat);
	// 剣の位置を保存（当たり判定用）
	swordPos_ = MV1GetPosition(swordModelId_);

	// 攻撃座標変換 
	attackPos_ = VTransform(attackLocalPos_, mat);
}

void Player::InitShield(void)
{	
	// シールドモデル読み込み
	shieldModelId_ = MV1LoadModel((Application::PATH_MODEL + "Shield.mv1").c_str());

	shieldPos_ = MV1GetFramePosition(modelId_, 20);

	shieldLocalPos_ = VGet(0.0f, 0.0f, 0.0f);
	MV1SetPosition(shieldModelId_, shieldLocalPos_);

	shieldAngles_ = VGet(0.0f, AsoUtility::Deg2RadF(180.0f),
		 0.0f);
	shieldLocalAngles_ = VGet(
		AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(180.0f), 
		AsoUtility::Deg2RadF(0.0f)
	);

	// 武器の大きさ設定
	shieldScales_ = { 0.8f, 0.8f, 0.8f };
	MV1SetRotationXYZ(shieldModelId_, shieldLocalAngles_);
}

void Player::SyncShield(void)
{

	// 盾(子)のローカル回転行列
	MATRIX shieldMat = MatrixUtility::GetMatrixRotateXYZ(shieldLocalAngles_);
	// 盾のローカル位置の変換行列
	MATRIX transMatPos = MGetTranslate(shieldLocalPos_);
	// 盾のスケール変換行列
	MATRIX scaleMat = MGetScale(shieldScales_);


	// 左手(親)の回転行列
	// プレイヤーの手の回転行列
	MATRIX handMat = MV1GetFrameLocalWorldMatrix(modelId_, 10);


	// 回転行列の合成
	// スケールの行列を剣と合成
	MATRIX localMat = MMult(scaleMat, shieldMat);
	// 武器のローカル位置の変換行列を合成
	localMat = MMult(localMat, transMatPos);
	// 親子の回転行列を合成(子:武器, 親:手と指定すると親⇒子の順に適用される)
	MATRIX mat = MMult(localMat, handMat);


	//// 回転行列をモデルに反映
	MV1SetMatrix(shieldModelId_, mat);

	// 当たり判定用に座標を保存
	shieldPos_ = VTransform(shieldLocalPos_, mat);

}

void Player::ReduceCntAlive(void)
{
	isAttackAlive_ = true;
	cntAttackAlive_--;

	if (cntAttackAlive_ < 0)
	{
		isAttackAlive_ = false;
	}

}

void Player::PlayerJump(void)
{
	InputManager& ins = InputManager::GetInstance();

	//重力（加速度を速度に加算していく）
	jumpPow_ -= GRAVITY_POW;

	// キーボードとゲームパッド両方をチェック
	bool isJumpInputKeyboard = ins.IsNew(KEY_INPUT_SPACE);
	bool isJumpInputPad = false;

	// ゲームパッドが接続されている数で処理を分ける
	if (GetJoypadNum() > 0)
	{
		// ゲームパッド操作
		InputManager::JOYPAD_IN_STATE padState =
			ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);
		isJumpInputPad = ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1,
			InputManager::JOYPAD_BTN::RIGHT);
	}

	// どちらかの入力があればジャンプ入力とする
	isJumpInput_ = isJumpInputKeyboard || isJumpInputPad;

	// ジャンプ開始判定
	if (isJumpInput_ && !isJump_)
	{
		isJump_ = true;
		jumpPow_ = JUMP_POW;

		ChangeJump();
	}

	// ジャンプタイマー
	if (isJump_)
	{
		jumpTimer_++;
	}

	pos_.y += jumpPow_;       // Y座標を更新

	// モデルに座標を設定する
	MV1SetPosition(modelId_, pos_);
}

void Player::PlayerAttack(void)
{

	auto& ins = InputManager::GetInstance();

	// 攻撃しているか判定
	bool isAttack;
	isAttack = false;

	// キーボードとゲームパッド両方をチェック
	bool isAttackInputKeyboard = ins.IsNew(KEY_INPUT_K);
	bool isAttackInputPad = false;

	// ゲームパッドが接続されている数で処理を分ける
	if (GetJoypadNum() > 0)
	{
		// ゲームパッド操作
		// 接続されているゲームパッド１の情報を取得
		InputManager::JOYPAD_IN_STATE padState =
			ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

		isAttackInputPad = ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1,
			InputManager::JOYPAD_BTN::DOWN);
	}

	// どちらかの入力があればジャンプ入力とする
	isAttack = isAttackInputKeyboard || isAttackInputPad;

	if (isAttack)
	{
		ChangeAttack();
	}
}

void Player::PlayerDodge(void)
{
	auto& ins = InputManager::GetInstance();

	// 回避しているか判定
	bool isDodge;
	isDodge = false;

	// キーボードとゲームパッド両方をチェック
	bool isDodgeInputKeyboard = ins.IsNew(KEY_INPUT_Q);
	bool isDodgeInputPad = false;

	// ゲームパッドが接続されている数で処理を分ける
	if (GetJoypadNum() > 0)
	{
		// ゲームパッド操作
		// 接続されているゲームパッド１の情報を取得
		InputManager::JOYPAD_IN_STATE padState =
			ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

		isDodgeInputPad = ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1,
			InputManager::JOYPAD_BTN::R_BUMPER);
	}

	isDodge = isDodgeInputKeyboard || isDodgeInputPad;

	if (isDodge)
	{
		ChangeDodge();
	}
}

void Player::PlayerGuard(void)
{
	auto& ins = InputManager::GetInstance();

	// 攻撃しているか判定
	bool inputGuard;
	inputGuard = false;

	// キーボードとゲームパッド両方をチェック
	bool isGuardInputKeyboard = ins.IsNew(KEY_INPUT_L);
	bool isGuardInputPad = false;

	// ゲームパッドが接続されている数で処理を分ける
	if (GetJoypadNum() > 0)
	{
		// ゲームパッド操作
		// 接続されているゲームパッド１の情報を取得
		InputManager::JOYPAD_IN_STATE padState =
			ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

		isGuardInputPad = ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1,
			InputManager::JOYPAD_BTN::L_BUMPER);
	}

	inputGuard = isGuardInputKeyboard || isGuardInputPad;

	if (inputGuard)
	{
		ChangeGuard();
	}
}

void Player::PlayerCombo(void)
{
	auto& ins = InputManager::GetInstance();

	// 攻撃しているか判定
	bool inputCombo;
	inputCombo = false;

	// キーボードとゲームパッド両方をチェック
	bool isComboInputKeyboard = ins.IsNew(KEY_INPUT_J);
	bool isComboInputPad = false;

	// ゲームパッドが接続されている数で処理を分ける
	if (GetJoypadNum() > 0)
	{
		// 接続されているゲームパッド１の情報を取得
		InputManager::JOYPAD_IN_STATE padState =
			ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

		isComboInputPad = ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1,
			InputManager::JOYPAD_BTN::TOP);
	}

	inputCombo = isComboInputKeyboard || isComboInputPad;

	if (inputCombo)
	{
		ChangeCombo();
	}
}

void Player::AttackSearch(void)
{
	// 向き角度から方向を取得
	MATRIX mat = MGetIdent();
	// 視野はプレイヤーの向き（angles_）のみを使用
	// localAngles_はモデル描画用の補正値なので含めない
	VECTOR totalAngles = angles_;

	mat = MatrixUtility::GetMatrixRotateXYZ(totalAngles);

	// 前方方向
	VECTOR forward = VTransform(AsoUtility::DIR_F, mat);

	// エネミーーの位置
	VECTOR enemyPos = enemy_->GetPos();
	VECTOR toEnemy = VSub(enemyPos, pos_);
	// プレイヤーまでの距離を求める
	float distance = VSize(toEnemy);

	// 範囲攻撃可能か
	bool isAttackable_ = false;

	if (distance < RANGE_ATTACK_REACH)
	{
		// 正規化
		VECTOR toEnemyNorm = VNorm(toEnemy);

		// 前方方向との内積で角度チェック
		float dot = VDot(forward, toEnemyNorm);
		float angle = acosf(dot);

		// 範囲攻撃可能
		if (angle <= AsoUtility::Deg2RadF(VIEW_ANGLE))
		{
			isAttackable_ = true;
		}
		else
		{
			isAttackable_ = false;
		}
	}	
	
	if (isAttackable_)
	{
		if (isCombo_ && rangeAttack_->GetLightningPoint() > 0)
		{
			rangeAttack_->SetLightningAlive(true);
			isRangeAttackActive_ = true;
			rangeAttack_->SetLightningPos(VGet(enemyPos.x, enemyPos.y + 350.0f, enemyPos.z));
			//enemy_->Damage(PLAYER_RANGE_DAMAGE);
		}
		else
		{
			rangeAttack_->SetLightningAlive(false);
		}
	}
	else
	{
		rangeAttack_->SetLightningAlive(false);
	}

}

void Player::Collision(void)
{
	// ステージとの当たり判定
	CollisionStageCapsule();
}

void Player::CollisionStageCapsule(void)
{
	// ステージ横の当たり判定(カプセル)
	VECTOR hitNormal = Collision::GetInstance()->CollCheckStageCapsule
	(
		VAdd(pos_, { 0.0f,COLL_CAPSULE_UP_POS,0.0f }),
		VAdd(pos_, { 0.0f,COLL_CAPSULE_DOWN_POS,0.0f }),
		COLL_CAPSULE_RADIUS
	);
	
	// 当たっていた部分の法線方向に押し流す
	pos_ = VAdd(pos_, hitNormal);
}

void Player::ChangeIdle(void)
{

	// 歩くアニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);

	isCombo_ = false;
	isAttackAlive_ = false;
	isRangeAttackActive_ = false;
	isJump_ = false;
	jumpTimer_ = 0.0f;
	rangeAttack_->SetSlashAlive(false);
	isShieldAlive_ = false;
	CollisionStage(pos_);

}

void Player::ChangeWalk(void)
{
	// 歩くアニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::WALK), true);

}

void Player::ChangeRun(void)
{
	// 走るアニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::RUN), true);
}

void Player::ChangeJump(void)
{
	isJump_ = true;
	//jumpPow_ = JUMP_POW;
	// ジャンプアニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::JUMP),false);
}

void Player::ChangeDodge(void)
{
	// 回避アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::DODGE), false);

	// 回避方向を決定
	if (!AsoUtility::EqualsVZero(moveDir_))
	{
		// 入力がある場合は、その方向に回避
		dodgeDir_ = VNorm(moveDir_);
	}
	else
	{
		// 入力がない場合は、プレイヤーの正面方向に回避
		dodgeDir_ = VGet(sinf(angles_.y), 0.0f, cosf(angles_.y));
	}

	// 回避のスピードと時間をセット
	dodgeSpeed_ = DODGE_SPEED;
	dodgeTimer_ = 0.0f;
}

void Player::ChangeGuard(void)
{
	isShieldAlive_ = true;
	// ガードアニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::GUARD), true);
}

void Player::ChangeAttack(void)
{
	// 攻撃判定生存カウンタ時間の減少
	ReduceCntAlive();

	// 攻撃アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::ATTACK), false);
}

void Player::ChangeCombo(void)
{

	// 攻撃判定生存カウンタ時間の減少
	ReduceCntAlive();

	// 攻撃アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::COMBO), false);
}

void Player::ChangeDamage(void)
{
	// 攻撃アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::DAMAGE), false);

}

void Player::ChangeDead(void)
{
	// 攻撃アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::DEAD), false);

}

void Player::ChangeEnd(void)
{
}

void Player::ChangeVictory(void)
{
}

void Player::UpdateIdle(void)
{
}

void Player::UpdateWalk(void)
{

	if (animationController_->IsEnd())
	{
		isAttackAlive_ = false;
		ChangeState(STATE::IDLE);
	}
}

void Player::UpdateRun(void)
{
	if (animationController_->IsEnd())
	{
		ChangeState(STATE::IDLE);
	}
}

void Player::UpdateJump(void)
{
	if (animationController_->IsEnd())
	{
		ChangeState(STATE::IDLE);
	}
}

void Player::UpdateDodge(void)
{
	// 実際に移動
	pos_ = VAdd(pos_, VScale(dodgeDir_, dodgeSpeed_));

	if (animationController_->IsEnd())
	{
		ChangeState(STATE::IDLE);
	}
}

void Player::UpdateGuard(void)
{
}

void Player::UpdateAttack(void)
{
	if (animationController_->IsEnd())
	{
		ChangeState(STATE::IDLE);
	}
}

void Player::UpdateCombo(void)
{
	isCombo_ = true;
	rangeAttack_->SetLightningAlive(true);
	if (animationController_->IsEnd())
	{
		isAttackAlive_ = false;
		ChangeState(STATE::IDLE);
	}
}

void Player::UpdateDamage(void)
{
	if (animationController_->IsEnd())
	{
		ChangeState(STATE::IDLE);
	}

}


void Player::UpdateDead(void)
{
	if (animationController_->IsEnd())
	{
	}

}

void Player::UpdateEnd(void)
{
}

void Player::UpdateVictory(void)
{
}

void Player::DrawIdle(void)
{
	MV1DrawModel(modelId_);

}

void Player::DrawWalk(void)
{
	MV1DrawModel(modelId_);

}

void Player::DrawRun(void)
{
	MV1DrawModel(modelId_);

}

void Player::DrawJump(void)
{
	MV1DrawModel(modelId_);

}

void Player::DrawDodge(void)
{
	MV1DrawModel(modelId_);

}

void Player::DrawGuard(void)
{
	MV1DrawModel(modelId_);
}

void Player::DrawAttack(void)
{
	MV1DrawModel(modelId_);

}

void Player::DrawCombo(void)
{
	MV1DrawModel(modelId_);
}

void Player::DrawDamage(void)
{
	MV1DrawModel(modelId_);
}

void Player::DrawDead(void)
{
	MV1DrawModel(modelId_);

}

void Player::DrawEnd(void)
{
	MV1DrawModel(modelId_);

}

void Player::DrawVictory(void)
{
	MV1DrawModel(modelId_);

}

void Player::Move(void)
{

	auto& ins = InputManager::GetInstance();

	// 攻撃中かチェック
	int currentAnim = animationController_->GetPlayType();
	bool isAttacking = (currentAnim == static_cast<int>(ANIM_TYPE::ATTACK));

	// 回避中かチェック
	bool isDodging = (currentAnim == static_cast<int>(ANIM_TYPE::DODGE));
	// 範囲攻撃中かチェック
	bool isComboing = (currentAnim == static_cast<int>(ANIM_TYPE::COMBO));
	// ジャンプ中かチェック
	bool isJumping = (currentAnim == static_cast<int>(ANIM_TYPE::JUMP));
	// ガード中かチェック
	bool isGuarding = (currentAnim == static_cast<int>(ANIM_TYPE::GUARD));
	// ダメージ中かチェック
	bool isDamaging = (currentAnim == static_cast<int>(ANIM_TYPE::DAMAGE));
	// 死亡中かチェック
	bool isDeading = (currentAnim == static_cast<int>(ANIM_TYPE::DEAD));


	// 攻撃アニメーションが終わったらIDLEに戻す
	if (isAttacking)
	{
		UpdateAttack();
		return;  // 攻撃中は移動処理をスキップ
	}

	if (isDodging)
	{
		UpdateDodge();
		return;
	}

	if (isComboing)
	{
		UpdateCombo();
		return;
	}

	if (isJumping)
	{
		UpdateJump();
	}

	if (isGuarding)
	{
		UpdateGuard();
	}

	if (isDamaging)
	{
		UpdateDamage();
		return;
	}

	if (isDeading)
	{
		return;
	}

	// カメラの角度を取得
	VECTOR camAngles =
		SceneManager::GetInstance().GetCamera()->GetAngles();

	VECTOR dir = AsoUtility::VECTOR_ZERO;

	// キーボード入力をチェック
	VECTOR dirKeyboard = AsoUtility::VECTOR_ZERO;
	bool isDashKeyboard = false;

	if (ins.IsNew(KEY_INPUT_W)) { dirKeyboard = AsoUtility::DIR_F; }
	if (ins.IsNew(KEY_INPUT_A)) { dirKeyboard = AsoUtility::DIR_L; }
	if (ins.IsNew(KEY_INPUT_S)) { dirKeyboard = AsoUtility::DIR_B; }
	if (ins.IsNew(KEY_INPUT_D)) { dirKeyboard = AsoUtility::DIR_R; }
	isDashKeyboard = ins.IsNew(KEY_INPUT_LSHIFT);

	// ゲームパッド入力をチェック
	VECTOR dirPad = AsoUtility::VECTOR_ZERO;
	bool isDashPad = false;

	if (GetJoypadNum() > 0)
	{
		// 接続されているゲームパッド１の情報を取得
		InputManager::JOYPAD_IN_STATE padState =
			ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);
		// アナログキーの入力値から方向を取得
		dirPad = ins.GetDirectionXZAKey(padState.AKeyLX, padState.AKeyLY);
		isDashPad = ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1,
			InputManager::JOYPAD_BTN::L_TRIGGER);
	}

	// キーボードとゲームパッドの入力を統合
		// 方向はどちらか優先（ゲームパッドを優先する例）
	if (!AsoUtility::EqualsVZero(dirPad))
	{
		dir = dirPad;
	}
	else if (!AsoUtility::EqualsVZero(dirKeyboard))
	{
		dir = dirKeyboard;
	}

	// ダッシュはどちらかが押されていればOK
	bool isDash_ = isDashKeyboard || isDashPad;

	// ダッシュ速度を歩行速度の2倍にする
	movePow_ = isDash_ ? PLAYER_DASH_MOVE : PLAYER_MOVE;

	if (!AsoUtility::EqualsVZero(dir))
	{
		// XYZの回転行列
		// XZ平面移動にする場合は、XZの回転を考慮しないようにする
		MATRIX mat = MGetIdent();
		//mat = MMult(mat, MGetRotX(camAngles.x));
		mat = MMult(mat, MGetRotY(camAngles.y));
		//mat = MMult(mat, MGetRotZ(camAngles.z));
		// 回転行列を使用して、ベクトルを回転させる
		moveDir_ = VTransform(dir, mat);
		// 方向×スピードで移動量を作って、座標に足して移動
		pos_ = VAdd(pos_, VScale(moveDir_, movePow_));
		if (!isJumping)
		{
			if (isDash_)
			{
				ChangeRun();
			}
			else
			{
				ChangeWalk();
			}
		}
	}
	else
	{
		if (!isJumping)
		{
			ChangeIdle();
		}
	}

	PlayerGuard();
	PlayerAttack();
	PlayerDodge();
	PlayerCombo();
}

void Player::DrawViewRange(void)
{
	// 三角形の視野
	float viewRad = AsoUtility::Deg2RadF(VIEW_ANGLE);

	// 向き角度から方向を取得
	MATRIX mat = MGetIdent();
	// 視野はプレイヤーの向き（angles_）のみを使用
	// localAngles_はモデル描画用の補正値なので含めない
	VECTOR totalAngles = angles_;

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



	// 範囲攻撃可能エリア
	// 正面の位置
	VECTOR rangePos1 = VAdd(pos0, VScale(forward, RANGE_ATTACK_REACH));

	// 正面から半時計周り
	VECTOR rangePos2 = VAdd(pos0, VScale(left, RANGE_ATTACK_REACH));

	// 正面から時計回り
	VECTOR rangePos3 = VAdd(pos0, VScale(right, RANGE_ATTACK_REACH));

	// 範囲攻撃可能エリアの描画
	pos0.y = rangePos1.y = rangePos2.y = rangePos3.y = 10.0f;	// 地面の少し上
	DrawTriangle3D(pos0, rangePos2, rangePos1, 0xffff00, true);
	DrawTriangle3D(pos0, rangePos1, rangePos3, 0xffff00, true);
	DrawLine3D(pos0, rangePos1, 0xffff00);
	DrawLine3D(pos0, rangePos2, 0xffff00);
	DrawLine3D(pos0, rangePos3, 0xffff00);


}
