//=============================================================================
//
// title.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "title.h"
#include "object2D.h"
#include "inputKeyboard.h"
#include "application.h"
#include "AnimateUI.h"

//�R���X�g���N�^
CTitle::CTitle()
{

}

//�f�X�g���N�^
CTitle::~CTitle()
{

}

//����������
HRESULT CTitle::Init(void)
{
	//������
	m_nCounter = 0;
	m_nSelect = 0;

	m_pObj2D[MAX_TITLE] = {};

	for (int nCnt = 0; nCnt < MAX_TITLE; nCnt++)
	{
		m_pObj2D[nCnt] = CObject_2D::Create();

		if (!m_pObj2D[nCnt])
		{
			return -1;
		}
	}
	//�|���S���̐ݒ�
	m_pObj2D[0]->SetPos(D3DXVECTOR3(600.0f, 300.0f, 0.0f));
	m_pObj2D[0]->SetSize(D3DXVECTOR2(400.0f, 100.0f));
	m_pObj2D[0]->SetTexture(CObject::TEXTURE_TITLE_LOGO);
	m_pObj2D[0]->SetPriority(5);

	m_pObj2D[1]->SetPos(D3DXVECTOR3(900.0f, 620.0f, 0.0f));
	m_pObj2D[1]->SetSize(D3DXVECTOR2(200.0f, 50.0f));
	m_pObj2D[1]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
	m_pObj2D[1]->SetTexture(CObject::TEXTURE_TUTORIAL);
	m_pObj2D[1]->SetPriority(5);

	m_pObj2D[2]->SetPos(D3DXVECTOR3(300.0f, 620.0f, 0.0f));
	m_pObj2D[2]->SetSize(D3DXVECTOR2(200.0f, 50.0f));
	m_pObj2D[2]->SetTexture(CObject::TEXTURE_START);
	m_pObj2D[2]->SetPriority(5);

	m_pObj2D[3] = CObject_2D::Create();
	m_pObj2D[3]->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	m_pObj2D[3]->SetSize(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 5));
	m_pObj2D[3]->SetTexture(CObject::TEXTURE_TITLE001);
	m_pObj2D[3]->SetPriority(4);

	/*CAnimateUI::UIAnimation animInfo;
	animInfo.deltaSize = D3DXVECTOR2(0.5f, 0.25f);
	animInfo.nChangeFrame = 60;
	CAnimateUI* pLogo = CAnimateUI::Create(CObject::TEXTURE_TITLE_LOGO, D3DXVECTOR3((float)SCREEN_WIDTH * 0.5f, (float)SCREEN_HEIGHT * 0.4f, 0.0f), D3DXVECTOR2(300.0f, 150.0f), ColorWhite, animInfo);
	pLogo->AnimateColor(true);*/

	return S_OK;
}

//�I������
void CTitle::Uninit(void)
{

}

//�X�V����
void CTitle::Update(void)
{
	if ((CInputKeyboard::GetKeyboardTrigger(DIK_D) || CInputKeyboard::GetKeyboardTrigger(DIK_RIGHT)))
	{//D�L�[�������ꂽ
		if (m_nSelect < 1)
		{
			m_nSelect++;
			m_pObj2D[m_nSelect + 1]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
		}
	}
	else if ((CInputKeyboard::GetKeyboardTrigger(DIK_A) || CInputKeyboard::GetKeyboardTrigger(DIK_LEFT)))
	{//A�L�[�������ꂽ
		if (m_nSelect > 0)
		{
			m_pObj2D[m_nSelect]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
			m_nSelect--;
		}
	}

	switch (m_nSelect)
	{
	case 0:
		//�I�����Ă���|���S���𔼓����ɂ���
		m_pObj2D[2]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		//����L�[(ENTER�L�[)�������ꂽ���ǂ���
		if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN))
		{
			//���[�h�ݒ�
			CApplication::SetFade(CApplication::MODE_FIRST_STAGE);
		}
		break;

	case 1:
		//�I�����Ă���|���S���𔼓����ɂ���
		m_pObj2D[1]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		////����L�[(ENTER�L�[)�������ꂽ���ǂ���
		//if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN))
		//{
		//	//���[�h�ݒ�
		//	CApplication::SetFade(CApplication::MODE_TUTORIAL);
		//}
		break;
	}

	if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN))
	{
		CApplication::SetFade(CApplication::MODE_FIRST_STAGE);
	}
}



//=============================================================================
//
//								�ÓI�֐�
//
//=============================================================================



//��������
CTitle* CTitle::Create(void)
{
	CTitle* pTitle = new CTitle;

	if (FAILED(pTitle->Init()))
	{
		return nullptr;
	}

	return pTitle;
}
