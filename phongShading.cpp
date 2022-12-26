//=============================================================================
//
// phongShading.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "phongShading.h"

//コンストラクタ
CPhongShading::CPhongShading(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;
	m_pEffect = nullptr;
}

//デストラクタ
CPhongShading::~CPhongShading()
{
	if (m_pEffect)
	{
		m_pEffect->Release();
		m_pEffect = nullptr;
	}
}

void CPhongShading::Invalidate()
{
	if (m_pEffect)
	{
		m_pEffect->OnLostDevice();
	}
}

void CPhongShading::Restore()
{
	if (m_pEffect)
	{
		m_pEffect->OnResetDevice();
	}
}

//エフェクトのロード処理
HRESULT CPhongShading::Load()
{
	D3DCAPS9 caps;

	//ハードウェアがサポートするバーテックスシェーダーとピクセルシェーダーのバージョンをチェックする
	m_pDevice->GetDeviceCaps(&caps);

	if (caps.VertexShaderVersion >= D3DVS_VERSION(1, 1) && caps.PixelShaderVersion >= D3DPS_VERSION(2, 0))
	{
		LPD3DXBUFFER pErr = nullptr;

		HRESULT hr = D3DXCreateEffectFromFile(m_pDevice, _T("PhongShading.hlsl"), nullptr, nullptr, 0, nullptr, &m_pEffect, &pErr);
		if (SUCCEEDED(hr))
		{
			m_pTechnique = m_pEffect->GetTechniqueByName("TShader");
			m_pWVP = m_pEffect->GetParameterByName(nullptr, "m_WVP");
			m_pLightDir = m_pEffect->GetParameterByName(nullptr, "m_LightDir");
			m_pEyePos = m_pEffect->GetParameterByName(nullptr, "m_EyePos");
			m_pAmbient = m_pEffect->GetParameterByName(nullptr, "m_Ambient");
			//         m_pEmissive = m_pEffect->GetParameterByName( nullptr, "m_Emissive" );
			m_pVtxCol = m_pEffect->GetParameterByName(nullptr, "m_VtxCol");
			m_pSpecularPower = m_pEffect->GetParameterByName(nullptr, "m_SpecularPower");
			m_pSpecular = m_pEffect->GetParameterByName(nullptr, "m_Specular");

			m_pEffect->SetTechnique(m_pTechnique);
		}

		else
		{
			return -1;
		}
	}

	else
	{
		return -2;
	}

	return S_OK;
}

void CPhongShading::Begin()
{
	if (m_pEffect)
	{
		m_pDevice->GetTransform(D3DTS_VIEW, &m_matView);
		m_pDevice->GetTransform(D3DTS_PROJECTION, &m_matProj);

		m_pEffect->Begin(nullptr, 0);
	}
}

void CPhongShading::BeginPass()
{
	if (m_pEffect)
	{
		m_pEffect->BeginPass(0);
	}
}

void CPhongShading::SetAmbient(float Ambient)
{
	if (m_pEffect)
	{
		D3DXVECTOR4 A;
		A = D3DXVECTOR4(Ambient, Ambient, Ambient, 1.0f);
		m_pEffect->SetVector(m_pAmbient, &A);
	}

	else
	{
		D3DMATERIAL9 old_material;
		m_pDevice->GetMaterial(&old_material);
		old_material.Ambient.r = Ambient;
		old_material.Ambient.g = Ambient;
		old_material.Ambient.b = Ambient;
		old_material.Ambient.a = 1.0f;
		m_pDevice->SetMaterial(&old_material);
	}
}

void CPhongShading::SetAmbient(D3DXVECTOR4* pAmbient)
{
	if (m_pEffect)
	{
		m_pEffect->SetVector(m_pAmbient, pAmbient);
	}
	else
	{
		D3DMATERIAL9 old_material;
		m_pDevice->GetMaterial(&old_material);
		old_material.Ambient.r = pAmbient->x;
		old_material.Ambient.g = pAmbient->y;
		old_material.Ambient.b = pAmbient->z;
		old_material.Ambient.a = pAmbient->w;
		m_pDevice->SetMaterial(&old_material);
	}
}


void CPhongShading::SetEmissive(float Emissive)
{
	if (m_pEffect)
	{
		D3DXVECTOR4 A;
		A = D3DXVECTOR4(Emissive, Emissive, Emissive, 1.0f);
		m_pEffect->SetVector(m_pEmissive, &A);
	}
	else
	{
		D3DMATERIAL9 old_material;
		m_pDevice->GetMaterial(&old_material);
		old_material.Emissive.r = Emissive;
		old_material.Emissive.g = Emissive;
		old_material.Emissive.b = Emissive;
		old_material.Emissive.a = 1.0f;
		m_pDevice->SetMaterial(&old_material);
	}
}

void CPhongShading::SetEmissive(D3DXVECTOR4* pEmissive)
{
	if (m_pEffect)
	{
		m_pEffect->SetVector(m_pEmissive, pEmissive);
	}
	else
	{
		D3DMATERIAL9 old_material;
		m_pDevice->GetMaterial(&old_material);
		old_material.Emissive.r = pEmissive->x;
		old_material.Emissive.g = pEmissive->y;
		old_material.Emissive.b = pEmissive->z;
		old_material.Emissive.a = pEmissive->w;
		m_pDevice->SetMaterial(&old_material);
	}
}


//スペキュラーの範囲を設定する
void CPhongShading::SetSpecular(float Specular)
{
	if (m_pEffect)
	{
		m_pEffect->SetFloat(m_pSpecular, Specular);
	}
}

//スペキュラーの強度を設定する
void CPhongShading::SetSpecularPower(float SpecularPower)
{
	if (m_pEffect)
	{
		m_pEffect->SetFloat(m_pSpecularPower, SpecularPower);
	}
}

void CPhongShading::SetMatrix(D3DXMATRIX* pMatWorld, D3DXVECTOR4* pCameraPos, D3DXVECTOR4* pLightDir)
{
	if (m_pEffect)
	{
		D3DXMATRIX m, m1;
		D3DXVECTOR4 LightDir;
		D3DXVECTOR4 v;

		m = (*pMatWorld) * m_matView * m_matProj;
		m_pEffect->SetMatrix(m_pWVP, &m);

		//カメラ位置
		m1 = (*pMatWorld) * m_matView;
		D3DXMatrixInverse(&m1, NULL, &m1);
		D3DXVec4Transform(&v, pCameraPos, &m1);
		m_pEffect->SetVector(m_pEyePos, &v);

		//ライトの方向ベクトル
		LightDir = *pLightDir;
		D3DXMatrixInverse(&m1, NULL, pMatWorld);
		D3DXVec4Transform(&v, &LightDir, &m1);
		D3DXVec4Normalize(&v, &v);
		m_pEffect->SetVector(m_pLightDir, &v);
	}
	else
	{
		m_pDevice->SetTransform(D3DTS_WORLD, pMatWorld);
	}
}

void CPhongShading::CommitChanges()
{
	if (m_pEffect)
	{
		m_pEffect->CommitChanges();
	}
}

void CPhongShading::EndPass()
{
	if (m_pEffect)
	{
		m_pEffect->EndPass();
	}
}

void CPhongShading::End()
{
	if (m_pEffect)
	{
		m_pEffect->End();
	}
}

BOOL CPhongShading::IsOK()
{
	if (m_pEffect)
	{
		return true;
	}

	return false;
}