//=============================================================================
//
// �����L���O����
// Author:����]��
//
//=============================================================================
#include <stdio.h>
#include "application.h"
#include "inputKeyboard.h"
#include "object2D.h"
#include "rendering.h"
#include "UIString.h"
#include "ranking.h"
#include "fade.h"
#include "sound.h"
#include "debugProc.h"

//�ÓI�����o�ϐ��錾
int CRanking::m_nData[MAX_RANKING_RANK];
int CRanking::m_nRanking;
//======================================================
//�R���X�g���N�^
//======================================================
CRanking::CRanking() : CMode()
{
}

//======================================================
//�f�X�g���N�^
//======================================================
CRanking::~CRanking()
{
}

//======================================================
//����������
//======================================================
HRESULT CRanking::Init(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//�|���S���̐���
	m_pObj2D = CObject_2D::Create();
	m_pObj2D->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	m_pObj2D->SetSize(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 5));
	//m_pObj2D->SetTexture(CObject::TEXTURE_RANKING);
	m_pObj2D->SetPriority(5);

	//�T�E���h�̍Đ�
	CApplication::GetSound()->Play(CSound::SOUND_LABEL_BGM_RANKING);

	//�t�@�C���ǂݍ��ݏ���
	Load();

	return S_OK;
}

//======================================================
//�I������
//======================================================
void CRanking::Uninit(void)
{
	//�T�E���h���~�߂�
	CApplication::GetSound()->Stop();

	if (m_pObj2D != nullptr)
	{
		m_pObj2D->Uninit();
		m_pObj2D = nullptr;
	}

	if (m_pScoreUI != nullptr)
	{
		m_pScoreUI->Uninit();
		m_pScoreUI = nullptr;
	}
}

//======================================================
//�X�V����
//======================================================
void CRanking::Update(void)
{
	//�L�[����
	if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN))
	{
		Save();
		CApplication::SetFade(CApplication::MODE_TITLE);
	}
}

//============================
//�t�@�C���ɏ����o��
//============================
void CRanking::Save()
{
	FILE*pFile;		//�t�@�C���|�C���^�錾

	//�t�@�C�����J��
	pFile = fopen("data/Save/ranking.txt", "w");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		for (int nCnt = 0; nCnt < MAX_RANKING_RANK; nCnt++)
		{
			//�t�@�C���ɐ��l�������o��
			fprintf(pFile, "%d\n", m_nData[nCnt]);
		}
		//�t�@�C�������
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ
		printf("***�t�@�C�����J���܂���ł���***");
	}
}

//============================
//�t�@�C������ǂݍ���
//============================
void CRanking::Load()
{
	FILE*pFile;		//�t�@�C���|�C���^�錾

	//�t�@�C�����J��
	pFile = fopen("data/Save/ranking.txt", "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		for (int nCnt = 0; nCnt < MAX_RANKING_RANK; nCnt++)
		{
			//�t�@�C�����琔�l��ǂݍ���
			fscanf(pFile, "%d\n", &m_nData[nCnt]);
		}
		//�t�@�C�������
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ
		printf("***�t�@�C�����J���܂���ł���***");
	}
}

//====================================
//�X�R�A�̐ݒ菈��
//====================================
void CRanking::SetRankingScore()
{
	int aPosTexU[MAX_RANKING_RANK][MAX_RANKING];	//�e���̐������i�[

	//�V�����X�R�A�ƌÂ��X�R�A�̔�r
	for (int nCnt = 0; nCnt < MAX_RANKING_RANK +1; nCnt++)
	{
		if (m_nRanking > m_nData[4])
		{
			m_nData[4] = m_nRanking;
		}
	}
	//�o�u���\�[�g
	for (int nCount = 0; nCount < MAX_RANKING_RANK -1; nCount++)
	{
		for (int nCount2 = nCount + 1; nCount2 < MAX_RANKING_RANK; nCount2++)
		{
			if (m_nData[nCount] < m_nData[nCount2])
			{
				int nMin = m_nData[nCount + 1];
				m_nData[nCount] = m_nData[nCount2];
				m_nData[nCount2] = nMin;
			}
		}
	}
	for (int nCntScore = 0; nCntScore < MAX_RANKING_RANK; nCntScore++)
	{
		aPosTexU[nCntScore][0] = m_nData[nCntScore] % 100000000 / 1000000;
		aPosTexU[nCntScore][1] = m_nData[nCntScore] % 10000000 / 1000000;
		aPosTexU[nCntScore][2] = m_nData[nCntScore] % 1000000 / 100000;
		aPosTexU[nCntScore][3] = m_nData[nCntScore] % 100000 / 10000;
		aPosTexU[nCntScore][4] = m_nData[nCntScore] % 10000 / 1000;
		aPosTexU[nCntScore][5] = m_nData[nCntScore] % 1000 / 100;
		aPosTexU[nCntScore][6] = m_nData[nCntScore] % 100 / 10;
		aPosTexU[nCntScore][7] = m_nData[nCntScore] % 10 / 1;
	}

	for (int nCntScore = 0; nCntScore < MAX_RANKING_RANK; nCntScore++)
	{
		//�z��̊m��
		int nScore[MAX_RANKING_RANK] = {};
		std::string str[MAX_RANKING_RANK] = {};
		std::string begin[MAX_RANKING_RANK] = {};
		const char* pStr[MAX_RANKING_RANK] = {};
		//�X�R�A���擾���āA8���̕�����ɕϊ�����
		nScore[nCntScore] = m_nData[nCntScore];
		str[nCntScore] = std::to_string(nScore[nCntScore]);

		if (str[nCntScore].size() < MAX_RANKING)
		{
			for (int nCnt = 0; nCnt < MAX_RANKING - (int)str[nCntScore].size(); nCnt++)
			{
				begin[nCntScore] += '0';
			}

			begin[nCntScore] += str[nCntScore];
		}
		else
		{
			begin[nCntScore] = str[nCntScore];
		}

		pStr[nCntScore] = begin[nCntScore].c_str();

		//UI�̐���
		m_pScoreUI = CUIString::Create(D3DXVECTOR3(800.0f,50.0f + (150.0f * nCntScore), 0.0f), D3DXVECTOR2(150.0f, 50.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), pStr[nCntScore], 5);
	}
}

//======================================================
//��������
//======================================================
CRanking *CRanking::Create()
{
	//���I�m��
	CRanking *pRanking = new CRanking;

	if (pRanking != nullptr)
	{
		//������
		pRanking->Init();
		pRanking->SetRankingScore();
	}

	return pRanking;
}