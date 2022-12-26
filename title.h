//=============================================================================
//
// title.h
// Author : Ricci Alex
//
//=============================================================================
#ifndef _TITLE_H
#define _TITLE_H

//=============================================================================
//インクルードファイル
//=============================================================================
#include "mode.h"

//=============================================================================
//前方宣言
//=============================================================================
class CObject_2D;

//マクロ定義
#define MAX_TITLE			(4)			//ポリゴンの最大数

class CTitle : public CMode
{
public:
	CTitle();
	~CTitle() override;

	HRESULT Init(void) override;				//初期化処理
	void Uninit(void) override;					//終了処理
	void Update(void) override;					//更新処理

	static CTitle* Create(void);				//生成処理

private:

	CObject_2D *m_pObj2D[MAX_TITLE];
	int m_nCounter;									//点滅時間
	int m_nSelect;									//選んでいる番号
};



#endif