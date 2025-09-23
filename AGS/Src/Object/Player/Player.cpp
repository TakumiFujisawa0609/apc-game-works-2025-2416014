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

void Player::Init(void)
{
	// モデル読み込み
	modelId_ = MV1LoadModel((Application::PATH_MODEL + "Player/Player.mv1").c_str());

	angles_ = { 0.0f, 0.0f, 0.0f};
	localAngles_ = { 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f };


	//// モデルの回転行列
	//MATRIX mat = MatrixUtility::GetMatrixRotateXYZ(angles_);
	////mat = MMult(mat, MGetRotX(angles_.x));
	////mat = MMult(mat, MGetRotY(angles_.y));
	////mat = MMult(mat, MGetRotZ(angles_.z));

	//// モデルのローカル回転行列
	//MATRIX localMat = MatrixUtility::GetMatrixRotateXYZ(localAngles_);
	////localMat = MMult(localMat, MGetRotX(localAngles_.x));
	////localMat = MMult(localMat, MGetRotY(localAngles_.y));
	////localMat = MMult(localMat, MGetRotZ(localAngles_.z));

	MATRIX mat = MatrixUtility::Multiplication(localAngles_, angles_);

	// 回転行列をモデルに反映
	MV1SetRotationMatrix(modelId_, mat);

	// モデルの位置設定
	pos_ = AsoUtility::VECTOR_ZERO;
	MV1SetPosition(modelId_, pos_);

	// モデルアニメーション制御の初期化
	animationController_ = new AnimationController(modelId_);

	animationController_->Add(
		static_cast<int>(ANIM_TYPE::IDLE), 30.0f, Application::PATH_MODEL + "Player/Idle.mv1");
	animationController_->Add(
		static_cast<int>(ANIM_TYPE::WALK), 30.0f, Application::PATH_MODEL + "Player/Walk.mv1");

	// 初期アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::WALK));

	scales_ = { 1.0f, 1.0f, 1.0f };
	MV1SetScale(modelId_, scales_);

	InitDice();

}

void Player::Update(void)
{

	//// モデルのY軸回転
	//angles_.y += AsoUtility::Deg2RadF(0.7f);

	// サイコロモデルのZ軸回転
	diceAngles_.z += AsoUtility::Deg2RadF(0.7f);

	//// モデルの回転行列
	//MATRIX mat = MatrixUtility::GetMatrixRotateXYZ(angles_);
	////mat = MMult(mat, MGetRotX(angles_.x));
	////mat = MMult(mat, MGetRotY(angles_.y));
	////mat = MMult(mat, MGetRotZ(angles_.z));

	//// モデルのローカル回転行列
	//MATRIX localMat = MatrixUtility::GetMatrixRotateXYZ(localAngles_);
	////localMat = MMult(localMat, MGetRotX(localAngles_.x));
	////localMat = MMult(localMat, MGetRotY(localAngles_.y));
	////localMat = MMult(localMat, MGetRotZ(localAngles_.z));

	//// 行列の合成(子, 親と指定すると親⇒子の順に適用される)
	//mat = MMult(localMat, mat);

	MATRIX mat = MatrixUtility::Multiplication(localAngles_, angles_);

	// 回転行列をモデルに反映
	MV1SetRotationMatrix(modelId_, mat);

	// キャラクター移動
	Move();
	MV1SetPosition(modelId_, pos_);

	// サイコロモデルの回転行列
	SyncDice();


	// アニメーションの更新
	animationController_->Update();

}

void Player::Draw(void)
{
	MV1DrawModel(modelId_);

	MV1DrawModel(diceModelId_);
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
	// ロードされた３Ｄモデルをメモリから解放
	MV1DeleteModel(modelId_);

	MV1DeleteModel(diceModelId_);

	// アニメーションコントローラの解放
	animationController_->Release();
	delete animationController_;


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

void Player::Move(void)
{
	auto& ins = InputManager::GetInstance();

	// カメラの角度を取得
	VECTOR camAngles = 
		SceneManager::GetInstance().GetCamera()->GetAngles();
	
	VECTOR dir = AsoUtility::VECTOR_ZERO;

	// ゲームパッドが接続されている数で処理を分ける
	if (GetJoypadNum() == 0)
	{
		// キーボード操作
		if (ins.IsNew(KEY_INPUT_W)) { dir = AsoUtility::DIR_F; }
		if (ins.IsNew(KEY_INPUT_A)) { dir = AsoUtility::DIR_L; }
		if (ins.IsNew(KEY_INPUT_S)) { dir = AsoUtility::DIR_B; }
		if (ins.IsNew(KEY_INPUT_D)) { dir = AsoUtility::DIR_R; }
	}
	else
	{
		// ゲームパッド操作
		// 接続されているゲームパッド１の情報を取得
		InputManager::JOYPAD_IN_STATE padState =
			ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);
		// アナログキーの入力値から方向を取得
		dir = ins.GetDirectionXZAKey(padState.AKeyLX, padState.AKeyLY);

	}

	// WASDでカメラを移動させる
	const float movePow = 3.0f;

	if (!AsoUtility::EqualsVZero(dir))
	{
		// XYZの回転行列
		// XZ平面移動にする場合は、XZの回転を考慮しないようにする
		MATRIX mat = MGetIdent();
		//mat = MMult(mat, MGetRotX(camAngles.x));
		mat = MMult(mat, MGetRotY(camAngles.y));
		//mat = MMult(mat, MGetRotZ(camAngles.z));

		// 回転行列を使用して、ベクトルを回転させる
		VECTOR moveDir = VTransform(dir, mat);

		// 方向×スピードで移動量を作って、座標に足して移動
		pos_ = VAdd(pos_, VScale(moveDir, movePow));
	}


}
