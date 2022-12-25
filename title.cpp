//=============================================================================
//
// title.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "title.h"
#include "object2D.h"
#include "inputKeyboard.h"
#include "application.h"
#include "AnimateUI.h"

//コンストラクタ
CTitle::CTitle()
{

}

//デストラクタ
CTitle::~CTitle()
{

}

//初期化処理
HRESULT CTitle::Init(void)
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

	CAnimateUI::UIAnimation animInfo;
	animInfo.deltaSize = D3DXVECTOR2(0.5f, 0.25f);
	animInfo.nChangeFrame = 60;
	CAnimateUI* pLogo = CAnimateUI::Create(CObject::TEXTURE_TITLE, D3DXVECTOR3((float)SCREEN_WIDTH * 0.5f, (float)SCREEN_HEIGHT * 0.4f, 0.0f), D3DXVECTOR2(300.0f, 150.0f), ColorWhite, animInfo);
	pLogo->AnimateColor(true);

	return S_OK;
}

//終了処理
void CTitle::Uninit(void)
{

}

//更新処理
void CTitle::Update(void)
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
CTitle* CTitle::Create(void)
{
	CTitle* pTitle = new CTitle;

	if (FAILED(pTitle->Init()))
	{
		return nullptr;
	}

	return pTitle;
}
