//=============================================================================
//
// スコアボックス処理 [scorebox.cpp]
// Author : KADO TAKUMA
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "timerbox.h"
#include "rendering.h"
#include "application.h"
#include <assert.h>

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CTimerBox::m_pTexture = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CTimerBox::CTimerBox() :
	CObject_2D(4)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CTimerBox::~CTimerBox()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTimerBox::Init(void)
{
	//オブジェクトの初期化処理
	CObject_2D::Init();

	//頂点サイズの設定
	CObject_2D::SetSize(D3DXVECTOR2(170.0f, 50.0f));

	//頂点カラーの設定
	CObject_2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//派生のテクスチャポインタを親のテクスチャポインタに代入する処理
	SetTexture(CObject::TEXTURE_TIMERBOX);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTimerBox::Uninit(void)
{
	//オブジェクトの終了処理
	CObject_2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTimerBox::Update(void)
{
	//オブジェクトの更新処理
	CObject_2D::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CTimerBox::Draw(void)
{
	//オブジェクトの描画処理
	CObject_2D::Draw();
}

//=============================================================================
// ライフの生成処理
//=============================================================================
CTimerBox *CTimerBox::Create(D3DXVECTOR3 pos)
{
	//ポインタ宣言
	CTimerBox *pScoreBox = nullptr;

	//インスタンス生成
	pScoreBox = new CTimerBox;

	if (pScoreBox != nullptr)
	{//ポインタが存在したら実行
		pScoreBox->Init();
		pScoreBox->SetPos(pos);
	}
	else
	{//ポインタが虚無だったら実行
		assert(false);
	}

	return pScoreBox;
}

//=============================================================================
// テクスチャの読み込み
//=============================================================================
HRESULT CTimerBox::Load(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//テクスチャの読み込み*
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ScoreBox_0.png",
		&m_pTexture);

	return S_OK;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CTimerBox::UnLoad(void)
{
	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
