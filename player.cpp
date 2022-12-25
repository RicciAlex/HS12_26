//=============================================================================
//
// player.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
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

//=============================================================================
//							静的変数の初期化
//=============================================================================

const float CPlayer::m_MaxWalkingSpeed = 7.0f;			//最大歩くスピード
const float CPlayer::m_AccelerationCoeff = 5.0f;		//加速係数
const D3DXVECTOR3 CPlayer::m_playerSize = D3DXVECTOR3(30.0f, 175.0f, 30.0f);				//プレイヤーのサイズ

//プレイヤーの色
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

//コンストラクタ
CPlayer::CPlayer() : CObject::CObject(1)
{
	//メンバー変数をクリアする
	m_LastPos = Vec3Null;							//前回の位置
	m_pos = Vec3Null;								//位置
	m_move = Vec3Null;								//速度の初期化処理
	m_DestRot = Vec3Null;							//目的の角度の初期化処理
	m_pAnimator = nullptr;							//アニメーターへのポインタ
	m_rot = Vec3Null;								//向き
	m_bRot = false;									//回転したかどうか
	m_pAnimator = nullptr;							//アニメーターへのポインタ
	m_State = (STATE)0;								//アニメーション状態
	m_bJump = false;								//ジャンプしているかどうか
	m_bLanded = false;
	m_bMoving = false;								//移動しているかどうか
	m_nInvincibilityCnt = 0;						//無敵状態のカウンター
	m_bAttacking = false;							//攻撃しているかどうか
	m_nCntAttack = 0;								//攻撃カウンター
	m_fFrictionCoeff = 0.0f;						//摩擦係数
	m_bFall = false;								//落下しているかどうか
	m_pHitbox = nullptr;							//ヒットボックス

	for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
	{//モデルの部分へのポインタ
		m_pModel[nCnt] = nullptr;
	}
}

//デストラクタ
CPlayer::~CPlayer()
{

}

//初期化処理
HRESULT CPlayer::Init(void)
{
	//メンバー変数の初期化処理
	m_LastPos = Vec3Null;							//前回の位置
	m_pos = Vec3Null;								//位置
	m_move = Vec3Null;								//速度の初期化処理
	m_DestRot = Vec3Null;							//目的の角度の初期化処理
	m_pAnimator = nullptr;							//アニメーターへのポインタ
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);		//向き
	m_bRot = false;									//回転したかどうか
	m_pAnimator = nullptr;							//アニメーターへのポインタ
	m_State = STATE_NEUTRAL;						//アニメーション状態
	m_bJump = false;								//ジャンプしているかどうか
	m_bLanded = false;
	m_bMoving = false;								//移動しているかどうか
	m_nInvincibilityCnt = 0;						//無敵状態のカウンター
	m_bAttacking = false;							//攻撃しているかどうか
	m_nCntAttack = 0;								//攻撃カウンター
	m_fFrictionCoeff = 0.1f;						//摩擦係数
	m_bFall = false;								//落下しているかどうか
	m_pHitbox = nullptr;							//ヒットボックス

	for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
	{//モデルの部分へのポインタ
		m_pModel[nCnt] = nullptr;
	}

	return S_OK;
}

//終了処理
void CPlayer::Uninit(void)
{
	//モデルパーツの終了処理
	for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
	{
		if (m_pModel[nCnt] != nullptr)
		{
			m_pModel[nCnt]->Uninit();
			delete m_pModel[nCnt];
			m_pModel[nCnt] = nullptr;
		}
	}

	//アニメーターの破棄処理
	if (m_pAnimator != nullptr)
	{
		m_pAnimator->Uninit();
		delete m_pAnimator;
		m_pAnimator = nullptr;
	}

	//ヒットボックスの破棄処理
	if (m_pHitbox != nullptr)
	{
		m_pHitbox->Release();
		m_pHitbox = nullptr;
	}
}

//更新処理
void CPlayer::Update(void)
{
	m_LastPos = m_pos;				//前回の位置の更新

	D3DXVECTOR3 cameraRot = CApplication::GetCamera()->GetRot();					//カメラの向きの取得処理

	//プレイヤーの目的角度の正規化処理
	D3DXVECTOR3 cR = D3DXVECTOR3(-cosf(cameraRot.y), 0.0f, sinf(cameraRot.y));
	float fA = acosf(cR.x);

	if (cR.z < 0.0f)
	{
		fA *= -1.0f;
	}

	if (!CApplication::GetFade() && !m_bFall)
	{//フェードしていなかったら
		PlayerController(0);		//プレイヤーを動かす
	}

	//位置の更新
	m_pos += m_move;				
	CDebugProc::Print("\n%f %f %f", m_move.x, m_move.y, m_move.z);
	
	m_move.x += (0.0f - m_move.x) * m_fFrictionCoeff;		//移動量のXコンポネントの更新
	m_move.y += (0.0f - m_move.y) * 0.1f;					//移動量のYコンポネントの更新
	m_move.z += (0.0f - m_move.z) * m_fFrictionCoeff;		//移動量のZコンポネントの更新


	//目的の角度の正規化処理
	if (m_DestRot.y - (m_rot.y) > D3DX_PI)
	{
		m_DestRot.y -= 2 * D3DX_PI;
	}
	else if (m_DestRot.y - (m_rot.y) < -D3DX_PI)
	{
		m_DestRot.y += 2 * D3DX_PI;
	}


	//回転角度の計算
	D3DXVECTOR3 rot = m_rot + ((m_DestRot - m_rot) * 0.1f);

	//回転角度の設定処理
	//m_pModel[BODY]->SetRot(rot);		

	//回転角度の正規化処理
	m_rot = rot;

	if (m_rot.y > D3DX_PI)
	{
		m_rot.y = -D3DX_PI + (m_rot.y - D3DX_PI);
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y = D3DX_PI - (D3DX_PI + m_rot.y);
	}

	if (m_rot.y < D3DX_PI * -2.0f)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
	else if (m_rot.y > D3DX_PI * 2.0f)
	{
		m_rot.y += D3DX_PI * -2.0f;
	}

	//回転の設定処理
	//m_pModel[BODY]->SetRot(D3DXVECTOR3(m_pModel[BODY]->GetRot().x, fRot, m_pModel[BODY]->GetRot().z));

	//重量を追加する
	if (m_move.y >= -10.0f)
	{
		m_move.y -= 0.65f;
	}

	if (m_move.y <= 0.0f)
	{
		float fHeight = 0.0f;

		if (!m_bFall)
		{
			//メッシュフィールドとの当たり判定
			CMeshfield* pField = CMeshfield::FieldInteraction(this, &fHeight);

			//地面との当たり判定
			if (pField != nullptr)
			{
				if (m_bJump)
				{
					if (m_pAnimator)
					{

						m_bJump = false;		//着地している状態にする
						m_bLanded = true;

						if (!m_bMoving)
						{
							m_pAnimator->SetPresentAnim(CPlayer::STATE_NEUTRAL);
						}
						else
						{
							m_pAnimator->SetPresentAnim(CPlayer::STATE_RUNNING);
						}
					}
				}

				m_bHit = false;			//当たってない状態にする
				//摩擦係数の取得
				m_fFrictionCoeff = pField->GetFriction();

				//影の高さの設定
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
		//アニメーションの更新
		m_pAnimator->Update();

		if (!m_bJump && m_bMoving)
		{
			D3DXVECTOR3 residualMove = m_move;
			residualMove.y = 0.0f;

			if (D3DXVec3Length(&residualMove) <= 0.1f)
			{
				m_pAnimator->SetPresentAnim(STATE_NEUTRAL);
				m_bMoving = false;
			}
		}

		if (m_bJump && m_bLanded)
		{
			m_bJump = false;

			if (!m_bMoving)
			{
				m_pAnimator->SetPresentAnim(CPlayer::STATE_NEUTRAL);
			}
			else
			{
				m_pAnimator->SetPresentAnim(CPlayer::STATE_RUNNING);
			}
		}
	}

	if (m_pos.y <= -1000.0f)
	{
		RespawnPlayer();
		m_bFall = false;
	}

	CCamera* pCamera = CApplication::GetCamera();

	if (pCamera != nullptr)
	{
		D3DXVECTOR3 p = D3DXVECTOR3(0.0f, 250.0f, -350.0f);
		D3DXVECTOR3 q = D3DXVECTOR3(0.0f, 0.0f, 300.0f);
		pCamera->SetPos(m_pos + p, m_pos + q);
	}

	CDebugProc::Print("\n\n Pos: %f %f %f", m_pos.x, m_pos.y, m_pos.z);
}

//描画処理
void CPlayer::Draw(void)
{
	if (m_nInvincibilityCnt % 10 <= 5)
	{//無敵状態だったら、点滅させる

		D3DXMATRIX mtxTrans, mtxRot;												//計算用のマトリックス
		D3DXMatrixIdentity(&m_mtxWorld);											//ワールドマトリックスの初期化処理

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		m_pModel[BODY]->Draw(m_mtxWorld);											//最初のモデルの描画処理

		for (int nCnt = 1; nCnt < PARTS_MAX; nCnt++)
		{//他のモデルの描画処理
			if (m_pModel[nCnt] != nullptr)
			{
				m_pModel[nCnt]->Draw();
			}
		}
	}
}

//位置の設定処理
void CPlayer::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//向きの設定処理
void CPlayer::SetRot(const D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//着地しているかどうかの設定処理
void CPlayer::SetLanded(void)
{
	if (m_bJump)
	{
		if (m_pAnimator)
		{

			m_bJump = false;		//着地している状態にする
			m_bLanded = true;

			if (!m_bMoving)
			{
				m_pAnimator->SetPresentAnim(CPlayer::STATE_NEUTRAL);
			}
			else
			{
				m_pAnimator->SetPresentAnim(CPlayer::STATE_RUNNING);
			}
		}
	}
}

//速度の設定処理
void CPlayer::SetMove(const D3DXVECTOR3 move)
{
	m_move = move;
}

//位置の取得処理
const D3DXVECTOR3 CPlayer::GetPos(void)
{
	return m_pos;
}

//前回の位置の取得処理
const D3DXVECTOR3 CPlayer::GetLastPos(void)
{
	return m_LastPos;
}

//速度の取得処理
const D3DXVECTOR3 CPlayer::GetMove(void)
{
	return m_move;
}

//=============================================================================
//
//								静的関数
//
//=============================================================================

//生成処理
CPlayer* CPlayer::Create(const D3DXVECTOR3 pos, int nCntPlayer)
{
	CPlayer* pModel = new CPlayer;		//インスタンスを生成する

	//プレイヤーの初期化処理
	if (FAILED(pModel->Init()))
	{
		return nullptr;
	}

	pModel->m_pos = pos;						//位置の設定
	pModel->m_LastPos = pos;					//前回の位置の設定

	pModel->m_rot = D3DXVECTOR3(0.0f, -D3DX_PI, 0.0f);
	pModel->m_pModel[BODY] = CModelPart::Create(CModel::MODEL_PLAYER_BODY, D3DXVECTOR3(0.0f, 53.0f, 0.0f), D3DXVECTOR3(0.0f, -2.98f, 0.0f));				//体のモデルを生成する

	pModel->m_pModel[HEAD] = CModelPart::Create(CModel::MODEL_PLAYER_HEAD, D3DXVECTOR3(0.0f, 76.0f, 0.0f), D3DXVECTOR3(0.0f, -0.19f, 0.0f));							//頭のモデルを生成する
	pModel->m_pModel[HEAD]->SetParent(pModel->m_pModel[BODY]);																//頭の親を設定する

	pModel->m_pModel[LEFT_ARM] = CModelPart::Create(CModel::MODEL_PLAYER_FOREARM, D3DXVECTOR3(-14.0f, 63.0f, 0.0f), D3DXVECTOR3(-0.19f, -1.16f, -1.12f));		//左腕のモデルを生成する
	pModel->m_pModel[LEFT_ARM]->SetParent(pModel->m_pModel[BODY]);															//左腕の親を設定する

	pModel->m_pModel[LEFT_HAND] = CModelPart::Create(CModel::MODEL_PLAYER_ARM, D3DXVECTOR3(0.0f, -20.0f, 0.0f), D3DXVECTOR3(2.07f, 0.75f, 0.062f));		//左手のモデルを生成する
	pModel->m_pModel[LEFT_HAND]->SetParent(pModel->m_pModel[LEFT_ARM]);														//左手の親を設定する

	pModel->m_pModel[RIGHT_ARM] = CModelPart::Create(CModel::MODEL_PLAYER_FOREARM, D3DXVECTOR3(14.0f, 63.0f, 0.0f), D3DXVECTOR3(1.13f, 0.062f, 0.43f));	//右腕のモデルを生成する
	pModel->m_pModel[RIGHT_ARM]->SetParent(pModel->m_pModel[BODY]);															//右腕の親を設定する

	pModel->m_pModel[RIGHT_HAND] = CModelPart::Create(CModel::MODEL_PLAYER_ARM, D3DXVECTOR3(-0.0f, -20.0f, 0.0f), D3DXVECTOR3(0.47f, -0.44f, -1.7f));	//右手のモデルを生成する
	pModel->m_pModel[RIGHT_HAND]->SetParent(pModel->m_pModel[RIGHT_ARM]);													//右手の親を設定する

	pModel->m_pModel[LEFT_LEG] = CModelPart::Create(CModel::MODEL_PLAYER_LEG, D3DXVECTOR3(8.0f, 10.0f, 0.0f), Vec3Null);		//左太腿のモデルを生成する
	pModel->m_pModel[LEFT_LEG]->SetParent(pModel->m_pModel[BODY]);															//左太腿の親を設定する

	pModel->m_pModel[LEFT_FOOT] = CModelPart::Create(CModel::MODEL_PLAYER_FOOT, D3DXVECTOR3(0.1f, -30.0f, 0.0f), Vec3Null);	//左足のモデルを生成する
	pModel->m_pModel[LEFT_FOOT]->SetParent(pModel->m_pModel[LEFT_LEG]);														//左足の親を設定する

	pModel->m_pModel[RIGHT_LEG] = CModelPart::Create(CModel::MODEL_PLAYER_LEG, D3DXVECTOR3(-8.0f, 10.0f, 0.0f), Vec3Null);	//右太腿のモデルを生成する
	pModel->m_pModel[RIGHT_LEG]->SetParent(pModel->m_pModel[BODY]);															//右太腿の親を設定する

	pModel->m_pModel[RIGHT_FOOT] = CModelPart::Create(CModel::MODEL_PLAYER_FOOT, D3DXVECTOR3(-0.1f, -30.0f, 0.0f), Vec3Null);//右足のモデルを生成する
	pModel->m_pModel[RIGHT_FOOT]->SetParent(pModel->m_pModel[RIGHT_LEG]);													//右足の親を設定する

	pModel->m_pModel[SAYA] = CModelPart::Create(CModel::MODEL_SAYA, D3DXVECTOR3(12.3f, 31.19f, -15.0f), D3DXVECTOR3(-2.2f, -2.95f, -1.46f));		//鞘のモデルを生成する
	pModel->m_pModel[SAYA]->SetParent(pModel->m_pModel[BODY]);															//鞘の親を設定する

	pModel->m_pModel[KATANA] = CModelPart::Create(CModel::MODEL_KATANA, D3DXVECTOR3(-0.0f, -30.5f, 0.0f), D3DXVECTOR3(2.92f, -1.29f, 0.47f ));	//刀のモデルを生成する
	pModel->m_pModel[KATANA]->SetParent(pModel->m_pModel[RIGHT_HAND]);														//刀の親を設定する

	//生成したモデルをアニメーターに代入する
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
	vParts.push_back(pModel->m_pModel[SAYA]);
	vParts.push_back(pModel->m_pModel[KATANA]);

	pModel->m_pAnimator = CAnimator::Create(&vParts, CAnimator::ANIM_TYPE_PLAYER);	

	vParts.clear();
	vParts.shrink_to_fit();

	pModel->m_pHitbox = CCylinderHitbox::Create(pos, Vec3Null, D3DXVECTOR3(30.0f, 175.0f, 30.0f), CHitbox::TYPE_PLAYER, pModel);

	if (pModel->m_pHitbox)
	{
		pModel->m_pHitbox->SetOverlapResponse(CHitbox::TYPE_OBSTACLE, CHitbox::RESPONSE_OVERLAP);
		pModel->m_pHitbox->SetOverlapResponse(CHitbox::TYPE_BUTTON, CHitbox::RESPONSE_OVERLAP);
		pModel->m_pHitbox->SetOverlapResponse(CHitbox::TYPE_VANISHING, CHitbox::RESPONSE_OVERLAP);
		pModel->m_pHitbox->SetOverlapResponse(CHitbox::TYPE_FALL, CHitbox::RESPONSE_OVERLAP);
		pModel->m_pHitbox->SetOverlapResponse(CHitbox::TYPE_OVERLAPPABLE, CHitbox::RESPONSE_OVERLAP);
	}

	return pModel;					//生成したインスタンスを返す
}

//プレイヤーの色の取得処理
D3DXCOLOR* CPlayer::GetPlayerColors(void)
{
	return m_playerColor;
}

//プレイヤーのキー処理
void CPlayer::PlayerController(int nCntPlayer)
{
	D3DXVECTOR3 cameraRot = CApplication::GetCamera()->GetRot();					//カメラの向きの取得処理
	D3DXVECTOR3 cR = D3DXVECTOR3(-cosf(cameraRot.y), 0.0f, sinf(cameraRot.y));
	float fA = acosf(cR.x);

	if (!m_bAttacking)
	{
		//移動量と目的の角度の計算
		if (CInputKeyboard::GetKeyboardPress(DIK_W))
		{//Wキーが押された場合
			if (CInputKeyboard::GetKeyboardPress(DIK_A))
			{//Aキーも押された場合
				if (m_move.x <= m_MaxWalkingSpeed && m_move.x >= -m_MaxWalkingSpeed)
				{
					m_move.x += m_AccelerationCoeff * cosf(D3DX_PI * 0.25f + cameraRot.y) * (m_fFrictionCoeff);
				}
				if (m_move.z <= m_MaxWalkingSpeed && m_move.z >= -m_MaxWalkingSpeed)
				{
					m_move.z += m_AccelerationCoeff * sinf(D3DX_PI * 0.25f + cameraRot.y) * (m_fFrictionCoeff);
				}

				m_DestRot.y = -D3DX_PI * 0.75f + fA;

				if (!m_bMoving && !m_bJump)
				{
					m_bMoving = true;

					if (m_pAnimator)
					{
						m_pAnimator->SetPresentAnim(STATE_RUNNING);
					}
				}
			}
			else if (CInputKeyboard::GetKeyboardPress(DIK_D))
			{//Dキーも押された場合
				if (m_move.x <= m_MaxWalkingSpeed && m_move.x >= -m_MaxWalkingSpeed)
				{
					m_move.x += m_AccelerationCoeff * cosf(-D3DX_PI * 0.25f + cameraRot.y) * (m_fFrictionCoeff);
				}
				if (m_move.z <= m_MaxWalkingSpeed && m_move.z >= -m_MaxWalkingSpeed)
				{
					m_move.z += m_AccelerationCoeff * sinf(-D3DX_PI * 0.25f + cameraRot.y) * (m_fFrictionCoeff);
				}

				m_DestRot.y = -D3DX_PI * 0.25f + fA;

				if (!m_bMoving && !m_bJump)
				{
					m_bMoving = true;

					if (m_pAnimator)
					{
						m_pAnimator->SetPresentAnim(STATE_RUNNING);
					}
				}
			}
			else
			{//Wキーだけが押された場合
				if (m_move.x <= m_MaxWalkingSpeed && m_move.x >= -m_MaxWalkingSpeed)
				{
					m_move.x += m_AccelerationCoeff * cosf(cameraRot.y) * (m_fFrictionCoeff);
				}
				if (m_move.z <= m_MaxWalkingSpeed && m_move.z >= -m_MaxWalkingSpeed)
				{
					m_move.z += m_AccelerationCoeff * sinf(cameraRot.y) * (m_fFrictionCoeff);
				}

				m_DestRot.y = -D3DX_PI * 0.5f + fA;

				if (!m_bMoving && !m_bJump)
				{
					m_bMoving = true;

					if (m_pAnimator)
					{
						m_pAnimator->SetPresentAnim(STATE_RUNNING);
					}
				}
			}
		}
		else if (CInputKeyboard::GetKeyboardPress(DIK_S))
		{//Sキーが押された場合
			if (CInputKeyboard::GetKeyboardPress(DIK_A))
			{//Aキーも押された場合
				if (m_move.x <= m_MaxWalkingSpeed && m_move.x >= -m_MaxWalkingSpeed)
				{
					m_move.x += m_AccelerationCoeff * cosf(D3DX_PI * 0.75f + cameraRot.y) * (m_fFrictionCoeff);
				}
				if (m_move.z <= m_MaxWalkingSpeed && m_move.z >= -m_MaxWalkingSpeed)
				{
					m_move.z += m_AccelerationCoeff * sinf(D3DX_PI * 0.75f + cameraRot.y) * (m_fFrictionCoeff);
				}

				m_DestRot.y = D3DX_PI * 0.75f + fA;

				if (!m_bMoving && !m_bJump)
				{
					m_bMoving = true;

					if (m_pAnimator)
					{
						m_pAnimator->SetPresentAnim(STATE_RUNNING);
					}
				}
			}
			else if (CInputKeyboard::GetKeyboardPress(DIK_D))
			{//Dキーも押された場合
				if (m_move.x <= m_MaxWalkingSpeed && m_move.x >= -m_MaxWalkingSpeed)
				{
					m_move.x += m_AccelerationCoeff * cosf(-D3DX_PI * 0.75f + cameraRot.y) * (m_fFrictionCoeff);
				}
				if (m_move.z <= m_MaxWalkingSpeed && m_move.z >= -m_MaxWalkingSpeed)
				{
					m_move.z += m_AccelerationCoeff * sinf(-D3DX_PI * 0.75f + cameraRot.y) * (m_fFrictionCoeff);
				}

				m_DestRot.y = D3DX_PI * 0.25f + fA;

				if (!m_bMoving && !m_bJump)
				{
					m_bMoving = true;

					if (m_pAnimator)
					{
						m_pAnimator->SetPresentAnim(STATE_RUNNING);
					}
				}
			}
			else
			{//Sキーだけが押された場合
				if (m_move.x <= m_MaxWalkingSpeed && m_move.x >= -m_MaxWalkingSpeed)
				{
					m_move.x += m_AccelerationCoeff * cosf(D3DX_PI + cameraRot.y) * (m_fFrictionCoeff);
				}
				if (m_move.z <= m_MaxWalkingSpeed && m_move.z >= -m_MaxWalkingSpeed)
				{
					m_move.z += m_AccelerationCoeff * sinf(D3DX_PI + cameraRot.y) * (m_fFrictionCoeff);
				}

				m_DestRot.y = D3DX_PI * 0.5f + fA;

				if (!m_bMoving && !m_bJump)
				{
					m_bMoving = true;

					if (m_pAnimator)
					{
						m_pAnimator->SetPresentAnim(STATE_RUNNING);
					}
				}
			}
		}
		else if (CInputKeyboard::GetKeyboardPress(DIK_D))
		{//Dキーだけ押された場合
			if (m_move.x <= m_MaxWalkingSpeed && m_move.x >= -m_MaxWalkingSpeed)
			{
				m_move.x += m_AccelerationCoeff * cosf(-D3DX_PI * 0.5f + cameraRot.y) * (m_fFrictionCoeff);
			}
			if (m_move.z <= m_MaxWalkingSpeed && m_move.z >= -m_MaxWalkingSpeed)
			{
				m_move.z += m_AccelerationCoeff * sinf(-D3DX_PI * 0.5f + cameraRot.y) * (m_fFrictionCoeff);
			}

			m_DestRot.y = fA;

			if (!m_bMoving && !m_bJump)
			{
				m_bMoving = true;

				if (m_pAnimator)
				{
					m_pAnimator->SetPresentAnim(STATE_RUNNING);
				}
			}
		}
		else if (CInputKeyboard::GetKeyboardPress(DIK_A))
		{//Aキーだけ押された場合
			if (m_move.x <= m_MaxWalkingSpeed && m_move.x >= -m_MaxWalkingSpeed)
			{
				m_move.x += m_AccelerationCoeff * cosf(D3DX_PI * 0.5f + cameraRot.y) * (m_fFrictionCoeff);
			}
			if (m_move.z <= m_MaxWalkingSpeed && m_move.z >= -m_MaxWalkingSpeed)
			{
				m_move.z += m_AccelerationCoeff * sinf(D3DX_PI * 0.5f + cameraRot.y) * (m_fFrictionCoeff);
			}
			m_DestRot.y = D3DX_PI + fA;

			if (!m_bMoving && !m_bJump)
			{
				m_bMoving = true;
				
				if (m_pAnimator)
				{
					m_pAnimator->SetPresentAnim(STATE_RUNNING);
				}
			}
		}
	}

	//SPACEキーが押された場合
	if (CInputKeyboard::GetKeyboardTrigger(DIK_SPACE) && !m_bJump && !m_bAttacking && m_move.y < 0.0f)
	{//ジャンプ
		m_move.y = 30.0f;
 		m_bJump = true;
		m_bLanded = false;

		if (m_pAnimator != nullptr)
		{
			m_pAnimator->SetPresentAnim(CPlayer::STATE_JUMP_START);
		}
	}

	//攻撃キーが押されている時
	if (CInputKeyboard::GetKeyboardTrigger(DIK_V) &&  !m_bJump && m_bLanded && !m_bHit && !m_bAttacking)
	{
		m_bAttacking = true;
		m_nCntAttack = 19;
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
			m_nInvincibilityCnt = 90;
			m_pHitbox->SetInvincibility(true);
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
			m_nInvincibilityCnt = 90;
			m_pHitbox->SetInvincibility(true);
		}

		m_pHitbox->SetEffect(CHitbox::EFFECT_MAX);
	}

	break;

	case CHitbox::EFFECT_JUMP:
		break;

	case CHitbox::EFFECT_FALL:

	{
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
