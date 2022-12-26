//=============================================================================
//
// membraneLighting.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "membraneLighting.h"


//コンストラクタ
CMembraneShading::CMembraneShading(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;
	m_pEffect = nullptr;
}

//デストラクタ
CMembraneShading::~CMembraneShading()
{
	if (m_pEffect)
	{
		m_pEffect->Release();
		m_pEffect = nullptr;
	}
}


void CMembraneShading::Invalidate()
{
	if (m_pEffect)
	{
		m_pEffect->OnLostDevice();
	}
}


void CMembraneShading::Restore()
{
	if (m_pEffect)
	{
		m_pEffect->OnResetDevice();
	}
}

//エフェクトのロード処理
HRESULT CMembraneShading::Load()
{
	D3DCAPS9 caps;

	m_pDevice->GetDeviceCaps(&caps);

	if (caps.VertexShaderVersion >= D3DVS_VERSION(2, 0) && caps.PixelShaderVersion >= D3DPS_VERSION(2, 0))
	{
		LPD3DXBUFFER pErr = nullptr;

		HRESULT hr = D3DXCreateEffectFromFile(m_pDevice, _T("MembraneShader.hlsl"), NULL, NULL, 0, NULL, &m_pEffect, &pErr);
		
		if (SUCCEEDED(hr))
		{
			m_pTechnique = m_pEffect->GetTechniqueByName("TShader");
			m_pWVP = m_pEffect->GetParameterByName(NULL, "m_WVP");
			m_pLightDir = m_pEffect->GetParameterByName(NULL, "m_LightDir");
			m_pEyePos = m_pEffect->GetParameterByName(NULL, "m_EyePos");
			m_pSpecularPower = m_pEffect->GetParameterByName(NULL, "m_SpecularPower");
			m_pSpecular = m_pEffect->GetParameterByName(NULL, "m_Specular");
			m_pAlpha = m_pEffect->GetParameterByName(NULL, "m_Alpha");

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

//開始処理
void CMembraneShading::Begin()
{
	if (m_pEffect)
	{
		m_pDevice->GetTransform(D3DTS_VIEW, &m_matView);				//ビューマトリックスを取得する
		m_pDevice->GetTransform(D3DTS_PROJECTION, &m_matProj);		//射影マトリックスを取得する

		m_pEffect->Begin(NULL, 0);				//エフェクトの開始処理
	}
}

//パスの開始処理
void CMembraneShading::BeginPass()
{
	if (m_pEffect)
	{
		m_pEffect->BeginPass(0);
	}
}

void CMembraneShading::SetAmbient(float Ambient)
{
	if (!m_pEffect)
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

void CMembraneShading::SetAmbient(D3DXVECTOR4 * pAmbient)
{
	if (!m_pEffect)
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

void CMembraneShading::SetMatrix(D3DXMATRIX * pMatWorld, D3DXVECTOR4 * pCameraPos, D3DXVECTOR4 * pLightDir)
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

		//Light
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

void CMembraneShading::SetSpecular(float Specular)
{
	if (m_pEffect)
	{
		m_pEffect->SetFloat(m_pSpecular, Specular);
	}
}

void CMembraneShading::SetSpecularPower(float SpecularPower)
{
	if (m_pEffect)
	{
		m_pEffect->SetFloat(m_pSpecularPower, SpecularPower);
	}
}

//α値の設定処理
void CMembraneShading::SetAlpha(float Alpha)
{
	if (m_pEffect)
	{
		m_pEffect->SetFloat(m_pAlpha, Alpha);
	}
}

void CMembraneShading::CommitChanges()
{
	if (m_pEffect)
	{
		m_pEffect->CommitChanges();
	}
}

//パスの終了処理
void CMembraneShading::EndPass()
{
	if (m_pEffect)
	{
		m_pEffect->EndPass();
	}
}

//エフェクトの終了処理
void CMembraneShading::End()
{
	if (m_pEffect)
	{
		m_pEffect->End();
	}
}

//エフェクトが使用できるかどうかの取得処理
BOOL CMembraneShading::IsOK()
{
	if (m_pEffect)
	{
		return true;
	}

	return false;
}
