//=============================================================================
//
// player.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "player.h"
#include "inputKeyboard.h"
#include "camera.h"
#include "application.h"
#include "debugProc.h"
#include "meshfield.h"
#include "modelPart.h"
#include "animator.h"
#include "rendering.h"
#include "CylinderHitbox.h"
#include "inputMouse.h"
#include "game.h"
#include "score.h"

//=============================================================================
//							�ÓI�ϐ��̏�����
//=============================================================================

const float CPlayer::m_MaxWalkingSpeed = 7.0f;			//�ő�����X�s�[�h
const float CPlayer::m_AccelerationCoeff = 5.0f;		//�����W��
const D3DXVECTOR3 CPlayer::m_playerSize = D3DXVECTOR3(15.0f, 60.0f, 15.0f);				//�v���C���[�̃T�C�Y
const float CPlayer::m_fMouseSafeRadius = 50.0f;
const float CPlayer::m_fMouseSensibilityCoefficient = 0.001f;
const float CPlayer::m_fFallLimit = 100.0f;
const float CPlayer::m_fFrameMove = 3.0f;

//�v���C���[�̐F
D3DXCOLOR CPlayer::m_playerColor[PLAYER_COLOR_MAX]
{

	{ 1.0f, 0.0f, 0.0f, 1.0f },
	{ 0.0f, 1.0f, 0.0f, 1.0f },
	{ 0.0f, 0.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 1.0f, 1.0f },
	{ 0.0f, 1.0f, 1.0f, 1.0f },
	{ 0.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f }
};

//�R���X�g���N�^
CPlayer::CPlayer() : CObject::CObject(1)
{
	//�����o�[�ϐ����N���A����
	m_LastPos = Vec3Null;							//�O��̈ʒu
	m_pos = Vec3Null;								//�ʒu
	m_move = Vec3Null;								//���x�̏���������
	m_DestRot = Vec3Null;							//�ړI�̊p�x�̏���������
	m_pAnimator = nullptr;							//�A�j���[�^�[�ւ̃|�C���^
	m_rot = Vec3Null;								//����
	m_nPresentInclination = 0; 
	m_bRot = false;									//��]�������ǂ���
	m_pAnimator = nullptr;							//�A�j���[�^�[�ւ̃|�C���^
	m_State = (STATE)0;								//�A�j���[�V�������
	m_bJump = false;								//�W�����v���Ă��邩�ǂ���
	m_bLanded = false;
	m_bMoving = false;								//�ړ����Ă��邩�ǂ���
	m_nInvincibilityCnt = 0;						//���G��Ԃ̃J�E���^�[
	m_nCntBalance = 0;								//�U���J�E���^�[
	m_fFrictionCoeff = 0.0f;						//���C�W��
	m_fBalance = 0.0f;								//�o�����X�ϐ�
	m_nBalanceChangeTime = 0;						//�o�����X���ς��t���[��
	m_fFrameBalance = 0.0f;							//���t���[�����Z����Ă���o�����X�̒l
	m_bFall = false;								//�������Ă��邩�ǂ���
	m_pHitbox = nullptr;							//�q�b�g�{�b�N�X

	for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
	{//���f���̕����ւ̃|�C���^
		m_pModel[nCnt] = nullptr;
	}
}

//�f�X�g���N�^
CPlayer::~CPlayer()
{

}

//����������
HRESULT CPlayer::Init(void)
{
	//�����o�[�ϐ��̏���������
	m_LastPos = Vec3Null;							//�O��̈ʒu
	m_pos = Vec3Null;								//�ʒu
	m_move = Vec3Null;								//���x�̏���������
	m_DestRot = Vec3Null;							//�ړI�̊p�x�̏���������
	m_pAnimator = nullptr;							//�A�j���[�^�[�ւ̃|�C���^
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);		//����
	m_nPresentInclination = 0;
	m_bRot = false;									//��]�������ǂ���
	m_pAnimator = nullptr;							//�A�j���[�^�[�ւ̃|�C���^
	m_State = STATE_NEUTRAL;						//�A�j���[�V�������
	m_bJump = false;								//�W�����v���Ă��邩�ǂ���
	m_bLanded = false;
	m_bMoving = false;								//�ړ����Ă��邩�ǂ���
	m_nInvincibilityCnt = 0;						//���G��Ԃ̃J�E���^�[
	m_nCntBalance = 0;								//�U���J�E���^�[
	m_nBalanceChangeTime = random(-m_nTimeRange, m_nTimeRange);				//�o�����X���ς��t���[��
	m_fFrictionCoeff = 0.1f;						//���C�W��
	m_fBalance = 0.0f;								//�o�����X�ϐ�
	m_fFrameBalance = 0.0f;							//���t���[�����Z����Ă���o�����X�̒l
	m_bFall = false;								//�������Ă��邩�ǂ���
	m_pHitbox = nullptr;							//�q�b�g�{�b�N�X

	for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
	{//���f���̕����ւ̃|�C���^
		m_pModel[nCnt] = nullptr;
	}

	return S_OK;
}

//�I������
void CPlayer::Uninit(void)
{
	//���f���p�[�c�̏I������
	for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
	{
		if (m_pModel[nCnt] != nullptr)
		{
			m_pModel[nCnt]->Uninit();
			delete m_pModel[nCnt];
			m_pModel[nCnt] = nullptr;
		}
	}

	//�A�j���[�^�[�̔j������
	if (m_pAnimator != nullptr)
	{
		m_pAnimator->Uninit();
		delete m_pAnimator;
		m_pAnimator = nullptr;
	}

	//�q�b�g�{�b�N�X�̔j������
	if (m_pHitbox != nullptr)
	{
		m_pHitbox->Release();
		m_pHitbox = nullptr;
	}
}

//�X�V����
void CPlayer::Update(void)
{
	CScore* pScore = CGame::GetScore();

	m_LastPos = m_pos;				//�O��̈ʒu�̍X�V

	D3DXVECTOR3 cameraRot = CApplication::GetCamera()->GetRot();					//�J�����̌����̎擾����

	//�v���C���[�̖ړI�p�x�̐��K������
	D3DXVECTOR3 cR = D3DXVECTOR3(-cosf(cameraRot.y), 0.0f, sinf(cameraRot.y));
	float fA = acosf(cR.x);

	if (cR.z < 0.0f)
	{
		fA *= -1.0f;
	}

	if (!CApplication::GetFade() && !m_bFall)
	{//�t�F�[�h���Ă��Ȃ�������
		PlayerController();		//�v���C���[�𓮂���
	}

	//�ʒu�̍X�V
	m_pos += m_move;				
	CDebugProc::Print("\n%f %f %f", m_move.x, m_move.y, m_move.z);
	
	m_move.x += (0.0f - m_move.x) * m_fFrictionCoeff;		//�ړ��ʂ�X�R���|�l���g�̍X�V
	m_move.y += (0.0f - m_move.y) * 0.1f;					//�ړ��ʂ�Y�R���|�l���g�̍X�V
	m_move.z += (0.0f - m_move.z) * m_fFrictionCoeff;		//�ړ��ʂ�Z�R���|�l���g�̍X�V

	if (m_LastPos.z > m_pos.z)
	{
		if (pScore != nullptr)
		{
			pScore->AddScore(1);
		}
	}

	//�d�ʂ�ǉ�����
	if (m_move.y >= -10.0f)
	{
		m_move.y -= 0.65f;
	}

	if (m_move.y <= 0.0f)
	{
		float fHeight = 0.0f;

		if (!m_bFall)
		{
			//���b�V���t�B�[���h�Ƃ̓����蔻��
			CMeshfield* pField = CMeshfield::FieldInteraction(this, &fHeight);

			//�n�ʂƂ̓����蔻��
			if (pField != nullptr)
			{
				if (m_bJump)
				{
					if (m_pAnimator)
					{

						m_bJump = false;		//���n���Ă����Ԃɂ���
						m_bLanded = true;

						if (!m_bMoving)
						{
							//m_pAnimator->SetPresentAnim(CPlayer::STATE_NEUTRAL);
						}
						else
						{
							//m_pAnimator->SetPresentAnim(CPlayer::STATE_RUNNING);
						}
					}
				}

				m_bHit = false;			//�������ĂȂ���Ԃɂ���
				//���C�W���̎擾
				m_fFrictionCoeff = pField->GetFriction();

				//�e�̍����̐ݒ�
				for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
				{
					if (m_pModel[nCnt] != nullptr)
					{
						m_pModel[nCnt]->SetShadowHeight(fHeight);
					}
				}
			}
		}
	}

	if (m_nInvincibilityCnt > 0)
	{
		m_nInvincibilityCnt--;

		if (m_nInvincibilityCnt <= 0)
		{
			m_nInvincibilityCnt = 0;
			m_pHitbox->SetInvincibility(false);
		}
	}

	if (m_pHitbox)
	{
		m_pHitbox->SetPos(m_pos);
		m_pHitbox->Update();

		if (m_pHitbox->GetCollisionDirection() == CHitbox::FROM_ABOVE)
		{
			m_bLanded = true;
		}
		else if (m_pHitbox->GetCollisionDirection() == CHitbox::FROM_BELOW)
		{
			m_move.y = 0.0f;
		}

		if (D3DXVec3Length(&m_pHitbox->GetMove()) != 0.0f)
		{
			m_pos += m_pHitbox->GetMove();
			m_pHitbox->SetMove(Vec3Null);
		}

		if (D3DXVec3Length(&m_pHitbox->GetAcceleration()) != 0.0f)
		{
			m_move += m_pHitbox->GetAcceleration();
			m_pHitbox->SetAcceleration(Vec3Null);
		}

		HitboxEffectUpdate();
	}

	if (m_pAnimator != nullptr)
	{
		//�A�j���[�V�����̍X�V
		m_pAnimator->Update();

		
	}

	if (m_pos.y <= -1000.0f)
	{
		RespawnPlayer();
		m_bFall = false;
	}

	CCamera* pCamera = CApplication::GetCamera();

	if (pCamera != nullptr)
	{
		D3DXVECTOR3 p = D3DXVECTOR3(100.0f, 250.0f, -350.0f);
		D3DXVECTOR3 q = D3DXVECTOR3(0.0f, 0.0f, 300.0f);
		pCamera->SetPos(m_pos + p, m_pos + q);
	}

	CDebugProc::Print("\n\n Pos: %f %f %f", m_pos.x, m_pos.y, m_pos.z);

	//m_rot.y += 0.01f;
}

//�`�揈��
void CPlayer::Draw(void)
{
	if (m_nInvincibilityCnt % 10 <= 5)
	{//���G��Ԃ�������A�_�ł�����

		D3DXMATRIX mtxTrans, mtxRot;												//�v�Z�p�̃}�g���b�N�X
		D3DXMatrixIdentity(&m_mtxWorld);											//���[���h�}�g���b�N�X�̏���������

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		m_pModel[BODY]->Draw(m_mtxWorld);											//�ŏ��̃��f���̕`�揈��

		for (int nCnt = 1; nCnt < PARTS_MAX; nCnt++)
		{//���̃��f���̕`�揈��
			if (m_pModel[nCnt] != nullptr)
			{
				m_pModel[nCnt]->Draw();
			}
		}
	}
}

//�ʒu�̐ݒ菈��
void CPlayer::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//�����̐ݒ菈��
void CPlayer::SetRot(const D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//���n���Ă��邩�ǂ����̐ݒ菈��
void CPlayer::SetLanded(void)
{
	if (m_bJump)
	{
		if (m_pAnimator)
		{

			m_bJump = false;		//���n���Ă����Ԃɂ���
			m_bLanded = true;

			if (!m_bMoving)
			{
				//m_pAnimator->SetPresentAnim(CPlayer::STATE_NEUTRAL);
			}
			else
			{
				//m_pAnimator->SetPresentAnim(CPlayer::STATE_RUNNING);
			}
		}
	}
}

//���x�̐ݒ菈��
void CPlayer::SetMove(const D3DXVECTOR3 move)
{
	m_move = move;
}

//�ʒu�̎擾����
const D3DXVECTOR3 CPlayer::GetPos(void)
{
	return m_pos;
}

//�O��̈ʒu�̎擾����
const D3DXVECTOR3 CPlayer::GetLastPos(void)
{
	return m_LastPos;
}

//���x�̎擾����
const D3DXVECTOR3 CPlayer::GetMove(void)
{
	return m_move;
}

//�|�������ǂ����̎擾����
const bool CPlayer::GetFall(void)
{
	return m_bFall;
}

//=============================================================================
//
//								�ÓI�֐�
//
//=============================================================================

//��������
CPlayer* CPlayer::Create(const D3DXVECTOR3 pos, int nCntPlayer)
{
	CPlayer* pModel = new CPlayer;		//�C���X�^���X�𐶐�����

	//�v���C���[�̏���������
	if (FAILED(pModel->Init()))
	{
		return nullptr;
	}

	pModel->m_pos = pos;						//�ʒu�̐ݒ�
	pModel->m_LastPos = pos;					//�O��̈ʒu�̐ݒ�

	pModel->m_rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	pModel->m_pModel[BODY] = CModelPart::Create(CModel::MODEL_PLAYER_BODY, D3DXVECTOR3(0.0f, 38.2f, 0.0f), D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f));				//�̂̃��f���𐶐�����

	pModel->m_pModel[HEAD] = CModelPart::Create(CModel::MODEL_PLAYER_HEAD, D3DXVECTOR3(0.0f, 21.5f, 0.0f), D3DXVECTOR3(1.51f, 0.0f, 0.0f));							//���̃��f���𐶐�����
	pModel->m_pModel[HEAD]->SetParent(pModel->m_pModel[BODY]);																//���̐e��ݒ肷��

	pModel->m_pModel[RIGHT_ARM] = CModelPart::Create(CModel::MODEL_PLAYER_FOREARM_RIGHT, D3DXVECTOR3(-10.0f, 25.0f, 0.0f), D3DXVECTOR3(-0.03f, 0.0f, -1.35f));	//�E�r�̃��f���𐶐�����
	pModel->m_pModel[RIGHT_ARM]->SetParent(pModel->m_pModel[BODY]);															//�E�r�̐e��ݒ肷��

	pModel->m_pModel[RIGHT_HAND] = CModelPart::Create(CModel::MODEL_PLAYER_ARM_RIGHT, D3DXVECTOR3(-9.0f, 0.0f, 0.0f), Vec3Null);	//�E��̃��f���𐶐�����
	pModel->m_pModel[RIGHT_HAND]->SetParent(pModel->m_pModel[RIGHT_ARM]);													//�E��̐e��ݒ肷��

	pModel->m_pModel[LEFT_ARM] = CModelPart::Create(CModel::MODEL_PLAYER_FOREARM_LEFT, D3DXVECTOR3(10.0f, 25.0f, 0.0f), D3DXVECTOR3(-0.03f, 0.0f, 1.35f));		//���r�̃��f���𐶐�����
	pModel->m_pModel[LEFT_ARM]->SetParent(pModel->m_pModel[BODY]);															//���r�̐e��ݒ肷��

	pModel->m_pModel[LEFT_HAND] = CModelPart::Create(CModel::MODEL_PLAYER_ARM_LEFT, D3DXVECTOR3(9.0f, 0.0f, 0.0f), Vec3Null);		//����̃��f���𐶐�����
	pModel->m_pModel[LEFT_HAND]->SetParent(pModel->m_pModel[LEFT_ARM]);														//����̐e��ݒ肷��

	pModel->m_pModel[RIGHT_LEG] = CModelPart::Create(CModel::MODEL_PLAYER_LEG_RIGHT, D3DXVECTOR3(-4.0f, 0.0f, 0.0f), Vec3Null);	//�E���ڂ̃��f���𐶐�����
	pModel->m_pModel[RIGHT_LEG]->SetParent(pModel->m_pModel[BODY]);															//�E���ڂ̐e��ݒ肷��

	pModel->m_pModel[RIGHT_FOOT] = CModelPart::Create(CModel::MODEL_PLAYER_FOOT_RIGHT, D3DXVECTOR3(-1.0f, -10.0f, -1.0f), Vec3Null);//�E���̃��f���𐶐�����
	pModel->m_pModel[RIGHT_FOOT]->SetParent(pModel->m_pModel[RIGHT_LEG]);													//�E���̐e��ݒ肷��

	pModel->m_pModel[LEFT_LEG] = CModelPart::Create(CModel::MODEL_PLAYER_LEG_LEFT, D3DXVECTOR3(4.0f, 0.0f, 0.0f), Vec3Null);		//�����ڂ̃��f���𐶐�����
	pModel->m_pModel[LEFT_LEG]->SetParent(pModel->m_pModel[BODY]);															//�����ڂ̐e��ݒ肷��

	pModel->m_pModel[LEFT_FOOT] = CModelPart::Create(CModel::MODEL_PLAYER_FOOT_LEFT, D3DXVECTOR3(1.0f, -10.0f, -1.0f), Vec3Null);	//�����̃��f���𐶐�����
	pModel->m_pModel[LEFT_FOOT]->SetParent(pModel->m_pModel[LEFT_LEG]);														//�����̐e��ݒ肷��

												
	//�����������f�����A�j���[�^�[�ɑ������
	std::vector <CModelPart*> vParts;
	vParts.clear();
	vParts.push_back(pModel->m_pModel[BODY]);
	vParts.push_back(pModel->m_pModel[HEAD]);
	vParts.push_back(pModel->m_pModel[LEFT_ARM]);
	vParts.push_back(pModel->m_pModel[LEFT_HAND]);
	vParts.push_back(pModel->m_pModel[RIGHT_ARM]);
	vParts.push_back(pModel->m_pModel[RIGHT_HAND]);
	vParts.push_back(pModel->m_pModel[LEFT_LEG]);
	vParts.push_back(pModel->m_pModel[LEFT_FOOT]);
	vParts.push_back(pModel->m_pModel[RIGHT_LEG]);
	vParts.push_back(pModel->m_pModel[RIGHT_FOOT]);

	pModel->m_pAnimator = CAnimator::Create(&vParts, CAnimator::ANIM_TYPE_PLAYER);	
	pModel->m_pAnimator->SetPresentAnim(STATE_MOVING);

	vParts.clear();
	vParts.shrink_to_fit();

	pModel->m_pHitbox = CCylinderHitbox::Create(pos, Vec3Null, m_playerSize, CHitbox::TYPE_PLAYER, pModel);

	if (pModel->m_pHitbox)
	{
		pModel->m_pHitbox->SetOverlapResponse(CHitbox::TYPE_OBSTACLE, CHitbox::RESPONSE_OVERLAP);
		pModel->m_pHitbox->SetOverlapResponse(CHitbox::TYPE_BUTTON, CHitbox::RESPONSE_OVERLAP);
		pModel->m_pHitbox->SetOverlapResponse(CHitbox::TYPE_VANISHING, CHitbox::RESPONSE_OVERLAP);
		pModel->m_pHitbox->SetOverlapResponse(CHitbox::TYPE_FALL, CHitbox::RESPONSE_OVERLAP);
		pModel->m_pHitbox->SetOverlapResponse(CHitbox::TYPE_OVERLAPPABLE, CHitbox::RESPONSE_OVERLAP);
	}

	return pModel;					//���������C���X�^���X��Ԃ�
}

//�v���C���[�̐F�̎擾����
D3DXCOLOR* CPlayer::GetPlayerColors(void)
{
	return m_playerColor;
}

//�v���C���[�̃L�[����
void CPlayer::PlayerController(void)
{
	D3DXVECTOR3 cameraRot = CApplication::GetCamera()->GetRot();					//�J�����̌����̎擾����
	D3DXVECTOR3 cR = D3DXVECTOR3(-cosf(cameraRot.y), 0.0f, sinf(cameraRot.y));
	float fA = acosf(cR.x);

	if (!m_bFall)
	{
		if (CInputKeyboard::GetKeyboardPress(DIK_A))
		{
			m_pos.x += -m_fFrameMove;
		}
		else if (CInputKeyboard::GetKeyboardPress(DIK_D))
		{
			m_pos.x += m_fFrameMove;
		}

		m_pos.z += m_fFrameMove;

		//ControlBalance();
	}

}


void CPlayer::SetFriction(const float fFriction)
{
	m_fFrictionCoeff = fFriction;
}

void CPlayer::RespawnPlayer(void)
{
	m_pos = D3DXVECTOR3(0.0f, -100.0f, 0.0f);

	m_nInvincibilityCnt = 90;
	m_pHitbox->SetInvincibility(true);
}

void CPlayer::HitboxEffectUpdate(void)
{
	CHitbox::INTERACTION_EFFECT effect = m_pHitbox->GetEffect();

	switch (effect)
	{
	case CHitbox::EFFECT_DAMAGE:

	{
		D3DXVECTOR3 dir = m_pHitbox->GetDirection();

		m_move.x = 0.0f;
		m_move.y = 15.0f;
		m_move.z = 0.0f;

		if (m_pHitbox->GetScore() < 0)
		{
			if (m_pAnimator && !m_bFall)
			{
				m_pAnimator->SetPresentAnim(STATE_FALL);
			}

			m_bFall = true;
		}

		m_pHitbox->SetEffect(CHitbox::EFFECT_MAX);
	}

	break;

	case CHitbox::EFFECT_LAUNCH:

		break;

	case CHitbox::EFFECT_PUSH:
		break;

	case CHitbox::EFFECT_BOUNCE:

	{
		D3DXVECTOR3 dir = m_pHitbox->GetDirection();

		m_move.x = dir.x * 20.0f;
		m_move.y = 15.0f;
		m_move.z = dir.z * 20.0f;

		if (m_pHitbox->GetScore() < 0)
		{
			if (m_pAnimator && !m_bFall)
			{
				m_pAnimator->SetPresentAnim(STATE_FALL);
			}

			m_bFall = true;
		}

		m_pHitbox->SetEffect(CHitbox::EFFECT_MAX);
	}

	break;

	case CHitbox::EFFECT_JUMP:
		break;

	case CHitbox::EFFECT_FALL:

	{
		if (m_pAnimator && !m_bFall)
		{
			m_pAnimator->SetPresentAnim(STATE_FALL);
		}

		m_bFall = true;

		m_move.x = 0.0f;
		m_move.z = 0.0f;

		m_pHitbox->SetEffect(CHitbox::EFFECT_MAX);
	}

		break;

	default:
		break;
	}
}

//�o�����X�̏���
void CPlayer::ControlBalance(void)
{
	m_nCntBalance++;

	if(m_nCntBalance >= m_nBalanceChangeTime)
	{
		m_nCntBalance = 0;

		m_nBalanceChangeTime = random(-m_nTimeRange, m_nTimeRange);

		m_fFrameBalance = (float)random(-m_nFrameBalanceRange, m_nFrameBalanceRange) * 0.02f;
	}


	//�}�E�X�J�[�\���̈ʒu�̎擾�ƕϊ�
	POINT pt;
	GetCursorPos(&pt);
	HWND wnd = CApplication::GetWindow();
	ScreenToClient(wnd, &pt);
	D3DXVECTOR3 MousePos, Target;
	MousePos.x = (float)pt.x;
	MousePos.y = (float)pt.y;
	MousePos.z = 0.0f;

	if (MousePos.x < (float)SCREEN_WIDTH * 0.5f - m_fMouseSafeRadius || MousePos.x > (float)SCREEN_WIDTH * 0.5f + m_fMouseSafeRadius)
	{
		float fM = ((float)SCREEN_WIDTH * 0.5f - MousePos.x) * m_fMouseSensibilityCoefficient;
		m_fBalance += fM;
	}

	m_fBalance += m_fFrameBalance;

	if (m_fBalance >= m_fFallLimit || m_fBalance <= -m_fFallLimit)
	{
		if (m_pAnimator && !m_bFall)
		{
			m_pAnimator->SetPresentAnim(STATE_FALL);
		}

		m_bFall = true;
	}

	CDebugProc::Print("\n\n Balance: %f", m_fBalance);
}
