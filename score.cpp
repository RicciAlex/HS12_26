//=============================================================================
//
// �X�R�A���� [score.cpp]
// Author : KADO TAKUMA
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "score.h"
#include "rendering.h"
#include "application.h"
#include "number.h"
#include "game.h"
#include <assert.h>

//=============================================================================
// �R���X�g���N�^
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
// �f�X�g���N�^
//=============================================================================
CScore::~CScore()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CScore::Init()
{
	//���g�̃T�C�Y���擾
	D3DXVECTOR2 size = D3DXVECTOR2(50.0f, 80.0f);

	for (int nCnt = 0; nCnt < m_nDigit; nCnt++)
	{
		m_apNumber[nCnt] = CNumber::Create(D3DXVECTOR3(m_pos.x + (nCnt * size.x) - (m_nDigit * size.x / 2) + (size.x / 2), m_pos.y, m_pos.z), m_size);
	}

	//�ϐ��̏�����
	m_nScore = 0;

	return S_OK;
}

//=============================================================================
// �I������
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

	////�C���X�^���X�̉������
	//Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CScore::Update()
{
	//�X�R�A��ړI�̒l�܂�1�����₷����
	if (m_nSaveScore > m_nScore)
	{
		for (int nCnt = 0; nCnt < 3; nCnt++)
		{
			CGame::GetScore()->AddScore(1);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CScore::Draw()
{

}

//=============================================================================
// �X�R�A��������
//=============================================================================
CScore * CScore::Create(bool bCalcDigit, int nScore, int nDigit, D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	//�|�C���^�錾
	CScore *pScore = nullptr;

	//�C���X�^���X����
	pScore = new CScore;

	if (pScore != nullptr)
	{//�|�C���^�����݂�������s
		pScore->SetDigit(nDigit);
		pScore->SetPos(pos);
		pScore->SetSize(size);
		pScore->SetCalcDigit(bCalcDigit);
		pScore->Init();
		pScore->SetScore(nScore);
	}
	else
	{//�|�C���^����������������s
		assert(false);
	}

	return pScore;
}

//=============================================================================
// �X�R�A�̐ݒ菈��
//=============================================================================
void CScore::SetScore(int nScore)
{
	//�e���̐������i�[
	int aPosTex[SCORE_DIGIT];

	//�l�i�[
	m_nScore = nScore;

	if (m_bCalcDigit)
	{
		//�������v�Z���鏈��
		CalcDigit(nScore);
	}

	//�X�R�A�v�Z��(���[�vver)
	for (int nCnt = 0; nCnt < m_nDigit; nCnt++)
	{
		aPosTex[nCnt] = m_nScore % ((int)pow(10, (m_nDigit - (nCnt + 1))) * 10) / (int)pow(10, (m_nDigit - (nCnt + 1)));
		m_apNumber[nCnt]->SetAnimPattern(aPosTex[nCnt]);
	}

	////�X�R�A�v�Z��
	//aPosTex[0] = m_nScore % 100000 / 10000;
	//aPosTex[1] = m_nScore % 10000 / 1000;
	//aPosTex[2] = m_nScore % 1000 / 100;
	//aPosTex[3] = m_nScore % 100 / 10;
	//aPosTex[4] = m_nScore % 10 / 1;

//	//�e�N�X�`�����W�̐ݒ�
//	for (int nCnt = 0; nCnt < m_nDigit; nCnt++)
//	{
//		//float fShiftWidth = 1.0f / 10;
//		//m_apNumber[nCnt]->SetUV((aPosTex[nCnt] * fShiftWidth), (fShiftWidth + (aPosTex[nCnt] * fShiftWidth)), 0.0f, 1.0f);
//
//	}
}

//=============================================================================
// �X�R�A�̉��Z����
//=============================================================================
void CScore::AddScore(int nValue)
{
	SetScore(m_nScore + nValue);

	////�X�R�A�̎擾����
	//CRanking::GetRanking(m_nScore);
}

//=============================================================================
// �X�R�A�̌��Z����
//=============================================================================
void CScore::SubScore(int nValue)
{
	SetScore(m_nScore - nValue);
}

//=============================================================================
// �ۑ��X�R�A�̉��Z����
//=============================================================================
void CScore::AddSaveScore(int nValue)
{
	m_nSaveScore += nValue;
}

//=============================================================================
// �������v�Z���鏈��
//=============================================================================
void CScore::CalcDigit(int nScore)
{
	//���g�̃T�C�Y���擾
	D3DXVECTOR2 size = D3DXVECTOR2(50.0f, 80.0f);

	//���g�̍��W���擾
	D3DXVECTOR3 pos = GetPos();

	//�O�̌����ۑ�
	int nBeforeDigit = m_nDigit;

	//�����̍X�V
	m_nDigit = 1;
	while ((nScore /= 10) != 0)
	{
		m_nDigit++;
	}

	//�ۑ����������ƐV������������������U�S������
	if (m_nDigit != nBeforeDigit)
	{
		for (int nCnt = 0; nCnt < nBeforeDigit; nCnt++)
		{
			if (m_apNumber[nCnt] != nullptr)
			{
				m_apNumber[nCnt]->SetShow(false);
			}
		}

		//�C���X�^���X����
		for (int nCnt = 0; nCnt < m_nDigit; nCnt++)
		{
			m_apNumber[nCnt]->SetShow(true);
			//m_apNumber[nCnt] = CNumber::Create(D3DXVECTOR3(m_pos.x + (nCnt * size.x) - (m_nDigit * size.x / 2) + (size.x / 2), m_pos.y, m_pos.z), m_size);
		}

		SetPos(pos);
	}
}

//=============================================================================
// ���W�ݒ菈��
//=============================================================================
void CScore::SetPos(D3DXVECTOR3 pos)
{
	//���g�̃T�C�Y���擾
	D3DXVECTOR2 size = D3DXVECTOR2(50.0f, 80.0f);

	//���W
	m_pos = pos;

	//�l�I���{�[�����C�t�̍��W�ݒ�
	for (int nCnt = 0; nCnt < m_nDigit; nCnt++)
	{
		if (m_apNumber[nCnt] != nullptr)
		{
			m_apNumber[nCnt]->SetPos(D3DXVECTOR3((m_pos.x + (nCnt * size.x)) - (m_nDigit * size.x / 2) + (size.x / 2), m_pos.y, m_pos.z));
		}
	}
}
