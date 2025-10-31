#include <DxLib.h>
#include "../../Application.h"
#include "Stage.h"

Stage::Stage(void)
{
}

Stage::~Stage(void)
{
}

void Stage::Init(void)
{

	// 外部ファイルの３Ｄモデルをロード
	modelId_ = MV1LoadModel("Data/Model/Stage.mv1");

	pos_ = { 0.0f,80.0f,0.0f };

	MV1SetPosition(modelId_, pos_);

}

void Stage::Update(void)
{
}

void Stage::Draw(void)
{
	// ロードされた３Ｄモデルを画面に描画
	MV1DrawModel(modelId_);

}

void Stage::Release(void)
{
	// ロードされた３Ｄモデルをメモリから解放
	MV1DeleteModel(modelId_);

}

int Stage::GetModelId(void) const
{
	return modelId_;
}
