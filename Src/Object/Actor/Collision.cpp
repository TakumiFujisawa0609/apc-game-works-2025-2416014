#include "Collision.h"
#include "../Stage/Stage.h"

Collision* Collision::instance_ = nullptr;

VECTOR Collision::CollCheckStageCapsule(VECTOR upPos, VECTOR downPos, float radius)
{
	// 移動方向の蓄積用
	VECTOR movePos = { 0.0f,0.0f,0.0f };

	// カプセルとの当たり判定
	MV1_COLL_RESULT_POLY_DIM hits = MV1CollCheck_Capsule
	(
		stage_->GetModelId(),
		-1,
		upPos,
		downPos,
		radius
	);

	// 衝突したポリゴン数分の検索
	for (int i = 0; i < hits.HitNum; i++)
	{
		// ポリゴン1枚に分解
		MV1_COLL_RESULT_POLY hit = hits.Dim[i];

		// ポリゴン検索を制限(全てすると重いので)
		for (int tryCnt = 0; tryCnt < 10; tryCnt++)
		{
			// 最初の衝突判定で検出した衝突ポリゴン1枚と衝突判定を取る
			int pHit = HitCheck_Capsule_Triangle
			(
				upPos,
				downPos,
				radius,
				hit.Position[0],
				hit.Position[1],
				hit.Position[2]
			);

			// カプセルとポリゴンが当たっていた
			if (pHit)
			{
				// 当たっていたので座標をポリゴンの法線方向に移動させる
				movePos = VAdd(movePos, VScale(hit.Normal, 1.0f));

				// カプセルの座標も移動させる
				upPos = VAdd(upPos, VScale(hit.Normal, 1.0f));
				downPos = VAdd(downPos, VScale(hit.Normal, 1.0f));

				//複数当たっている可能性があるので再検索
				continue;
			}
		}
	}

	// 検出した地面ポリゴン情報の後始末
	MV1CollResultPolyDimTerminate(hits);
	return  movePos;
}

Collision::Collision()
	:stage_(nullptr)
{

}

Collision::~Collision()
{
}
