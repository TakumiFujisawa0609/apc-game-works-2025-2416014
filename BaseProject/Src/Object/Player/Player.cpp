#include <DxLib.h>
#include "../../Application.h"
#include "../../Common/AnimationController.h"
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

	angles_ = { 0.0f, 0.0f, 0.0f };
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

}

void Player::Update(void)
{

	// モデルのY軸回転
	angles_.y += AsoUtility::Deg2RadF(0.7f);
	
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

	// アニメーションの更新
	animationController_->Update();

}

void Player::Draw(void)
{
	MV1DrawModel(modelId_);

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

	// アニメーションコントローラの解放
	animationController_->Release();
	delete animationController_;


}
