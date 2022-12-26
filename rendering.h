//=============================================================================
//
// rendering.h
// Author : Ricci Alex
//
//=============================================================================
#ifndef RENDERING_H
#define RENDERING_H

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �O���錾
//=============================================================================
class CMembraneShading;
class CPhongShading;

//=============================================================================
// �����_�����O�N���X
//=============================================================================
class CRenderer
{
public:
	CRenderer();									//�R���X�g���N�^
	~CRenderer();									//�f�X�g���N�^
	HRESULT Init(HWND hWnd, bool bWindow);			//����������
	void Uninit(void);								//�I������
	void Update(void);								//�X�V����
	void Draw(void);								//�`�揈��
													
	LPDIRECT3DDEVICE9 GetDevice(void);				//�f�o�C�X�̎擾����
	CMembraneShading* GetMembraneEffect(void);		//�V�F�[�_�[�̃G�t�F�N�g�̎擾����
	CPhongShading* GetPhongEffect(void);

private:
	void DrawFPS(void);								//FPS�̕`�揈��

private:
	
	LPDIRECT3D9 m_pD3D;								// Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9 m_pD3DDevice;					// Device�I�u�W�F�N�g
	LPD3DXFONT m_pFont;								//�t�H���g

	CMembraneShading* m_pMembrane;					//�V�F�[�_�[�̃G�t�F�N�g�ւ̃|�C���^
	CPhongShading* m_pPhong;
};

#endif // !RENDERING_H