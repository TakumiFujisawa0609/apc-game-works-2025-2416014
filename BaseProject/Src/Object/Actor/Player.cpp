#include <DxLib.h>
#include "../../Application.h"
#include "../../Common/AnimationController.h"
#include "../../Manager/Camera.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/InputManager.h"
#include "./RangeAttack.h"
#include "../../Utility/AsoUtility.h"
#include "../../Utility/MatrixUtility.h"
#include "Player.h"

Player::Player(void)
{
}

Player::~Player(void)
{
}

void Player::Update(void)
{
	ActorBase::Update();

	rangeAttack_->Update();

	////重力（加速度を速度に加算していく）
	//jumpPow_ -= GRAVITY_POW;

	////プレイヤーの座標に移動量（速度、ジャンプ力）を加算する
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

	// 武器モデルの位置、角度同期
	SyncSword();

	PlayerJump();

	// サイコロモデルのZ軸回転
	//diceAngles_.z += AsoUtility::Deg2RadF(0.7f);
	//SyncDice();
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

	//// サイコロモデルの描画
	//MV1DrawModel(diceModelId_);

	//// デバッグ表示
	//DrawFormatString(
	//	0, 100, 0xffffff,
	//	"サイコロ角度　 ：(% .1f, % .1f, % .1f)",
	//	AsoUtility::Rad2DegF(diceAngles_.x),
	//	AsoUtility::Rad2DegF(diceAngles_.y),
	//	AsoUtility::Rad2DegF(diceAngles_.z)
	//	);

	// 武器モデルの描画
	MV1DrawModel(swordModelId_);

	DrawFormatString(
		0, 50, 0xffffff,
		"キャラ角度　 ：(% .1f, % .1f, % .1f)",
		AsoUtility::Rad2DegF(angles_.x),
		AsoUtility::Rad2DegF(angles_.y),
		AsoUtility::Rad2DegF(angles_.z)
	);

	DrawFormatString(
		0, 70, 0xffffff,
		"キャラ座標　 ：(% .1f, % .1f, % .1f)",
		pos_.x,
		pos_.y,
		pos_.z
	);

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


	//DrawSphere3D(VGet(pos_.x, pos_.y + 100, pos_.z), collisionRadius_, 50, 0x0000ff, 0x0000ff, true);

	if (isAttackAlive_)
	{
		DrawSphere3D(VGet(attackPos_.x, attackPos_.y, attackPos_.z), attackCollisionRadius_, 50, 0x00ff00, 0x00ff00, true);
	}

	// 視野描画
	DrawViewRange();
}

void Player::Release(void)
{
	ActorBase::Release();

	rangeAttack_->Release();

	//// モデルの解放
	//MV1DeleteModel(diceModelId_);
	MV1DeleteModel(swordModelId_);

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
	isAttackAlive = isAttackAlive_;
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
	hp_ -= damage;
	if (hp_ < 0)
	{
		hp_ = 0;
	}

	if (hp_ == 0)
	{
		ChangeState(STATE::DEAD);
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
	pos_ = VGet(0.0f, 10.0f, -950.0f);

	// モデルの角度
	angles_ = { 0.0f, 0.0f, 0.0f };
	localAngles_ = { 0.0f, AsoUtility::Deg2RadF(180.0f),0.0f };

	// モデルの大きさ設定
	scales_ = { 1.0f, 1.0f, 1.0f };

	// 角度から方向に変換する
	moveDir_ = { sinf(angles_.y), 0.0f, cosf(angles_.y) };

	isAttackAlive_ = false;
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
		static_cast<int>(ANIM_TYPE::ATTACK), 40.0f, Application::PATH_MODEL + "Player/Slash.mv1");
	animationController_->Add(
		static_cast<int>(ANIM_TYPE::COMBO), 50.0f, Application::PATH_MODEL + "Player/Combo.mv1");
	// 未追加
	//animationController_->Add(
	//	static_cast<int>(ANIM_TYPE::DAMAGE), 30.0f, Application::PATH_MODEL + "Player/Damage.mv1");

	// 初期アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE));

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

	// ここに個別の初期化処理を追加できる
	//InitDice();
	InitSword();

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

	// 手(親)の回転行列
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

void Player::ReduceCntAlive(void)
{
	isAttackAlive_ = true;
	cntAttackAlive_--;

	if (cntAttackAlive_ < 0)
	{
		isAttackAlive_ = false;
	}

}

//void Player::InitDice(void)
//{
//	// サイコロモデル読み込み
//	diceModelId_ = MV1LoadModel((Application::PATH_MODEL + "Sword.mv1").c_str());
//
//	diceLocalPos_ = { 200.0f,100.0f,0.0f };
//	MV1SetPosition(diceModelId_, diceLocalPos_);
//
//	diceAngles_ = { 0.0f, 0.0f, 0.0f };
//	diceLocalAngles_ = { 0.0f,AsoUtility::Deg2RadF(0.0f), 0.0f };
//
//	MATRIX diceMat = MatrixUtility::Multiplication(diceLocalAngles_, diceAngles_);
//
//	MV1SetRotationMatrix(diceModelId_, diceMat);
//
//	diceScales_ = { 0.5f, 0.5f, 0.5f };
//	MV1SetScale(diceModelId_, diceScales_);
//
//}

//void Player::SyncDice(void)
//{
//	// サイコロモデルの回転行列
//	MATRIX diceMat = MatrixUtility::Multiplication(diceLocalAngles_, diceAngles_);
//	// 親の回転行列
//	MATRIX parentMat = MatrixUtility::GetMatrixRotateXYZ(angles_);
//	// 親子の回転行列を合成(子:サイコロ, 親:プレイヤーと指定すると親⇒子の順に適用される)
//	MATRIX mat = MMult(diceMat, parentMat);
//	// 回転行列をモデルに反映
//	MV1SetRotationMatrix(diceModelId_, mat);
//
//	// サイコロのローカル座標を親の回転行列で回転
//	VECTOR localRotPos_ = VTransform(diceLocalPos_, parentMat);
//	
//	// サイコロのワールド座標
//	dicePos_ = VAdd(localRotPos_, pos_);
//	MV1SetPosition(diceModelId_, dicePos_);
//
//}


void Player::PlayerJump(void)
{
	InputManager& ins = InputManager::GetInstance();

	// ジャンプボタン入力判定
	bool isJumpInput = false;

	// ゲームパッドが接続されている数で処理を分ける
	if (GetJoypadNum() == 0)
	{
		// キーボード操作
		isJumpInput = ins.IsNew(KEY_INPUT_SPACE);
	}
	else
	{
		// ゲームパッド操作
		InputManager::JOYPAD_IN_STATE padState =
			ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);
		isJumpInput = ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1,
			InputManager::JOYPAD_BTN::RIGHT);
	}

	// ジャンプ開始判定
	if (isJumpInput && !isJump_)
	{
		ChangeJump();

	}

	// ジャンプ中のみ重力を適用
	if (isJump_)
	{
		// 重力（加速度を速度に加算していく）
		jumpPow_ -= GRAVITY_POW;

		// プレイヤーの座標に移動量を加算
		pos_.y += jumpPow_;

	}

	// モデルに座標を設定する
	MV1SetPosition(modelId_, pos_);
}
void Player::PlayerAttack(void)
{

	auto& ins = InputManager::GetInstance();

	// 攻撃しているか判定
	bool isAttack;
	isAttack = false;

	// ゲームパッドが接続されている数で処理を分ける
	if (GetJoypadNum() == 0)
	{
		// キーボード操作
		// アタックキー
		isAttack = ins.IsNew(KEY_INPUT_K);
	}
	else
	{
		// ゲームパッド操作
		// 接続されているゲームパッド１の情報を取得
		InputManager::JOYPAD_IN_STATE padState =
			ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

		isAttack = ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1,
			InputManager::JOYPAD_BTN::DOWN);
	}

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

	// ゲームパッドが接続されている数で処理を分ける
	if (GetJoypadNum() == 0)
	{
		// キーボード操作
		// 回避キー
		isDodge = ins.IsNew(KEY_INPUT_Q);
	}
	else
	{
		// ゲームパッド操作
		// 接続されているゲームパッド１の情報を取得
		InputManager::JOYPAD_IN_STATE padState =
			ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

		isDodge = ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1,
			InputManager::JOYPAD_BTN::R_BUMPER);
	}

	if (isDodge)
	{
		ChangeDodge();
	}
}

void Player::PlayerCombo(void)
{
	auto& ins = InputManager::GetInstance();

	// 攻撃しているか判定
	bool isCombo;
	isCombo = false;

	// ゲームパッドが接続されている数で処理を分ける
	if (GetJoypadNum() == 0)
	{
		// キーボード操作
		// アタックキー
		isCombo = ins.IsNew(KEY_INPUT_J);
	}
	else
	{
		// ゲームパッド操作
		// 接続されているゲームパッド１の情報を取得
		InputManager::JOYPAD_IN_STATE padState =
			ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

		isCombo = ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1,
			InputManager::JOYPAD_BTN::TOP);
	}

	if (isCombo)
	{
		rangeAttack_->SetLightningAlive(true);

		ChangeCombo();
	}
	else
	{
		rangeAttack_->SetLightningAlive(false);
	}
}

//void Player::playerDamage(void)
//{
//
//}

void Player::ChangeIdle(void)
{

	// 歩くアニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);

	isAttackAlive_ = false;
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
	jumpPow_ = JUMP_POW;
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

void Player::ChangeDead(void)
{

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

void Player::UpdateAttack(void)
{
	if (animationController_->IsEnd())
	{
		ChangeState(STATE::IDLE);
	}
}

void Player::UpdateCombo(void)
{
	if (animationController_->IsEnd())
	{
		isAttackAlive_ = false;
		ChangeState(STATE::IDLE);
	}
}

void Player::UpdateDead(void)
{
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

void Player::DrawAttack(void)
{
	MV1DrawModel(modelId_);

}

void Player::DrawCombo(void)
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

	// カメラの角度を取得
	VECTOR camAngles =
		SceneManager::GetInstance().GetCamera()->GetAngles();

	VECTOR dir = AsoUtility::VECTOR_ZERO;
	// ダッシュ判定
	bool isDash_ = false;

	// ゲームパッドが接続されている数で処理を分ける
	if (GetJoypadNum() == 0)
	{
		// キーボード操作
		if (ins.IsNew(KEY_INPUT_W)) { dir = AsoUtility::DIR_F; }
		if (ins.IsNew(KEY_INPUT_A)) { dir = AsoUtility::DIR_L; }
		if (ins.IsNew(KEY_INPUT_S)) { dir = AsoUtility::DIR_B; }
		if (ins.IsNew(KEY_INPUT_D)) { dir = AsoUtility::DIR_R; }

		// ダッシュキー
		isDash_ = ins.IsNew(KEY_INPUT_LSHIFT);
	}
	else
	{
		// ゲームパッド操作
		// 接続されているゲームパッド１の情報を取得
		InputManager::JOYPAD_IN_STATE padState =
			ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);
		// アナログキーの入力値から方向を取得
		dir = ins.GetDirectionXZAKey(padState.AKeyLX, padState.AKeyLY);

		isDash_ = ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1,
			InputManager::JOYPAD_BTN::L_TRIGGER);
	}

	// ダッシュ速度を歩行速度の2倍にする(書き方が違うけどif文と同じ意味)
	float movePow = isDash_ ? PLAYER_DASH_MOVE : PLAYER_MOVE;

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
		pos_ = VAdd(pos_, VScale(moveDir_, movePow));

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

}
