//=============================================================================
//
// ランキング
// Author:髙野馨將
//
//=============================================================================
#ifndef _RANKING_H_                        //このマクロ定義がされてなかったら
#define _RANKING_H_                        //二重インクルード防止のマクロ定義

#include "mode.h"

//前方宣言
class CObject2D;
class CFade;
class CUIString;

//マクロ定義
#define MAX_RANKING			(8)			//ランキングの桁数
#define MAX_RANKING_RANK    (5)			//ランキングの順位分
#define MAX_POLIGON			(3)			//ポリゴンの最大数

//ランキングクラス
class CRanking : public CMode
{
public:
	CRanking();
	~CRanking();

	//プロトタイプ宣言
	HRESULT Init();
	void Uninit();
	void Update();
	void Load();
	void Save();
	void SetRankingScore();
	static void GetRanking(int nRanking) { m_nRanking = nRanking; }
	static CRanking *Create();    //ランキングの生成

private:
	static int m_nRanking;
	static int m_nData[MAX_RANKING_RANK];
	CObject_2D* m_pObj2D[MAX_POLIGON];
	CUIString* m_pScoreUI;
	CFade *m_pFade;
	int m_nCounter;									//点滅時間
	int m_nSelect;									//選んでいる番号
};

#endif;