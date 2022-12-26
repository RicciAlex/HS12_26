//=============================================================================
//
// results.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "results.h"
#include "object2D.h"
#include "inputKeyboard.h"
#include "application.h"
#include "UIString.h"
#include "debugProc.h"

//�R���X�g���N�^
CResults::CResults()
{

}

//�f�X�g���N�^
CResults::~CResults()
{

}

//����������
HRESULT CResults::Init(void)
{
	CObject_2D* pObj = CObject_2D::Create();

	if (!pObj)
	{
		return -1;
	}

	pObj->SetPos(D3DXVECTOR3((float)SCREEN_WIDTH * 0.5f, (float)SCREEN_WIDTH * 0.5f, 0.0f));
	pObj->SetSize(D3DXVECTOR2((float)SCREEN_WIDTH * 0.5f, (float)SCREEN_WIDTH * 0.5f));
	pObj->SetTexture(CObject::TEXTURE_RESULT);
	pObj->SetPriority(4);

	CUIString* pString = CUIString::Create(D3DXVECTOR3((float)SCREEN_WIDTH * 0.40f, (float)SCREEN_HEIGHT * 0.3f, 0.0f), D3DXVECTOR2(350.0f, 50.0f), ColorYellow, "RESULTS", 5);

	if (m_pScoreUI != nullptr)
	{//�X�R�AUI�̍X�V����

	 //�X�R�A���擾���āA4���̕�����ɕϊ�����
		//int nScore = m_pScore->GetScore();
		//std::string str = std::to_string(nScore);
		//std::string begin = {};

		//if (str.size() < 4)
		//{
		//	for (int nCnt = 0; nCnt < 4 - (int)str.size(); nCnt++)
		//	{
		//		begin += '0';
		//	}

		//	begin += str;
		//}
		//else
		//{
		//	begin = str;
		//}

		//const char* pStr = begin.c_str();
		//m_pScoreUI->ChangeString(pStr);				//UI�̕�����̐ݒ�

		//UI�̐���
		//m_pScoreUI = CUIString::Create(D3DXVECTOR3(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f)), D3DXVECTOR2(500.0f, 50.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), pStr, 5);
	}

	return S_OK;
}

//�I������
void CResults::Uninit(void)
{

}

//�X�V����
void CResults::Update(void)
{
	if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN))
	{
		CApplication::SetFade(CApplication::MODE_RANKING);
	}
}



//=============================================================================
//
//								�ÓI�֐�
//
//=============================================================================



//��������
CResults* CResults::Create(void)
{
	CResults* pResults = new CResults;

	if (FAILED(pResults->Init()))
	{
		return nullptr;
	}

	return pResults;
}
