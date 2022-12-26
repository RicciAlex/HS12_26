//=============================================================================
//
// results.h
// Author : Ricci Alex
//
//=============================================================================
#ifndef _RESULTS_H
#define _RESULTS_H

//=============================================================================
//インクルードファイル
//=============================================================================
#include "mode.h"

//=============================================================================
//前方宣言
//=============================================================================
class CUIString;

class CResults : public CMode
{
public:
	CResults();
	~CResults() override;

	HRESULT Init(void) override;				//初期化処理
	void Uninit(void) override;					//終了処理
	void Update(void) override;					//更新処理

	static CResults* Create(void);				//生成処理

private:
	CUIString *m_pScoreUI;

};



#endif