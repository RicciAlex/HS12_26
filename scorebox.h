//=============================================================================
//
// スコアボックス処理 [scorebox.h]
// Author : KADO TAKUMA
//
//=============================================================================
#ifndef _SCOREBOX_H_
#define _SCOREBOX_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"
#include "object2D.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define SCOREBOX_TEXTURE	(1)		// 使用するテクスチャの枚数
#define SCOREBOX_WIDTH	(300.0f)	// スコアボックスの幅
#define SCOREBOX_HEIGHT	(80.0f)	// スコアボックスの高さ

//=============================================================================
// クラスの定義
//=============================================================================
class CScoreBox : public CObject2D
{
public:
	//--------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------
	CScoreBox(int nPriority = PRIORITY_LEVEL3);
	~CScoreBox() override;

	//--------------------------------
	// メンバー関数
	//--------------------------------
	HRESULT Init(void) override;				// 初期化処理
	void Uninit(void) override;					// 終了処理
	void Update(void) override;					// 更新処理
	void Draw(void) override;					// 描画処理
	static CScoreBox *Create(D3DXVECTOR3 pos);	// スコアボックスの生成関数
	static HRESULT Load(void);					// テクスチャの読み込み関数
	static void UnLoad(void);					// テクスチャの破棄関数

private:
	//--------------------------------
	// メンバー変数
	//--------------------------------
	static LPDIRECT3DTEXTURE9 m_pTexture;		// テクスチャのポインタ
};

#endif