//=============================================================================
//
// tutorial.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "tutorial.h"
#include "object2D.h"
#include "inputKeyboard.h"
#include "application.h"
#include "AnimateUI.h"

//コンストラクタ
CTutorial::CTutorial()
{

}

//デストラクタ
CTutorial::~CTutorial()
{

}

//初期化処理
HRESULT CTutorial::Init(void)
{
	CObject_2D* pObj = CObject_2D::Create();

	if (!pObj)
	{
		return -1;
	}

	pObj->SetPos(D3DXVECTOR3((float)SCREEN_WIDTH * 0.5f, (float)SCREEN_WIDTH * 0.5f, 0.0f));
	pObj->SetSize(D3DXVECTOR2((float)SCREEN_WIDTH * 0.5f, (float)SCREEN_WIDTH * 0.5f));
	pObj->SetColor(ColorCyan);
	pObj->SetPriority(4);

	//CUIString* pString = CUIString::Create(D3DXVECTOR3((float)SCREEN_WIDTH * 0.40f, (float)SCREEN_HEIGHT * 0.3f, 0.0f), D3DXVECTOR2(350.0f, 50.0f), ColorYellow, "RESULTS", 5);

	//サウンドの再生
	CApplication::GetSound()->Play(CSound::SOUND_LABEL_BGM_TITLE);

	return S_OK;
}

//終了処理
void CTutorial::Uninit(void)
{
	//サウンドを止める
	CApplication::GetSound()->Stop();
}

//更新処理
void CTutorial::Update(void)
{
	if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN))
	{
		CApplication::SetFade(CApplication::MODE_FIRST_STAGE);
	}
}



//=============================================================================
//
//								静的関数
//
//=============================================================================



//生成処理
CTutorial* CTutorial::Create(void)
{
	CTutorial* pTitle = new CTutorial;

	if (FAILED(pTitle->Init()))
	{
		return nullptr;
	}

	return pTitle;
}
