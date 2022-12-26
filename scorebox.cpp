//=============================================================================
//
// スコアボックス処理 [scorebox.cpp]
// Author : KADO TAKUMA
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "scorebox.h"
#include "renderer.h"
#include "application.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CScoreBox::m_pTexture = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CScoreBox::CScoreBox(int nPriority) :
	CObject2D(nPriority)
{
	//オブジェクトのタイプセット処理
	CObject::SetType(OBJTYPE_SCOREBOX);
}

//=============================================================================
// デストラクタ
//=============================================================================
CScoreBox::~CScoreBox()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScoreBox::Init(void)
{
	//オブジェクトの初期化処理
	CObject2D::Init();

	//頂点サイズの設定
	CObject2D::SetSize(D3DXVECTOR2(SCOREBOX_WIDTH, SCOREBOX_HEIGHT));

	//頂点カラーの設定
	CObject2D::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//派生のテクスチャポインタを親のテクスチャポインタに代入する処理
	BindTexture(m_pTexture);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CScoreBox::Uninit(void)
{
	//オブジェクトの終了処理
	CObject2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CScoreBox::Update(void)
{
	//オブジェクトの更新処理
	CObject2D::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CScoreBox::Draw(void)
{
	//オブジェクトの描画処理
	CObject2D::Draw();
}

//=============================================================================
// ライフの生成処理
//=============================================================================
CScoreBox *CScoreBox::Create(D3DXVECTOR3 pos)
{
	//ポインタ宣言
	CScoreBox *pScoreBox = nullptr;

	//インスタンス生成
	pScoreBox = new CScoreBox;

	if (pScoreBox != nullptr)
	{//ポインタが存在したら実行
		pScoreBox->SetPos(pos);
		pScoreBox->Init();
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
HRESULT CScoreBox::Load(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDivice();

	//テクスチャの読み込み*
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ScoreBox_0.png",
		&m_pTexture);

	return S_OK;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CScoreBox::UnLoad(void)
{
	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
