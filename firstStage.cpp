//=============================================================================
//
// firstStage.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "firstStage.h"
#include "player.h"
#include "meshfield.h"
#include "inputKeyboard.h"
#include "application.h"
#include "bg.h"
#include "BoxHitbox.h"
#include "object3D.h"

//�R���X�g���N�^
CFirstStage::CFirstStage()
{

}

//�f�X�g���N�^
CFirstStage::~CFirstStage()
{

}

//����������
HRESULT CFirstStage::Init(void)
{
	//��{�N���X�̏���������
	if (FAILED(CGame::Init()))
	{
		return -1;
	}

	CPlayer* pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, -200.0f, 0.0f), 0);

	if (!pPlayer)
	{
		return -1;
	}

	SetPlayer(pPlayer);

	CMeshfield::Create(D3DXVECTOR3(-500.0f, -200.0f, 500.0f), Vec3Null, D3DXVECTOR2(100.0f, 100.0f), 10, 10);

	return S_OK;
}

//�I������
void CFirstStage::Uninit(void)
{
	//��{�N���X�̏I������
	CGame::Uninit();
}

//�X�V����
void CFirstStage::Update(void)
{
	//��{�N���X�̍X�V����
	CGame::Update();

#ifdef _DEBUG

	if (CInputKeyboard::GetKeyboardTrigger(DIK_O))
	{
		CApplication::SetFade(CApplication::MODE_RESULTS);
	}

#endif // DEBUG

}




//=============================================================================
//
//								�ÓI�֐�
//
//=============================================================================


//��������
CFirstStage* CFirstStage::Create(void)
{
	CFirstStage* pStage = new CFirstStage;		//�C���X�^���X�𐶐�����

	if (FAILED(pStage->Init()))
	{//����������
		return nullptr;
	}

	return pStage;								//���������C���X�^���X��Ԃ�
}