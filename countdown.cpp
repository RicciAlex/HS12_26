//=============================================================================
//
// �J�E���g�_�E������ [countdown.cpp]
// Author : KADO TAKUMA
//
//=============================================================================
#include "countdown.h"
#include "application.h"
#include "rendering.h"
#include "sound.h"
#include <assert.h>

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCountDown::CCountDown() :
	CObject_2D(5)
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCountDown::~CCountDown()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCountDown::Init(void)
{
	//�I�u�W�F�N�g�̏���������
	CObject_2D::Init();

	//���_�J���[�̐ݒ�
	CObject_2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//���_�T�C�Y�̐ݒ�
	CObject_2D::SetSize(D3DXVECTOR2(250.0f, 250.0f));

	//�h���̃e�N�X�`���|�C���^��e�̃e�N�X�`���|�C���^�ɑ�����鏈��
	SetTexture(CObject::TEXTURE_COUNTDOWN_3);

	////�T�E���h�̍Đ�
	//PlaySound(SOUND_LABEL_SE_COUNTDOWN);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CCountDown::Uninit(void)
{
	//�I�u�W�F�N�g�̏I������
	CObject_2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CCountDown::Update(void)
{
	//�J�E���^�[���Z
	m_nCounter++;

	//�J�E���g�_�E���ɍ��킹�ĉ摜�ύX
	if (m_nCounter == 60)
	{
		//�h���̃e�N�X�`���|�C���^��e�̃e�N�X�`���|�C���^�ɑ�����鏈��
		SetTexture(CObject::TEXTURE_COUNTDOWN_2);

		////�T�E���h�̍Đ�
		//PlaySound(SOUND_LABEL_SE_COUNTDOWN);
	}
	else if (m_nCounter == 120)
	{
		//�h���̃e�N�X�`���|�C���^��e�̃e�N�X�`���|�C���^�ɑ�����鏈��
		SetTexture(CObject::TEXTURE_COUNTDOWN_1);

		////�T�E���h�̍Đ�
		//PlaySound(SOUND_LABEL_SE_COUNTDOWN);
	}
	else if (m_nCounter == 180)
	{
		//�h���̃e�N�X�`���|�C���^��e�̃e�N�X�`���|�C���^�ɑ�����鏈��
		SetTexture(CObject::TEXTURE_COUNTDOWN_0);
	}
	else if (m_nCounter == 240)
	{
		//�I������
		Release();
		return;
	}

	//�I�u�W�F�N�g�̍X�V����
	CObject_2D::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CCountDown::Draw(void)
{
	//�I�u�W�F�N�g�̕`�揈��
	CObject_2D::Draw();
}

//=============================================================================
// ��������
//=============================================================================
CCountDown * CCountDown::Create(D3DXVECTOR3 pos)
{
	//�|�C���^�錾
	CCountDown *pCountDown;

	//�C���X�^���X����
	pCountDown = new CCountDown;

	if (pCountDown != nullptr)
	{//�|�C���^�����݂�������s
		pCountDown->Init();
		pCountDown->SetPos(pos);
	}
	else
	{//�|�C���^����������������s
		assert(false);
	}

	return pCountDown;
}
