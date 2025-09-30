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

	// サイコロモデルのZ軸回転
	diceAngles_.z += AsoUtility::Deg2RadF(0.7f);
	SyncDice();
}

void Player::Draw(void)
{
	ActorBase::Draw();

	MV1DrawModel(diceModelId_);

	// デバッグ表示
	DrawFormatString(
		0, 100, 0xffffff,
		"サイコロ角度　 ：(% .1f, % .1f, % .1f)",
		AsoUtility::Rad2DegF(diceAngles_.x),
		AsoUtility::Rad2DegF(diceAngles_.y),
		AsoUtility::Rad2DegF(diceAngles_.z)
		);

	DrawFormatString(
		0, 50, 0xffffff,
		"キャラ角度　 ：(% .1f, % .1f, % .1f)",
		AsoUtility::Rad2DegF(angles_.x),
		AsoUtility::Rad2DegF(angles_.y),
		AsoUtility::Rad2DegF(angles_.z)
	);

}

void Player::Release(void)
{
	ActorBase::Release();

	// モデルの解放
	MV1DeleteModel(diceModelId_);

}

void Player::InitLoad(void)
{
	// モデル読み込み
	modelId_ = MV1LoadModel((Application::PATH_MODEL + "Player/Player.mv1").c_str());

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
	// ここに個別の初期化処理を追加できる
	InitDice();

}

void Player::InitDice(void)
{
	// サイコロモデル読み込み
	diceModelId_ = MV1LoadModel((Application::PATH_MODEL + "Dice.mv1").c_str());

	diceLocalPos_ = { 200.0f,100.0f,0.0f };
	MV1SetPosition(diceModelId_, diceLocalPos_);

	diceAngles_ = { 0.0f, 0.0f, 0.0f };
	diceLocalAngles_ = { 0.0f,AsoUtility::Deg2RadF(0.0f), 0.0f };

	MATRIX diceMat = MatrixUtility::Multiplication(diceLocalAngles_, diceAngles_);

	MV1SetRotationMatrix(diceModelId_, diceMat);

	diceScales_ = { 0.1f, 0.1f, 0.1f };
	MV1SetScale(diceModelId_, diceScales_);

}

void Player::SyncDice(void)
{
	// サイコロモデルの回転行列
	MATRIX diceMat = MatrixUtility::Multiplication(diceLocalAngles_, diceAngles_);
	// 親の回転行列
	MATRIX parentMat = MatrixUtility::GetMatrixRotateXYZ(angles_);
	// 親子の回転行列を合成(子:サイコロ, 親:プレイヤーと指定すると親⇒子の順に適用される)
	MATRIX mat = MMult(diceMat, parentMat);
	// 回転行列をモデルに反映
	MV1SetRotationMatrix(diceModelId_, mat);

	// サイコロのローカル座標を親の回転行列で回転
	VECTOR localRotPos_ = VTransform(diceLocalPos_, parentMat);
	
	// サイコロのワールド座標
	dicePos_ = VAdd(localRotPos_, pos_);
	MV1SetPosition(diceModelId_, dicePos_);

}

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
		// アニメーションを攻撃にする
		animationController_->Play(static_cast<int>(ANIM_TYPE::ATTACK),false);
	}

}

void Player::playerDamage(void)
{

}

void Player::Move(void)
{
	auto& ins = InputManager::GetInstance();

	// 攻撃中かチェック
	int currentAnim = animationController_->GetPlayType();
	bool isAttacking = (currentAnim == static_cast<int>(ANIM_TYPE::ATTACK));

	// 攻撃アニメーションが終わったらIDLEに戻す
	if (isAttacking)
	{
		if (animationController_->IsEnd())
		{
			animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE));
		}
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
			animationController_->Play(static_cast<int>(ANIM_TYPE::RUN));
		}
		else
		{
			// アニメーションを歩きにする
			animationController_->Play(static_cast<int>(ANIM_TYPE::WALK));
		}
	}
	else
	{

		// アニメーションをIDLEにする
		animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE));

	}

	playerAttack();

}
