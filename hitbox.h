//=============================================================================
//
// hitbox.h
// Author : Ricci Alex
//
//=============================================================================
#ifndef HITBOX_H
#define HITBOX_H

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "object.h"
#include <vector>

//=============================================================================
//�O���錾
//=============================================================================
class CSolidObject;

class CHitbox
{
public:

	enum HITBOX_SHAPE
	{
		SHAPE_SPHERE = 0,
		SHAPE_BOX,
		SHAPE_CYLINDER,

		SHAPE_MAX
	};

	enum HITBOX_TYPE
	{
		TYPE_PLAYER = 0,
		TYPE_OBSTACLE,
		TYPE_NEUTRAL,
		TYPE_VANISHING,
		TYPE_BUTTON,
		TYPE_OVERLAPPABLE,
		TYPE_FALL,

		TYPE_MAX
	};

	enum COLLISION_DIRECTION
	{
		FROM_ABOVE = 0,
		FROM_BELOW,
		FROM_SIDE,
		NO_COLLISION,

		COLLISION_DIR_MAX
	};

	enum INTERACTION_EFFECT
	{
		EFFECT_DAMAGE = 0,
		EFFECT_LAUNCH,
		EFFECT_PUSH,
		EFFECT_BOUNCE,
		EFFECT_JUMP,
		EFFECT_FALL,

		EFFECT_MAX
	};

	enum OVERLAP_RESPONSE_TYPE
	{
		RESPONSE_EXTRUDE = 0,
		RESPONSE_OVERLAP,
		RESPONSE_IGNORE,

		RESPONSE_MAX

	};

	CHitbox();												//�R���X�g���N�^
	virtual ~CHitbox();										//�f�X�g���N�^

	virtual HRESULT Init(void);								//����������
	virtual void    Uninit(void);							//�I������
	virtual void    Update(void);							//�X�V����
															
	void Release(void);										//�j������
															
	D3DXVECTOR3  GetPos(void);								//�ʒu�̎擾����
	D3DXVECTOR3  GetRelativePos(void);						//���Έʒu�̎擾����
	D3DXVECTOR3  GetLastPos(void);							//�O��̈ʒu�̎擾����
	D3DXVECTOR3  GetMove(void);								//���x�̎擾����
	D3DXVECTOR3  GetAcceleration(void);						//�����̎擾����
	D3DXVECTOR3  GetSize(void);								//�T�C�Y�̎擾����
	D3DXVECTOR3  GetRot(void);								//�����̎擾����
	HITBOX_SHAPE GetShape(void);							//�`�̎擾����
	HITBOX_TYPE  GetType(void);								//��ނ̎擾����
	CObject*	 GetParent(void);							//�e�̎擾����
	INTERACTION_EFFECT GetEffect(void);						//�G�t�F�N�g�̎擾����
	const D3DXVECTOR3  GetDirection(void);					//�G�t�F�N�g�̌����̎擾����
	COLLISION_DIRECTION GetCollisionDirection(void);		
	OVERLAP_RESPONSE_TYPE GetOverlapResponse(const int nIdx);
															
	void SetPos(const D3DXVECTOR3 pos);						//�ʒu�̐ݒ菈��
	void SetRelativePos(const D3DXVECTOR3 pos);				//���Έʒu�̐ݒ菈��
	void SetLastPos(const D3DXVECTOR3 pos);					//�O��̈ʒu�̐ݒ菈��
	void SetMove(const D3DXVECTOR3 move);					//���x�̐ݒ菈��
	void SetAcceleration(const D3DXVECTOR3 acc);			//�����̐ݒ菈��
	void SetSize(const D3DXVECTOR3 size);					//�T�C�Y�̐ݒ菈��
	void SetRot(const D3DXVECTOR3 rot);						//�����̐ݒ菈��
	void SetShape(const HITBOX_SHAPE shape);				//�`�̐ݒ菈��
	void SetType(const HITBOX_TYPE type);					//��ނ̐ݒ菈��
	void SetParent(CObject* pParent);						//�e�̐ݒ菈��
	void SetCollisionState(bool bCollision);				//�����Ɠ����������ǂ����̐ݒ菈��
	void SetPlayerIdx(const int nPlayerIdx);				//�v���C���[�C���f�b�N�X�̐ݒ菈��
	void SetScore(const int nScore);						//�X�R�A�̐ݒ菈��
	void SetEffect(INTERACTION_EFFECT effect);				//�G�t�F�N�g�̐ݒ菈��
	void SetInvincibility(const bool bInv);					//���G��Ԃ̐ݒ菈��
	void SetDirection(const D3DXVECTOR3 dir);				//�G�t�F�N�g�̌����̐ݒ菈��
	void SetCollisionDirection(COLLISION_DIRECTION dir);
															
	bool GetCollisionState(void);							//�����Ɠ����������ǂ����̎擾����
	const int GetPlayerIdx(void);							//�v���C���[�C���f�b�N�X�̎擾����
	const int GetScore(void);								//�X�R�A�̎擾����
	const bool GetInvincibility(void);						//���G��Ԃ̎擾����

	void SetOverlapResponse(HITBOX_TYPE type, OVERLAP_RESPONSE_TYPE reaction);		//�d�Ȃ����ꍇ�̔����ݒ菈��
	void IgnoreAllCollision(void);							//�S���̏Փ˂𖳎�����悤�ɂ̐ݒ菈��
															
	static void ReleaseAll(void);							//�S���̃q�b�g�{�b�N�X�̔j������
	static std::vector <CHitbox*>* GetAllHitbox(void);		//�S���̃q�b�g�{�b�N�X�̎擾����

private:

	D3DXVECTOR3  m_pos;										//�ʒu
	D3DXVECTOR3  m_RelativePos;								//���Έʒu
	D3DXVECTOR3  m_LastPos;									//�O��̈ʒu
	D3DXVECTOR3  m_move;									//���x
	D3DXVECTOR3  m_acceleration;							//����
	D3DXVECTOR3  m_size;									//�T�C�Y
	D3DXVECTOR3  m_rot;										//����
	D3DXVECTOR3  m_dir;										//�G�t�F�N�g�̌���
	HITBOX_SHAPE m_shape;									//�`
	HITBOX_TYPE  m_type;									//���
	INTERACTION_EFFECT m_effect;							//�G�t�F�N�g
	COLLISION_DIRECTION m_CollisionDir;
	OVERLAP_RESPONSE_TYPE m_response[TYPE_MAX];
	bool		 m_bCollided;								//�����Ɠ����������ǂ���
	CObject*	 m_pParent;									//�e
	int			 m_nPlayerIdx;								//�v���C���[�C���f�b�N�X
	int			 m_nScore;									//�X�R�A
	bool		 m_bInvincible;								//���G��Ԃł��邩�ǂ���
															
	static std::vector <CHitbox*> m_vHitbox;				//�S���̃q�b�g�{�b�N�X
};



#endif
#pragma once