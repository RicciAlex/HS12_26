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

	return S_OK;
}

//終了処理
void CGame::Uninit(void)
{
	
}

//更新処理
void CGame::Update(void)
{
	
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