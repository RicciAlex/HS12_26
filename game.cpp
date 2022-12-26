//=============================================================================
//
// game.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "application.h"
#include "game.h"
#include "player.h"
#include "score.h"
#include "scorebox.h"
#include "timer.h"
#include "timerbox.h"
#include "inputKeyboard.h"
#include "countdown.h"

CScore* CGame::m_pScore = nullptr;
CTimer* CGame::m_pTimer = nullptr;
CCountDown* CGame::m_pCountDown = nullptr;

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

	CScoreBox::Create(D3DXVECTOR3(640.0f, 50.0f, 0.0F));
	//CTimerBox::Create(D3DXVECTOR3(820.0f, 50.0f, 0.0F));

	m_pScore = CScore::Create(false, 0, 5, D3DXVECTOR3(640.0f, 60.0f, 0.0F), D3DXVECTOR2(18.0f, 25.0f));
	//m_pTimer = CTimer::Create(false, 0, 5, D3DXVECTOR3(820.0f, 60.0f, 0.0F), D3DXVECTOR2(18.0f, 25.0f));

	m_pCountDown = CCountDown::Create(D3DXVECTOR3(640.0f, 360.0f, 0.0f));

	//�T�E���h�̍Đ�
	CApplication::GetSound()->Play(CSound::SOUND_LABEL_BGM_GAME);

	return S_OK;
}

//�I������
void CGame::Uninit(void)
{
	m_pScore->Uninit();

	//�T�E���h���~�߂�
	CApplication::GetSound()->Stop();
}

//�X�V����
void CGame::Update(void)
{
	if (CInputKeyboard::GetKeyboardPress(DIK_M))
	{
		m_pScore->AddScore(1);
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