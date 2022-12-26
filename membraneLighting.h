//=============================================================================
//
// membraneLighting.h
// Author : Ricci Alex
//
//=============================================================================
#ifndef _MEMBRANE_LIGHTING_H
#define _MEMBRANE_LIGHTING_H

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"


class CMembraneShading
{

public:

	CMembraneShading(LPDIRECT3DDEVICE9 pDevice);
	~CMembraneShading();
	void Invalidate();
	void Restore();
	HRESULT Load();
	void Begin();
	void BeginPass();
	void SetAmbient(float Ambient);
	void SetAmbient(D3DXVECTOR4* pAmbient);
	void SetMatrix(D3DXMATRIX* pMatWorld, D3DXVECTOR4* pCameraPos, D3DXVECTOR4* pLightDir);
	void SetSpecular(float Specular);
	void SetSpecularPower(float SpecularPower);
	void SetAlpha(float Alpha);
	void CommitChanges();
	void EndPass();
	void End();
	BOOL IsOK();
	LPD3DXEFFECT GetEffect() { return m_pEffect; };

private:

	LPD3DXEFFECT m_pEffect;
	D3DXHANDLE m_pTechnique;
	D3DXHANDLE m_pWVP;
	D3DXHANDLE m_pLightDir;
	D3DXHANDLE m_pEyePos;
	D3DXHANDLE m_pSpecular;
	D3DXHANDLE m_pSpecularPower;
	D3DXHANDLE m_pAlpha;
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;
	LPDIRECT3DDEVICE9 m_pDevice;
};



#endif