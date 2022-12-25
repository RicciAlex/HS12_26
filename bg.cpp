//=============================================================================
//
// bg.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "bg.h"
#include "billboard.h"
#include "application.h"
#include "camera.h"

//コンストラクタ
CBg::CBg()
{
	m_pos = Vec3Null;
	m_fFar = 0.0f;

	for (int nCnt = 0; nCnt < MaxBgLayers; nCnt++)
	{
		m_pBg[nCnt] = nullptr;
		m_relativePos[nCnt] = Vec3Null;
	}
}

//デストラクタ
CBg::~CBg()
{

}

//初期化処理
HRESULT CBg::Init(void)
{
	m_pos = Vec3Null;
	m_fFar = 0.0f;

	return S_OK;
}

//終了処理
void CBg::Uninit(void)
{
	for (int nCnt = 0; nCnt < MaxBgLayers; nCnt++)
	{
		if (m_pBg[nCnt])
		{
			m_pBg[nCnt]->Release();
			m_pBg[nCnt] = nullptr;
		}
	}
}

//更新処理
void CBg::Update(void)
{
	CCamera* pCamera = CApplication::GetCamera();

	if (pCamera)
	{
		D3DXVECTOR3 cameraPos = pCamera->GetPos();
		D3DXVECTOR3 dir = pCamera->GetDirection();

		m_pos = cameraPos + (dir * m_fFar);

		for (int nCnt = 0; nCnt < MaxBgLayers; nCnt++)
		{
			if (m_pBg[nCnt])
			{
				m_pBg[nCnt]->SetPos(m_pos + m_relativePos[nCnt]);
			}
		}
	}
}

//描画処理
void CBg::Draw(void)
{

}

//位置の設定処理
void CBg::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//位置の取得処理
const D3DXVECTOR3 CBg::GetPos(void)
{
	return m_pos;
}

//背景の追加処理
void CBg::SetBg(const D3DXVECTOR3 relativePos, const D3DXCOLOR col)
{
	for (int nCnt = 0; nCnt < MaxBgLayers; nCnt++)
	{
		if (!m_pBg[nCnt])
		{
			m_pBg[nCnt] = CBillboard::Create(m_pos, D3DXVECTOR2(100000.0f, 100000.0f));
			m_pBg[nCnt]->SetColor(col);
			m_relativePos[nCnt] = relativePos;

			break;
		}
	}
}

void CBg::SetBg(const D3DXVECTOR3 relativePos, const D3DXCOLOR col, CObject::TextType type)
{
	for (int nCnt = 0; nCnt < MaxBgLayers; nCnt++)
	{
		if (!m_pBg[nCnt])
		{
			m_pBg[nCnt] = CBillboard::Create(m_pos, D3DXVECTOR2(100000.0f, 100000.0f));
			m_pBg[nCnt]->SetColor(col);
			m_pBg[nCnt]->SetTexture(type);
			m_relativePos[nCnt] = relativePos;

			break;
		}
	}
}






CBg* CBg::Create(const D3DXVECTOR3 pos, const float fDistance)
{
	CBg* pBg = new CBg;

	if (FAILED(pBg->Init()))
	{
		return nullptr;
	}

	pBg->m_pos = pos;
	pBg->m_fFar = fDistance;

	return pBg;
}