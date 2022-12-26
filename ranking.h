//=============================================================================
//
// �����L���O
// Author:����]��
//
//=============================================================================
#ifndef _RANKING_H_                        //���̃}�N����`������ĂȂ�������
#define _RANKING_H_                        //��d�C���N���[�h�h�~�̃}�N����`

#include "mode.h"

//�O���錾
class CObject2D;
class CFade;
class CUIString;

//�}�N����`
#define MAX_RANKING			(8)        //�����L���O�̌���
#define MAX_RANKING_RANK    (5)        //�����L���O�̏��ʕ�
#define MAX_TEXTURE			(4)        //�e�N�X�`���̐�

//�����L���O�N���X
class CRanking : public CMode
{
public:
	CRanking();
	~CRanking();

	//�v���g�^�C�v�錾
	HRESULT Init();
	void Uninit();
	void Update();
	void Load();
	void Save();
	void SetRankingScore();
	static void GetRanking(int nRanking) { m_nRanking = nRanking; }
	static CRanking *Create();    //�����L���O�̐���

private:
	CObject_2D* m_pObj2D;
	CUIString* m_pScoreUI;
	CFade *m_pFade;
	static int m_nRanking;
	static int m_nData[MAX_RANKING_RANK];
};

#endif;