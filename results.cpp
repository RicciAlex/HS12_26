//=============================================================================
//
// results.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "results.h"
#include "object2D.h"
#include "inputKeyboard.h"
#include "application.h"
#include "UIString.h"
#include "debugProc.h"

//コンストラクタ
CResults::CResults()
{

}

//デストラクタ
CResults::~CResults()
{

}

//初期化処理
HRESULT CResults::Init(void)
{
	CObject_2D* pObj = CObject_2D::Create();

	if (!pObj)
	{
		return -1;
	}

	pObj->SetPos(D3DXVECTOR3((float)SCREEN_WIDTH * 0.5f, (float)SCREEN_WIDTH * 0.5f, 0.0f));
	pObj->SetSize(D3DXVECTOR2((float)SCREEN_WIDTH * 0.5f, (float)SCREEN_WIDTH * 0.5f));
	pObj->SetTexture(CObject::TEXTURE_RESULT);
	pObj->SetPriority(4);

	CUIString* pString = CUIString::Create(D3DXVECTOR3((float)SCREEN_WIDTH * 0.40f, (float)SCREEN_HEIGHT * 0.3f, 0.0f), D3DXVECTOR2(350.0f, 50.0f), ColorYellow, "RESULTS", 5);

	//サウンドの再生
	CApplication::GetSound()->Play(CSound::SOUND_LABEL_BGM_RESULT);

	if (m_pScoreUI != nullptr)
	{//スコアUIの更新処理

	 //スコアを取得して、4桁の文字列に変換する
		//int nScore = m_pScore->GetScore();
		//std::string str = std::to_string(nScore);
		//std::string begin = {};

		//if (str.size() < 4)
		//{
		//	for (int nCnt = 0; nCnt < 4 - (int)str.size(); nCnt++)
		//	{
		//		begin += '0';
		//	}

		//	begin += str;
		//}
		//else
		//{
		//	begin = str;
		//}

		//const char* pStr = begin.c_str();
		//m_pScoreUI->ChangeString(pStr);				//UIの文字列の設定

		//UIの生成
		//m_pScoreUI = CUIString::Create(D3DXVECTOR3(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f)), D3DXVECTOR2(500.0f, 50.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), pStr, 5);
	}

	return S_OK;
}

//終了処理
void CResults::Uninit(void)
{
	//サウンドを止める
	CApplication::GetSound()->Stop();
}

//更新処理
void CResults::Update(void)
{
	if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN))
	{
		CApplication::SetFade(CApplication::MODE_RANKING);
	}
}



//=============================================================================
//
//								静的関数
//
//=============================================================================



//生成処理
CResults* CResults::Create(void)
{
	CResults* pResults = new CResults;

	if (FAILED(pResults->Init()))
	{
		return nullptr;
	}

	return pResults;
}
