//=============================================================================
//
// nail.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "nail.h"
#include "BoxHitbox.h"
#include "application.h"
#include "rendering.h"
#include "phongShading.h"
#include "camera.h"
#include "directionalLight.h"

//コンストラクタ
CNail::CNail()
{
	//メンバー変数をクリアする
	m_pHitbox = nullptr;				//ヒットボックス
}

//デストラクタ
CNail::~CNail()
{

}

//初期化処理
HRESULT CNail::Init(void)
{
	//メンバー変数を初期化する
	if (FAILED(CModel::Init()))
	{//親クラスの初期化処理
		return -1;
	}

	m_pHitbox = nullptr;

	return S_OK;
}

//終了処理
void CNail::Uninit(void)
{
	//ヒットボックスの破棄処理
	if (m_pHitbox)
	{
		m_pHitbox->Release();
		m_pHitbox = nullptr;
	}
}

//更新処理
void CNail::Update(void)
{
	if (GetPos().z <= CApplication::GetCamera()->GetPos().z - 500.0f)
	{
		Release();
	}
}

//描画処理
void CNail::Draw(void)
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



//=============================================================================
//
// 
//
//=============================================================================




//生成処理
CNail* CNail::Create(const D3DXVECTOR3 pos)
{
	CNail* pNail = new CNail;			//インスタンスを生成する

	if (FAILED(pNail->Init()))
	{//初期化処理
		return nullptr;
	}

	pNail->SetModel(CModel::MODEL_NAILS);		//モデルの設定
	pNail->SetPos(pos);							//位置を設定する
	pNail->SetScale(0.75f);						//スケールを設定する
	pNail->SetShadowDraw(false);				//影を描画しないように設定する

	float fRot = (float)CObject::random(-31415, 31415) * 0.0001f;
	pNail->SetRot(D3DXVECTOR3(0.0f, fRot, 0.0f));

	pNail->m_pHitbox = CBoxHitbox::Create(pos, Vec3Null, D3DXVECTOR3(37.0f, 10.0f, 37.0f), CHitbox::TYPE_OBSTACLE, pNail, -1, CHitbox::EFFECT_BOUNCE);		//ヒットボックスの生成

	return pNail;
}