//=============================================================================
//
// ランキング処理
// Author:髙野馨將
//
//=============================================================================
#include <stdio.h>
#include "application.h"
#include "inputKeyboard.h"
#include "object2D.h"
#include "rendering.h"
#include "UIString.h"
#include "ranking.h"
#include "fade.h"
#include "sound.h"
#include "debugProc.h"

//静的メンバ変数宣言
int CRanking::m_nData[MAX_RANKING_RANK];
int CRanking::m_nRanking;
//======================================================
//コンストラクタ
//======================================================
CRanking::CRanking() : CMode()
{
}

//======================================================
//デストラクタ
//======================================================
CRanking::~CRanking()
{
}

//======================================================
//初期化処理
//======================================================
HRESULT CRanking::Init(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//ポリゴンの生成
	m_pObj2D = CObject_2D::Create();
	m_pObj2D->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	m_pObj2D->SetSize(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 5));
	//m_pObj2D->SetTexture(CObject::TEXTURE_RANKING);
	m_pObj2D->SetPriority(5);

	//サウンドの再生
	CApplication::GetSound()->Play(CSound::SOUND_LABEL_BGM_RANKING);

	//ファイル読み込み処理
	Load();

	return S_OK;
}

//======================================================
//終了処理
//======================================================
void CRanking::Uninit(void)
{
	//サウンドを止める
	CApplication::GetSound()->Stop();

	if (m_pObj2D != nullptr)
	{
		m_pObj2D->Uninit();
		m_pObj2D = nullptr;
	}

	if (m_pScoreUI != nullptr)
	{
		m_pScoreUI->Uninit();
		m_pScoreUI = nullptr;
	}
}

//======================================================
//更新処理
//======================================================
void CRanking::Update(void)
{
	//キー入力
	if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN))
	{
		Save();
		CApplication::SetFade(CApplication::MODE_TITLE);
	}
}

//============================
//ファイルに書き出す
//============================
void CRanking::Save()
{
	FILE*pFile;		//ファイルポインタ宣言

	//ファイルを開く
	pFile = fopen("data/Save/ranking.txt", "w");

	if (pFile != NULL)
	{//ファイルが開けた場合
		for (int nCnt = 0; nCnt < MAX_RANKING_RANK; nCnt++)
		{
			//ファイルに数値を書き出す
			fprintf(pFile, "%d\n", m_nData[nCnt]);
		}
		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{//ファイルが開かなかった場合
		printf("***ファイルが開けませんでした***");
	}
}

//============================
//ファイルから読み込む
//============================
void CRanking::Load()
{
	FILE*pFile;		//ファイルポインタ宣言

	//ファイルを開く
	pFile = fopen("data/Save/ranking.txt", "r");

	if (pFile != NULL)
	{//ファイルが開けた場合
		for (int nCnt = 0; nCnt < MAX_RANKING_RANK; nCnt++)
		{
			//ファイルから数値を読み込む
			fscanf(pFile, "%d\n", &m_nData[nCnt]);
		}
		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{//ファイルが開かなかった場合
		printf("***ファイルが開けませんでした***");
	}
}

//====================================
//スコアの設定処理
//====================================
void CRanking::SetRankingScore()
{
	int aPosTexU[MAX_RANKING_RANK][MAX_RANKING];	//各桁の数字を格納

	//新しいスコアと古いスコアの比較
	for (int nCnt = 0; nCnt < MAX_RANKING_RANK +1; nCnt++)
	{
		if (m_nRanking > m_nData[4])
		{
			m_nData[4] = m_nRanking;
		}
	}
	//バブルソート
	for (int nCount = 0; nCount < MAX_RANKING_RANK -1; nCount++)
	{
		for (int nCount2 = nCount + 1; nCount2 < MAX_RANKING_RANK; nCount2++)
		{
			if (m_nData[nCount] < m_nData[nCount2])
			{
				int nMin = m_nData[nCount + 1];
				m_nData[nCount] = m_nData[nCount2];
				m_nData[nCount2] = nMin;
			}
		}
	}
	for (int nCntScore = 0; nCntScore < MAX_RANKING_RANK; nCntScore++)
	{
		aPosTexU[nCntScore][0] = m_nData[nCntScore] % 100000000 / 1000000;
		aPosTexU[nCntScore][1] = m_nData[nCntScore] % 10000000 / 1000000;
		aPosTexU[nCntScore][2] = m_nData[nCntScore] % 1000000 / 100000;
		aPosTexU[nCntScore][3] = m_nData[nCntScore] % 100000 / 10000;
		aPosTexU[nCntScore][4] = m_nData[nCntScore] % 10000 / 1000;
		aPosTexU[nCntScore][5] = m_nData[nCntScore] % 1000 / 100;
		aPosTexU[nCntScore][6] = m_nData[nCntScore] % 100 / 10;
		aPosTexU[nCntScore][7] = m_nData[nCntScore] % 10 / 1;
	}

	for (int nCntScore = 0; nCntScore < MAX_RANKING_RANK; nCntScore++)
	{
		//配列の確保
		int nScore[MAX_RANKING_RANK] = {};
		std::string str[MAX_RANKING_RANK] = {};
		std::string begin[MAX_RANKING_RANK] = {};
		const char* pStr[MAX_RANKING_RANK] = {};
		//スコアを取得して、8桁の文字列に変換する
		nScore[nCntScore] = m_nData[nCntScore];
		str[nCntScore] = std::to_string(nScore[nCntScore]);

		if (str[nCntScore].size() < MAX_RANKING)
		{
			for (int nCnt = 0; nCnt < MAX_RANKING - (int)str[nCntScore].size(); nCnt++)
			{
				begin[nCntScore] += '0';
			}

			begin[nCntScore] += str[nCntScore];
		}
		else
		{
			begin[nCntScore] = str[nCntScore];
		}

		pStr[nCntScore] = begin[nCntScore].c_str();

		//UIの生成
		m_pScoreUI = CUIString::Create(D3DXVECTOR3(800.0f,50.0f + (150.0f * nCntScore), 0.0f), D3DXVECTOR2(150.0f, 50.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), pStr[nCntScore], 5);
	}
}

//======================================================
//生成処理
//======================================================
CRanking *CRanking::Create()
{
	//動的確保
	CRanking *pRanking = new CRanking;

	if (pRanking != nullptr)
	{
		//初期化
		pRanking->Init();
		pRanking->SetRankingScore();
	}

	return pRanking;
}