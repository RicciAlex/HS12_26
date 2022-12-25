//=============================================================================
//
// fade.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "fade.h"
#include "object2D.h"


//=============================================================================
//							静的変数の初期化
//=============================================================================
const float CFade::m_fBaseFadeSpeed = 0.001f;				//基本のフェードスピード

//コンストラクタ
CFade::CFade()
{
	m_fade = FADE_NONE;
	m_pFade = nullptr;
	m_modeNext = CApplication::MODE_TITLE;
	m_fFadeSpeed = 0.0f;
	m_fadeColor = ColorNull;
}

//デストラクタ
CFade::~CFade()
{

}

//初期化処理
HRESULT CFade::Init(void)
{
	m_pFade = nullptr;

	m_modeNext = CApplication::MODE_TITLE;

	m_fade = FADE_NONE;
	m_fFadeSpeed = m_fBaseFadeSpeed;
	m_fadeColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	return S_OK;
}

//終了処理
void CFade::Uninit(void)
{
	if (m_pFade != nullptr)
	{
		m_pFade->Release();
		m_pFade = nullptr;
	}
}

//更新処理
void CFade::Update(void)
{
	switch (m_fade)
	{
	case CFade::FADE_NONE:

		break;

	case CFade::FADE_IN:

	{
		if (m_pFade != nullptr)
		{
			D3DXCOLOR col = m_pFade->GetColor();

			col.a -= m_fFadeSpeed;

			if (col.a <= 0.0f)
			{
				m_pFade->Release();
				m_pFade = nullptr;
				m_fade = FADE_NONE;
				CApplication::SetFade(false);
			}
			else
			{
				m_pFade->SetColor(col);
			}
		}

	}

	break;

	case CFade::FADE_OUT:

	{
		if (m_pFade != nullptr)
		{
			D3DXCOLOR col = m_pFade->GetColor();

			col.a += m_fFadeSpeed;

			if (col.a >= 0.0f)
			{
				m_pFade->Release();
				m_pFade = nullptr;
				m_fade = FADE_IN;

				CApplication::SetMode(m_modeNext);

				SetFadeIn();
			}
			else
			{
				m_pFade->SetColor(col);
			}
		}
	}

	break;

	default:
		break;
	}
}

//描画処理
void CFade::Draw(void)
{
	if (m_pFade != nullptr)
	{
		m_pFade->Draw();
	}
}




//フェードインの設定処理
void CFade::SetFadeIn(void)
{
	m_fade = FADE_IN;

	CApplication::SetFade(true);

	if (m_pFade == nullptr)
	{
		m_pFade = CObject_2D::Create();

		if (m_pFade != nullptr)
		{
			m_pFade->SetTexture(CObject::TEXTURE_NULL);
			m_pFade->SetTextureParameter(1, 1, 1, INT_MAX);
			m_pFade->SetColor(m_fadeColor);
			m_pFade->SetPos(D3DXVECTOR3((float)SCREEN_WIDTH * 0.5f, (float)SCREEN_HEIGHT * 0.5f, 0.0f));
			m_pFade->SetSize(D3DXVECTOR2((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT));
			m_pFade->SetStartingRot(D3DX_PI * 0.5f);
			m_pFade->SetPriority(5);
		}
	}
}

//フェードアウトの設定処理
void CFade::SetFadeOut(CApplication::Mode mode)
{
	m_fade = FADE_OUT;

	CApplication::SetFade(true);

	if (m_pFade == nullptr)
	{
		m_pFade = CObject_2D::Create();

		if (m_pFade != nullptr)
		{
			m_pFade->SetTexture(CObject::TEXTURE_NULL);
			m_pFade->SetTextureParameter(1, 1, 1, INT_MAX);
			m_pFade->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
			m_pFade->SetPos(D3DXVECTOR3((float)SCREEN_WIDTH * 0.5f, (float)SCREEN_HEIGHT * 0.5f, 0.0f));
			m_pFade->SetSize(D3DXVECTOR2((float)SCREEN_WIDTH * 0.5f, (float)SCREEN_HEIGHT * 0.5f));
			m_pFade->SetStartingRot(D3DX_PI * 0.5f);
			m_pFade->SetPriority(5);

			m_modeNext = mode;
		}
	}
}

//生成処理
CFade* CFade::Create(void)
{
	CFade* pFade = new CFade;

	if (FAILED(pFade->Init()))
	{
		return nullptr;
	}

	pFade->SetFadeIn();

	return pFade;
}

//生成処理
CFade* CFade::Create(const float fFadeSpeed)
{
	CFade* pFade = new CFade;

	if (FAILED(pFade->Init()))
	{
		return nullptr;
	}

	pFade->m_fFadeSpeed = fFadeSpeed;
	pFade->SetFadeIn();

	return pFade;
}

//生成処理
CFade* CFade::Create(const D3DXCOLOR fadeColor)
{
	CFade* pFade = new CFade;

	if (FAILED(pFade->Init()))
	{
		return nullptr;
	}

	pFade->m_fadeColor = fadeColor;
	pFade->SetFadeIn();

	return pFade;
}

//生成処理
CFade* CFade::Create(const D3DXCOLOR fadeColor, const float fFadeSpeed)
{
	CFade* pFade = new CFade;

	if (FAILED(pFade->Init()))
	{
		return nullptr;
	}

	pFade->m_fadeColor = fadeColor;
	pFade->m_fFadeSpeed = fFadeSpeed;
	pFade->SetFadeIn();

	return pFade;
}