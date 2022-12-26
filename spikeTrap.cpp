//=============================================================================
//
// spikeTrap.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "spikeTrap.h"
#include "BoxHitbox.h"
#include "application.h"
#include "rendering.h"
#include "camera.h"
#include "directionalLight.h"
#include "phongShading.h"


//コンストラクタ
CSpikeTrap::CSpikeTrap()
{
	//メンバー変数をクリアする
	m_nDelay = 0;
	m_nCntTime = 0;
	m_fSpeed = 0.0f;
	m_fStartingHeight = 0.0f;
	m_bMoving = false;

	m_pHitbox = nullptr;
}

//デストラクタ
CSpikeTrap::~CSpikeTrap()
{

}

//初期化処理
HRESULT CSpikeTrap::Init(void)
{
	//基本クラスの初期化処理
	if (FAILED(CModel::Init()))
	{
		return -1;
	}

	//メンバー変数を初期化する
	m_nDelay = 60;
	m_nCntTime = 0;
	m_fSpeed = 1.0f;
	m_fStartingHeight = 0.0f;
	m_bMoving = false;

	m_pHitbox = nullptr;

	return S_OK;
}

//終了処理
void CSpikeTrap::Uninit(void)
{
	//ヒットボックスの破棄
	if (m_pHitbox)
	{//nullチェック
		m_pHitbox->Release();			//メモリを解放する
		m_pHitbox = nullptr;			//ポインタをnullにする
	}

	//基本クラスの終了処理
	CModel::Uninit();
}

//更新処理
void CSpikeTrap::Update(void)
{
	//上下移動の処理
	if (m_bMoving)
	{//動いていたら

		D3DXVECTOR3 pos = GetPos();		//位置の取得

		pos.y += m_fSpeed;				//高さの更新

		//限界を超えたら、戻してから、速度を逆にして、動かないように設定する
		if (pos.y >= m_fStartingHeight + 40.0f)
		{
			m_fSpeed *= -1.0f;
			pos.y = m_fStartingHeight + 40.0f;
			m_bMoving = false;
		}
		else if (pos.y <= m_fStartingHeight)
		{
			pos.y = m_fStartingHeight;
			m_fSpeed *= -1.0f;
			m_bMoving = false;
		}

		SetPos(pos);				//位置の設定
	}
	else
	{//動いていなかったら

		m_nCntTime++;			//カウンターをインクリメントする

		if (m_nCntTime >= m_nDelay)
		{//設定した時間が経ったら、カウンターを0に戻して、動くように設定する
			m_nCntTime = 0;
			m_bMoving = true;
		}
	}

	//ヒットボックスの更新処理
	if (m_pHitbox)
	{//nullチェック
		m_pHitbox->SetPos(GetPos());		//ヒットボックスの位置の更新

		m_pHitbox->Update();
	}
}

//描画処理
void CSpikeTrap::Draw(void)
{
	CPhongShading* pPhong = CApplication::GetRenderer()->GetPhongEffect();

	if (pPhong)
	{
		D3DXVECTOR3 posV = CApplication::GetCamera()->GetPos();		//視点の取得

		//視点の位置をVector4に変換する
		D3DXVECTOR4 cameraPos = {};									

		cameraPos.x = posV.x;
		cameraPos.y = posV.y;
		cameraPos.z = posV.z;
		cameraPos.w = 1.0f;

		//位置を取得してから、Vector4に変換して、正規化する
		D3DXVECTOR3 l = CDirectionalLight::GetPrincipalLightDir();
		D3DXVec3Normalize(&l, &l);
		D3DXVECTOR4 light = D3DXVECTOR4(posV.x - l.x, posV.y - l.y, posV.z - l.z, 0.0f);
		D3DXVec3Normalize((D3DXVECTOR3*)&light, (D3DXVECTOR3*)&light);

		pPhong->Begin();											//シェーダーエフェクトの開始処理
		pPhong->SetAmbient(0.50f);									//環境光の設定
		pPhong->SetSpecular(40.0f);									//スペキュラーの範囲の設定
		pPhong->SetSpecularPower(0.75f);							//スペキュラーの強度の設定
		pPhong->SetMatrix(GetWorldMatrix(), &cameraPos, &light);	//ワールドマトリックスの設定
		pPhong->BeginPass();										//パスの開始
	}

	//基本クラスの描画処理
	CModel::Draw();

	if (pPhong)
	{
		pPhong->EndPass();		//パスの終了処理
		pPhong->End();			//シェーダーエフェクトの終了処理
	}
}

//初期時のディレイの設定処理
void CSpikeTrap::SetStartingDelay(const int nDelay)
{
	m_nCntTime = -nDelay;
}


//=============================================================================
//
//								静的関数
//
//=============================================================================



//生成処理
CSpikeTrap * CSpikeTrap::Create(const D3DXVECTOR3 pos)
{
	CSpikeTrap* pTrap = new CSpikeTrap;			//インスタンスを生成する

	if (FAILED(pTrap->Init()))
	{//初期化処理
		return nullptr;
	}

	pTrap->SetPos(pos);								//位置の設定
	pTrap->SetModel(CModel::MODEL_SPIKE_TRAP);		//モデルの設定
	pTrap->m_fStartingHeight = pos.y - 40.0f;		//初期時の高さの設定

													//ヒットボックスの生成
	pTrap->m_pHitbox = CBoxHitbox::Create(D3DXVECTOR3(pos.x, pos.y - 40.0f, pos.z), Vec3Null, D3DXVECTOR3(32.0f, 35.0f, 32.0f), CHitbox::TYPE_OBSTACLE, pTrap, -1, CHitbox::EFFECT_BOUNCE);

	CModel* pModel = CModel::Create(CModel::MODEL_SPIKE_BASE, D3DXVECTOR3(pos.x, pos.y + 0.1f, pos.z));			//下のモデルの生成
	pModel->SetShadowDraw(false);

	return pTrap;					//生成したインスタンスを返す
}

//生成処理
CSpikeTrap* CSpikeTrap::Create(const D3DXVECTOR3 pos, const float fSpeed)
{
	CSpikeTrap* pTrap = new CSpikeTrap;			//インスタンスを生成する

	if (FAILED(pTrap->Init()))
	{//初期化処理
		return nullptr;
	}

	pTrap->SetPos(pos);								//位置の設定
	pTrap->SetModel(CModel::MODEL_SPIKE_TRAP);		//モデルの設定
	pTrap->m_fSpeed = fSpeed;						//速度の設定
	pTrap->m_fStartingHeight = pos.y - 40.0f;		//初期時の高さの設定

	//ヒットボックスの生成
	pTrap->m_pHitbox = CBoxHitbox::Create(D3DXVECTOR3(pos.x, pos.y - 40.0f, pos.z), Vec3Null, D3DXVECTOR3(32.0f, 35.0f, 32.0f), CHitbox::TYPE_OBSTACLE, pTrap, -1, CHitbox::EFFECT_BOUNCE);

	CModel* pModel = CModel::Create(CModel::MODEL_SPIKE_BASE, D3DXVECTOR3(pos.x, pos.y + 0.1f, pos.z));			//下のモデルの生成
	pModel->SetShadowDraw(false);

	return pTrap;					//生成したインスタンスを返す

}

//生成処理(delayというのは下げたら、前回上がるまでのフレームです。デフォルトは60)
CSpikeTrap* CSpikeTrap::Create(const D3DXVECTOR3 pos, const float fSpeed, const int nDelay)
{
	CSpikeTrap* pTrap = new CSpikeTrap;			//インスタンスを生成する

	if (FAILED(pTrap->Init()))
	{//初期化処理
		return nullptr;
	}

	pTrap->SetPos(pos);								//位置の設定
	pTrap->SetModel(CModel::MODEL_SPIKE_TRAP);		//モデルの設定
	pTrap->m_fSpeed = fSpeed;						//速度の設定
	pTrap->m_fStartingHeight = pos.y - 40.0f;		//初期時の高さの設定

	if (nDelay <= 0)
	{
		pTrap->m_nDelay = 1;							//ディレイの設定
	}
	else
	{
		pTrap->m_nDelay = nDelay;						//ディレイの設定
	}

	//ヒットボックスの生成
	pTrap->m_pHitbox = CBoxHitbox::Create(D3DXVECTOR3(pos.x, pos.y - 40.0f, pos.z), Vec3Null, D3DXVECTOR3(32.0f, 35.0f, 32.0f), CHitbox::TYPE_OBSTACLE, pTrap, -1, CHitbox::EFFECT_BOUNCE);

	CModel* pModel = CModel::Create(CModel::MODEL_SPIKE_BASE, D3DXVECTOR3(pos.x, pos.y + 0.1f, pos.z));			//下のモデルの生成
	pModel->SetShadowDraw(false);

	return pTrap;				//生成したインスタンスを返す
}

CSpikeTrap * CSpikeTrap::Create(const D3DXVECTOR3 pos, const float fSpeed, const int nDelay, const int nStartDelay)
{
	CSpikeTrap* pTrap = new CSpikeTrap;			//インスタンスを生成する

	if (FAILED(pTrap->Init()))
	{//初期化処理
		return nullptr;
	}

	pTrap->SetPos(pos);								//位置の設定
	pTrap->SetModel(CModel::MODEL_SPIKE_TRAP);		//モデルの設定
	pTrap->m_fSpeed = fSpeed;						//速度の設定
	pTrap->m_fStartingHeight = pos.y - 40.0f;		//初期時の高さの設定
	pTrap->m_nCntTime = -nStartDelay;				//初期時のディレイの設定

	if (nDelay <= 0)
	{
		pTrap->m_nDelay = 1;							//ディレイの設定
	}
	else
	{
		pTrap->m_nDelay = nDelay;						//ディレイの設定
	}

	//ヒットボックスの生成
	pTrap->m_pHitbox = CBoxHitbox::Create(D3DXVECTOR3(pos.x, pos.y - 40.0f, pos.z), Vec3Null, D3DXVECTOR3(32.0f, 35.0f, 32.0f), CHitbox::TYPE_OBSTACLE, pTrap, -1, CHitbox::EFFECT_BOUNCE);

	CModel* pModel = CModel::Create(CModel::MODEL_SPIKE_BASE, D3DXVECTOR3(pos.x, pos.y + 0.1f, pos.z));			//下のモデルの生成
	pModel->SetShadowDraw(false);

	return pTrap;				//生成したインスタンスを返す
}
