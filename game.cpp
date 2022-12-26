//=============================================================================
//
// game.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "game.h"
#include "player.h"
#include "score.h"
#include "inputKeyboard.h"

CScore* CGame::m_pScore = nullptr;

//�R���X�g���N�^
CGame::CGame() : CMode(true)
{
	m_pPlayer = nullptr;
}

//�f�X�g���N�^
CGame::~CGame()
{

}

//����������
HRESULT CGame::Init(void)
{
	m_pPlayer = nullptr;

	//�X�R�A����
	m_pScore = CScore::Create(false, 1500, 5, D3DXVECTOR3(760.0f, 55.0f, 0.0F), D3DXVECTOR2(25.0f, 40.0f));

	return S_OK;
}

//�I������
void CGame::Uninit(void)
{
	m_pScore->Uninit();
}

//�X�V����
void CGame::Update(void)
{
	if (CInputKeyboard::GetKeyboardPress(DIK_M))
	{
		m_pScore->AddScore(100);
	}
}


//�v���C���[�̐ݒ菈��
void CGame::SetPlayer(CPlayer* pPlayer)
{
	m_pPlayer = pPlayer;
}

//�v���C���[�̎擾����
CPlayer* CGame::GetPlayer(void)
{
	return m_pPlayer;
}