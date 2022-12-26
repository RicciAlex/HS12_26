//=============================================================================
//
// phongShading.h
// Author : Ricci Alex
//
//=============================================================================
#ifndef _PHONG_SHADING_H
#define _PHONG_SHADING_H

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"

class CPhongShading
{
public:
	CPhongShading(LPDIRECT3DDEVICE9 pDevice);				//コンストラクタ
	~CPhongShading();											//デストラクタ
	HRESULT Load();											//ロード処理
	void Invalidate();										//終了処理
	void Restore();											
	void Begin();											//開始処理
	void BeginPass();										//パスの開始処理
	void SetAmbient(float Ambient);							//環境光の設定処理
	void SetAmbient(D3DXVECTOR4* pAmbient);					//環境光の設定処理
	   void SetEmissive( float Emissive );				
	   void SetEmissive( D3DXVECTOR4* pEmissive );	
	void SetSpecular(float Specular);						//スペキュラーの範囲の設定処理
	void SetSpecularPower(float SpecularPower);				//スペキュラーの強度の設定処理
	void SetMatrix(D3DXMATRIX* pMatWorld, D3DXVECTOR4* pCameraPos, D3DXVECTOR4* pLightDir);			//マトリックスの設定処理
	void CommitChanges();									//変更のセーブ処理
	void EndPass();											//パスの終了処理
	void End();												//終了処理
	BOOL IsOK();											//使用できるかどうかの終了処理

private:
	LPD3DXEFFECT m_pEffect;					
	D3DXHANDLE m_pTechnique;				
	D3DXHANDLE m_pWVP;						
	D3DXHANDLE m_pLightDir;					
	D3DXHANDLE m_pEyePos;					
	D3DXHANDLE m_pAmbient;					
	D3DXHANDLE m_pEmissive;				
	D3DXHANDLE m_pSpecular;					
	D3DXHANDLE m_pSpecularPower;	
	D3DXHANDLE m_pVtxCol;
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;
	LPDIRECT3DDEVICE9 m_pDevice;			
};

#endif