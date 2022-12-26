//=============================================================================
//
// カウントダウン処理 [countdown.cpp]
// Author : KADO TAKUMA
//
//=============================================================================
#include "countdown.h"
#include "application.h"
#include "rendering.h"
#include "sound.h"
#include <assert.h>

//=============================================================================
// コンストラクタ
//=============================================================================
CCountDown::CCountDown() :
	CObject_2D(5)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CCountDown::~CCountDown()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCountDown::Init(void)
{
	//オブジェクトの初期化処理
	CObject_2D::Init();

	//頂点カラーの設定
	CObject_2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//頂点サイズの設定
	CObject_2D::SetSize(D3DXVECTOR2(250.0f, 250.0f));

	//派生のテクスチャポインタを親のテクスチャポインタに代入する処理
	SetTexture(CObject::TEXTURE_COUNTDOWN_3);

	////サウンドの再生
	//PlaySound(SOUND_LABEL_SE_COUNTDOWN);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCountDown::Uninit(void)
{
	//オブジェクトの終了処理
	CObject_2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CCountDown::Update(void)
{
	//カウンター加算
	m_nCounter++;

	//カウントダウンに合わせて画像変更
	if (m_nCounter == 60)
	{
		//派生のテクスチャポインタを親のテクスチャポインタに代入する処理
		SetTexture(CObject::TEXTURE_COUNTDOWN_2);

		////サウンドの再生
		//PlaySound(SOUND_LABEL_SE_COUNTDOWN);
	}
	else if (m_nCounter == 120)
	{
		//派生のテクスチャポインタを親のテクスチャポインタに代入する処理
		SetTexture(CObject::TEXTURE_COUNTDOWN_1);

		////サウンドの再生
		//PlaySound(SOUND_LABEL_SE_COUNTDOWN);
	}
	else if (m_nCounter == 180)
	{
		//派生のテクスチャポインタを親のテクスチャポインタに代入する処理
		SetTexture(CObject::TEXTURE_COUNTDOWN_0);
	}
	else if (m_nCounter == 240)
	{
		//終了処理
		Release();
		return;
	}

	//オブジェクトの更新処理
	CObject_2D::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CCountDown::Draw(void)
{
	//オブジェクトの描画処理
	CObject_2D::Draw();
}

//=============================================================================
// 生成処理
//=============================================================================
CCountDown * CCountDown::Create(D3DXVECTOR3 pos)
{
	//ポインタ宣言
	CCountDown *pCountDown;

	//インスタンス生成
	pCountDown = new CCountDown;

	if (pCountDown != nullptr)
	{//ポインタが存在したら実行
		pCountDown->Init();
		pCountDown->SetPos(pos);
	}
	else
	{//ポインタが虚無だったら実行
		assert(false);
	}

	return pCountDown;
}
