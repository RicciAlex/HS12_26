//=============================================================================
//
// スコア処理 [score.cpp]
// Author : KADO TAKUMA
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "score.h"
#include "rendering.h"
#include "application.h"
#include "number.h"
#include "game.h"
#include <assert.h>

//=============================================================================
// コンストラクタ
//=============================================================================
CScore::CScore() : 
	CObject(5),
	m_nScore(0),
	m_nSaveScore(0)
{
	for (int nCnt = 0; nCnt < SCORE_DIGIT; nCnt++)
	{
		m_apNumber[nCnt] = nullptr;
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CScore::~CScore()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScore::Init()
{
	//自身のサイズを取得
	D3DXVECTOR2 size = D3DXVECTOR2(50.0f, 80.0f);

	for (int nCnt = 0; nCnt < m_nDigit; nCnt++)
	{
		m_apNumber[nCnt] = CNumber::Create(D3DXVECTOR3(m_pos.x + (nCnt * size.x) - (m_nDigit * size.x / 2) + (size.x / 2), m_pos.y, m_pos.z), m_size);
	}

	//変数の初期化
	m_nScore = 0;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CScore::Uninit()
{
	for (int nCnt = 0; nCnt < m_nDigit; nCnt++)
	{
		if (m_apNumber[nCnt] != nullptr)
		{
			m_apNumber[nCnt]->Uninit();
			m_apNumber[nCnt] = nullptr;
		}
	}

	////インスタンスの解放処理
	//Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CScore::Update()
{
	//スコアを目的の値まで1ずつ増やす処理
	if (m_nSaveScore > m_nScore)
	{
		for (int nCnt = 0; nCnt < 3; nCnt++)
		{
			CGame::GetScore()->AddScore(1);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CScore::Draw()
{

}

//=============================================================================
// スコア生成処理
//=============================================================================
CScore * CScore::Create(bool bCalcDigit, int nScore, int nDigit, D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	//ポインタ宣言
	CScore *pScore = nullptr;

	//インスタンス生成
	pScore = new CScore;

	if (pScore != nullptr)
	{//ポインタが存在したら実行
		pScore->SetDigit(nDigit);
		pScore->SetPos(pos);
		pScore->SetSize(size);
		pScore->SetCalcDigit(bCalcDigit);
		pScore->Init();
		pScore->SetScore(nScore);
	}
	else
	{//ポインタが虚無だったら実行
		assert(false);
	}

	return pScore;
}

//=============================================================================
// スコアの設定処理
//=============================================================================
void CScore::SetScore(int nScore)
{
	//各桁の数字を格納
	int aPosTex[SCORE_DIGIT];

	//値格納
	m_nScore = nScore;

	if (m_bCalcDigit)
	{
		//桁数を計算する処理
		CalcDigit(nScore);
	}

	//スコア計算式(ループver)
	for (int nCnt = 0; nCnt < m_nDigit; nCnt++)
	{
		aPosTex[nCnt] = m_nScore % ((int)pow(10, (m_nDigit - (nCnt + 1))) * 10) / (int)pow(10, (m_nDigit - (nCnt + 1)));
		m_apNumber[nCnt]->SetAnimPattern(aPosTex[nCnt]);
	}

	////スコア計算式
	//aPosTex[0] = m_nScore % 100000 / 10000;
	//aPosTex[1] = m_nScore % 10000 / 1000;
	//aPosTex[2] = m_nScore % 1000 / 100;
	//aPosTex[3] = m_nScore % 100 / 10;
	//aPosTex[4] = m_nScore % 10 / 1;

//	//テクスチャ座標の設定
//	for (int nCnt = 0; nCnt < m_nDigit; nCnt++)
//	{
//		//float fShiftWidth = 1.0f / 10;
//		//m_apNumber[nCnt]->SetUV((aPosTex[nCnt] * fShiftWidth), (fShiftWidth + (aPosTex[nCnt] * fShiftWidth)), 0.0f, 1.0f);
//
//	}
}

//=============================================================================
// スコアの加算処理
//=============================================================================
void CScore::AddScore(int nValue)
{
	SetScore(m_nScore + nValue);

	////スコアの取得処理
	//CRanking::GetRanking(m_nScore);
}

//=============================================================================
// スコアの減算処理
//=============================================================================
void CScore::SubScore(int nValue)
{
	SetScore(m_nScore - nValue);
}

//=============================================================================
// 保存スコアの加算処理
//=============================================================================
void CScore::AddSaveScore(int nValue)
{
	m_nSaveScore += nValue;
}

//=============================================================================
// 桁数を計算する処理
//=============================================================================
void CScore::CalcDigit(int nScore)
{
	//自身のサイズを取得
	D3DXVECTOR2 size = D3DXVECTOR2(50.0f, 80.0f);

	//自身の座標を取得
	D3DXVECTOR3 pos = GetPos();

	//前の桁数保存
	int nBeforeDigit = m_nDigit;

	//桁数の更新
	m_nDigit = 1;
	while ((nScore /= 10) != 0)
	{
		m_nDigit++;
	}

	//保存した桁数と新しい桁数が違ったら一旦全部消す
	if (m_nDigit != nBeforeDigit)
	{
		for (int nCnt = 0; nCnt < nBeforeDigit; nCnt++)
		{
			if (m_apNumber[nCnt] != nullptr)
			{
				m_apNumber[nCnt]->SetShow(false);
			}
		}

		//インスタンス生成
		for (int nCnt = 0; nCnt < m_nDigit; nCnt++)
		{
			m_apNumber[nCnt]->SetShow(true);
			//m_apNumber[nCnt] = CNumber::Create(D3DXVECTOR3(m_pos.x + (nCnt * size.x) - (m_nDigit * size.x / 2) + (size.x / 2), m_pos.y, m_pos.z), m_size);
		}

		SetPos(pos);
	}
}

//=============================================================================
// 座標設定処理
//=============================================================================
void CScore::SetPos(D3DXVECTOR3 pos)
{
	//自身のサイズを取得
	D3DXVECTOR2 size = D3DXVECTOR2(50.0f, 80.0f);

	//座標
	m_pos = pos;

	//ネオンボールライフの座標設定
	for (int nCnt = 0; nCnt < m_nDigit; nCnt++)
	{
		if (m_apNumber[nCnt] != nullptr)
		{
			m_apNumber[nCnt]->SetPos(D3DXVECTOR3((m_pos.x + (nCnt * size.x)) - (m_nDigit * size.x / 2) + (size.x / 2), m_pos.y, m_pos.z));
		}
	}
}
