#include <DxLib.h>
#include "../../Application.h"
#include "../../Common/AnimationController.h"
#include "../../Manager/Camera.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/InputManager.h"
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
	case Player::STATE::DODGE:
		UpdateDodge();
		break;
	case Player::STATE::ATTACK:
		UpdateAttack();
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

	// サイコロモデルのZ軸回転
	//diceAngles_.z += AsoUtility::Deg2RadF(0.7f);
	//SyncDice();
}

void Player::Draw(void)
{
	ActorBase::Draw();

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
	case Player::STATE::DODGE:
		DrawDodge();
		break;
	case Player::STATE::ATTACK:
		DrawAttack();
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

	DrawSphere3D(VGet(pos_.x, pos_.y + 100, pos_.z), collisionRadius_, 50, 0x0000ff, 0x0000ff, true);

}

void Player::Release(void)
{
	ActorBase::Release();

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
	case STATE::DODGE:
		ChangeDodge();
		break;
	case STATE::ATTACK:
		ChangeAttack();
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

float Player::GetcollisionRadius(void)
{
	return collisionRadius_;
}

void Player::InitLoad(void)
{
	// モデル読み込み
	modelId_ = MV1LoadModel((Application::PATH_MODEL + "Player/Hero.mv1").c_str());

}

void Player::InitTransform(void)
{
	// モデルの位置設定
	pos_ = AsoUtility::VECTOR_ZERO;

	// モデルの角度
	angles_ = { 0.0f, 0.0f, 0.0f };
	localAngles_ = { 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f };

	// モデルの大きさ設定
	scales_ = { 1.0f, 1.0f, 1.0f };

	// 角度から方向に変換する
	moveDir_ = { sinf(angles_.y), 0.0f, cosf(angles_.y) };

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
		static_cast<int>(ANIM_TYPE::ATTACK), 30.0f, Application::PATH_MODEL + "Player/Slash.mv1");
	// 未追加
	//animationController_->Add(
	//	static_cast<int>(ANIM_TYPE::DAMAGE), 30.0f, Application::PATH_MODEL + "Player/Damage.mv1");
	
	// 初期アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE));

}

void Player::InitPost(void)
{

	// 衝突判定用半径
	collisionRadius_ = PLAYER_RADIUS;

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

	//swordPos_ = MV1GetFramePosition(modelId_, 44);

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

	//MATRIX swordMat = MatrixUtility::Multiplication(swordLocalAngles_, swordAngles_);
	//MV1SetRotationMatrix(swordModelId_, swordMat);
	
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
	MATRIX localMat = MMult(scaleMat,swordMat);
	// 武器のローカル位置の変換行列を合成
	localMat = MMult(localMat, transMatPos);
	// 親子の回転行列を合成(子:武器, 親:手と指定すると親⇒子の順に適用される)
	MATRIX mat = MMult(localMat, handMat);
	

	// 回転行列をモデルに反映
	MV1SetMatrix(swordModelId_, mat);
	// 剣の位置を保存（当たり判定用）
	swordPos_ = MV1GetPosition(swordModelId_);
	
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

void Player::playerAttack(void)
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
			InputManager::JOYPAD_BTN::RIGHT);
	}

	if (isAttack)
	{
		ChangeAttack();
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

void Player::ChangeDodge(void)
{
}

void Player::ChangeAttack(void)
{
	// 攻撃アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::ATTACK), false);
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

void Player::UpdateDodge(void)
{
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

void Player::DrawDodge(void)
{
	MV1DrawModel(modelId_);

}

void Player::DrawAttack(void)
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

// プレイヤージャンプ処理
//void Player::playerJump(void)
//{
//	auto& ins = InputManager::GetInstance();
//
//	// 攻撃しているか判定
//	bool isJump;
//	isJump = false;
//
//	// ゲームパッドが接続されている数で処理を分ける
//	if (GetJoypadNum() == 0)
//	{
//		// キーボード操作
//		// アタックキー
//		isJump = ins.IsNew(KEY_INPUT_SPACE);
//	}
//	else
//	{
//		// ゲームパッド操作
//		// 接続されているゲームパッド１の情報を取得
//		InputManager::JOYPAD_IN_STATE padState =
//			ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);
//
//		isJump = ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1,
//			InputManager::JOYPAD_BTN::RIGHT);
//	}
//
//	if (isJump)
//	{
//		// アニメーションを攻撃にする
//		animationController_->Play(static_cast<int>(ANIM_TYPE::JUMP), false);
//	}
//
//}

void Player::Move(void)
{

	auto& ins = InputManager::GetInstance();

	// 攻撃中かチェック
	int currentAnim = animationController_->GetPlayType();
	bool isAttacking = (currentAnim == static_cast<int>(ANIM_TYPE::ATTACK));

	// 攻撃アニメーションが終わったらIDLEに戻す
	if (isAttacking)
	{
		UpdateAttack();
		return;  // 攻撃中は移動処理をスキップ
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
			InputManager::JOYPAD_BTN::LEFT);
	}

	// ダッシュ速度を歩行速度の2倍にする(書き方が違うけどif文と同じ意味)
	float movePow = isDash_ ? 6.0f : 3.0f;

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

		// 移動方向から角度に変換する
		//angles_.y = atan2f(moveDir_.x, moveDir_.z);

		// 方向×スピードで移動量を作って、座標に足して移動
		pos_ = VAdd(pos_, VScale(moveDir_, movePow));
	
		if (isDash_)
		{
			// アニメーションを走りにする
			ChangeRun();
		}
		else
		{
			// アニメーションを歩きにする
			ChangeWalk();
		}
	}
	else
	{

		// アニメーションをIDLEにする
		ChangeIdle();
	}

	playerAttack();

}
