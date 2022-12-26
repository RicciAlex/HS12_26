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

	//�T�E���h�̍Đ�
	CApplication::GetSound()->Play(CSound::SOUND_LABEL_BGM_RESULT);

	return S_OK;
}

//�I������
void CResults::Uninit(void)
{
	//�T�E���h���~�߂�
	CApplication::GetSound()->Stop();
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
