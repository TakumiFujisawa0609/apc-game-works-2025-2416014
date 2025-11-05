#pragma once
#include <string>
#include <DxLib.h>

class UiManager
{
public:

	// コンストラクタ
	UiManager(void);
	// デストラクタ
	virtual ~UiManager(void);

	// 初期化
	void Init(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void Release(void);

	// 座標を取得・設定
	const VECTOR& GetPos(void) const;
	void SetPos(const VECTOR& pos);

protected:

	// 情報
	int img_;		//画像
	VECTOR pos_;	//座標
	VECTOR scl_;	//大きさ
	VECTOR rot_;	//回転
	int alpha_;  // 透明度 0-255

	// 描画優先度（数字が大きいほど手前）
	int drawPriority_; 

	// 生存判定フラグ
	bool isAlive_;
	// 描画するかどうか
	bool isVisible_; 

	// アニメーション用タイマー
	float animTimer_;  

	// 色（デフォルトは白255,255,255）
	COLOR_U8 color_;  
	
	// リソースロード
	virtual void InitLoad(void) = 0;

	// 大きさ、回転、座標の初期化
	virtual void InitTransform(void) = 0;

	// 初期化後の個別処理
	virtual void InitPost(void) = 0;

};