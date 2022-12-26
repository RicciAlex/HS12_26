//=============================================================================
//
// game.h
// Author : Ricci Alex
//
//=============================================================================
#ifndef _GAME_H
#define _GAME_H

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "mode.h"

//=============================================================================
//�O���錾
//=============================================================================
class CPlayer;
class CScore;
class CTimer;
class CCountDown;

class CGame : public CMode
{
public:

	//�X�e�[�W
	enum STAGE
	{
		FIRST_STAGE = 0,

		STAGE_MAX
	};

	CGame();						//�R���X�g���N�^
	~CGame() override;				//�f�X�g���N�^

	HRESULT Init(void);				//����������
	void Uninit(void);				//�I������
	void Update(void);				//�X�V����

	void SetPlayer(CPlayer* pPlayer);		//�v���C���[�̐ݒ菈��
	CPlayer* GetPlayer(void);				//�v���C���[�̎擾����
	static CScore* GetScore() { return m_pScore; }
	static CTimer* GetTimer() { return m_pTimer; }
	static CCountDown* GetCountDown() { return m_pCountDown; }

private:

	CPlayer* m_pPlayer;
	static CScore* m_pScore;
	static CTimer* m_pTimer;
	static CCountDown* m_pCountDown;

};



#endif