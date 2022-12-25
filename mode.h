//=============================================================================
//
// mode.h
// Author : Ricci Alex
//
//=============================================================================
#ifndef _MODE_H
#define _MODE_H

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"


class CMode
{
public:

	CMode();								//コンストラクタ
	CMode(const bool bGame);				//コンストラクタ
	virtual ~CMode();						//デストラクタ
											
	virtual HRESULT Init(void) = 0;			//初期化処理
	virtual void Uninit(void) = 0;			//終了処理
	virtual void Update(void) = 0;			//更新処理

	const bool IsGame(void);				//ゲームであるかどうか

private:

	bool m_bGame;							//ゲームモードであるかどうか

};



#endif