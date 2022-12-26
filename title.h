//=============================================================================
//
// title.h
// Author : Ricci Alex
//
//=============================================================================
#ifndef _TITLE_H
#define _TITLE_H

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "mode.h"

//=============================================================================
//�O���錾
//=============================================================================
class CObject_2D;

//�}�N����`
#define MAX_TITLE			(4)			//�|���S���̍ő吔

class CTitle : public CMode
{
public:
	CTitle();
	~CTitle() override;

	HRESULT Init(void) override;				//����������
	void Uninit(void) override;					//�I������
	void Update(void) override;					//�X�V����

	static CTitle* Create(void);				//��������

private:

	CObject_2D *m_pObj2D[MAX_TITLE];
	int m_nCounter;									//�_�Ŏ���
	int m_nSelect;									//�I��ł���ԍ�
};



#endif