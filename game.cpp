//=============================================================================
//
// game.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "game.h"
#include "player.h"
#include "score.h"
#include "scorebox.h"
#include "timer.h"
#include "timerbox.h"
#include "inputKeyboard.h"

CScore* CGame::m_pScore = nullptr;
CTimer* CGame::m_pTimer = nullptr;

//コンストラクタ
CGame::CGame() : CMode(true)
{
	m_pPlayer = nullptr;
}

//デストラクタ
CGame::~CGame()
{

}

//初期化処理
HRESULT CGame::Init(void)
{
	m_pPlayer = nullptr;

	CScoreBox::Create(D3DXVECTOR3(640.0f, 50.0f, 0.0F));
	//CTimerBox::Create(D3DXVECTOR3(820.0f, 50.0f, 0.0F));

	m_pScore = CScore::Create(false, 0, 5, D3DXVECTOR3(640.0f, 60.0f, 0.0F), D3DXVECTOR2(18.0f, 25.0f));
	//m_pTimer = CTimer::Create(false, 0, 5, D3DXVECTOR3(820.0f, 60.0f, 0.0F), D3DXVECTOR2(18.0f, 25.0f));

	return S_OK;
}

//終了処理
void CGame::Uninit(void)
{
	m_pScore->Uninit();
}

//更新処理
void CGame::Update(void)
{
	if (CInputKeyboard::GetKeyboardPress(DIK_M))
	{
		m_pScore->AddScore(1);
	}
}


//プレイヤーの設定処理
void CGame::SetPlayer(CPlayer* pPlayer)
{
	m_pPlayer = pPlayer;
}

//プレイヤーの取得処理
CPlayer* CGame::GetPlayer(void)
{
	return m_pPlayer;
}