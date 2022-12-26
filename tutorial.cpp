//=============================================================================
//
// tutorial.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "tutorial.h"
#include "object2D.h"
#include "inputKeyboard.h"
#include "application.h"
#include "AnimateUI.h"

//�R���X�g���N�^
CTutorial::CTutorial()
{

}

//�f�X�g���N�^
CTutorial::~CTutorial()
{

}

//����������
HRESULT CTutorial::Init(void)
{
	CObject_2D* pObj = CObject_2D::Create();

	if (!pObj)
	{
		return -1;
	}

	pObj->SetPos(D3DXVECTOR3(600.0f, 300.0f, 0.0f));
	pObj->SetSize(D3DXVECTOR2(400.0f, 100.0f));
	pObj->SetTexture(CObject::TEXTURE_TUTORIAL);
	pObj->SetPriority(5);
	pObj = CObject_2D::Create();

	//�T�E���h�̍Đ�
	CApplication::GetSound()->Play(CSound::SOUND_LABEL_BGM_TITLE);

	return S_OK;
}

//�I������
void CTutorial::Uninit(void)
{
	//�T�E���h���~�߂�
	CApplication::GetSound()->Stop();
}

//�X�V����
void CTutorial::Update(void)
{
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
CTutorial* CTutorial::Create(void)
{
	CTutorial* pTitle = new CTutorial;

	if (FAILED(pTitle->Init()))
	{
		return nullptr;
	}

	return pTitle;
}
