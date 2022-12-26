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
#include "inputKeyboard.h"

CScore* CGame::m_pScore = nullptr;

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

	//スコア生成
	m_pScore = CScore::Create(false, 1500, 5, D3DXVECTOR3(760.0f, 55.0f, 0.0F), D3DXVECTOR2(25.0f, 40.0f));

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
		m_pScore->AddScore(100);
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