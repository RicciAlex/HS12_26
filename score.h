//=============================================================================
//
// �X�R�A���� [score.h]
// Author : KADO TAKUMA
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "object.h"

//=============================================================================
// �O����`
//=============================================================================
class CNumber;

//=============================================================================
// �}�N����`
//=============================================================================
#define SCORE_DIGIT	(5)		//�g�p���錅��

//=============================================================================
// �N���X�̒�`
//=============================================================================
class CScore : public CObject
{
public:
	//-------------------------------------------------------------------------
	// �R���X�g���N�^�ƃf�X�g���N�^
	//-------------------------------------------------------------------------
	CScore();
	~CScore() override;

	//-------------------------------------------------------------------------
	// �����o�[�֐�
	//-------------------------------------------------------------------------
	HRESULT Init() override;																				// ����������
	void Uninit() override;																					// �I������						
	void Update() override;																					// �X�V����
	void Draw() override;																					// �`�揈��
	static CScore *Create(bool bCalcDigit, int nScore, int nDigit, D3DXVECTOR3 pos, D3DXVECTOR2 size);		// ��������

	void SetScore(int nScore);										// �X�R�A�̐ݒ菈��
	void AddScore(int nValue);										// �X�R�A�̉��Z����
	void SubScore(int nValue);										// �X�R�A�̌��Z����
	void AddSaveScore(int nValue);									// �ۑ��X�R�A�̉��Z����
	int GetScore(void) { return m_nScore; };						// �X�R�A�̃Q�b�^�[
	void CalcDigit(int nScore);										// �������v�Z���鏈��

	//void VtxUpdate() override {};									// ���_���W�X�V����
	void SetPos(const D3DXVECTOR3 pos) override;					// ���W�ݒ菈��
	void SetSize(D3DXVECTOR2 size)  { m_size = size; };		// �T�C�Y�ݒ菈��
	//void SetMove(D3DXVECTOR3 move) override { D3DXVECTOR3 i = move; };	// �ړ��ʐݒ菈��
	//void SetCol(D3DXCOLOR col) override { D3DXCOLOR i = col; };			// �F�ݒ菈��
	//void SetRot(D3DXVECTOR3 rot) override { D3DXVECTOR3 i = rot; };	// �����ݒ菈��
	void SetDigit(int nDigit) { m_nDigit = nDigit; }				// �����ݒ菈��
	void SetCalcDigit(bool bCalcDigit) { m_bCalcDigit = bCalcDigit; }

	const D3DXVECTOR3 GetPos(void) override { return m_pos; }				// ���W�擾����
	D3DXVECTOR2 GetSize(void)  { return m_size; }			// �T�C�Y�擾����
	//D3DXVECTOR3 GetMove(void) override { return D3DXVECTOR3(); }	// �ړ��ʎ擾����
	//D3DXVECTOR3 GetRot(void) override { return D3DXVECTOR3(); }		// �����擾����
	//D3DXCOLOR GetCol(void) override { return D3DXCOLOR(); }			// �F�擾����

private:
	//-------------------------------------------------------------------------
	// �����o�[�ϐ�
	//-------------------------------------------------------------------------
	CNumber *m_apNumber[SCORE_DIGIT];			// �\�����錅���̃|�C���^
	D3DXVECTOR3 m_pos;							// ���W
	D3DXVECTOR2 m_size;							// �T�C�Y
	int  m_nScore;								// ���i�[�ϐ�
	int  m_nDigit;								// �g�p���錅��
	bool m_bCalcDigit;							// ������ύX���邩�ۂ�
	int m_nSaveScore;
};

#endif
