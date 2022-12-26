//=============================================================================
//
// application.cpp
// Author : Ricci Alex
//
//=============================================================================

//インクルードファイル
#include "application.h"
#include "rendering.h"
#include "debugProc.h"
#include "object2D.h"
#include "directionalLight.h"
#include "camera.h"
#include "object3D.h"
#include "model.h"
#include "inputKeyboard.h"
#include "inputMouse.h"
#include "meshfield.h"
#include "player.h"
#include "animator.h"
#include "firstStage.h"
#include "fade.h"
#include "silhouette.h"
#include "title.h"
#include "results.h"
#include "BoxHitbox.h"
#include "ranking.h"
#include "tutorial.h"

//静的メンバー変数の宣言
HWND CApplication::m_hWnd;													//ウインドウ
CRenderer* CApplication::m_pRenderer = nullptr;								//レンディングインスタンスへのポインタ
CInputKeyboard* CApplication::m_pKeyboard = nullptr;						//キーボードへのポインタ
//CInputMouse* CApplication::m_pMouse = nullptr;								//マウスインスタンスへのポインタ
//CInputPad* CApplication::m_pPad = nullptr;									//パッドのインスタンス
CSound* CApplication::m_pSound = nullptr;									//サウンドのインスタンス
CFade* CApplication::m_pFade = nullptr;										//フェードのインスタンス
CCamera* CApplication::m_pCamera = nullptr;									//カメラのインスタンス
CMode* CApplication::m_pMode = nullptr;										// モードへのポインタ
//CMenu* CApplication::m_pMenu = nullptr;										// メニューへのポインタ
//CMessage* CApplication::m_pMessage = nullptr;								// メッセージへのポインタ
CDebugProc* CApplication::m_pDebug = nullptr;								//デバッグ表示
CApplication::Mode CApplication::m_mode = CApplication::MODE_TITLE;			//モード
CApplication::Mode CApplication::m_modeNext = CApplication::MODE_TITLE;		//次のモード
bool CApplication::m_bPause = false;										//ポーズ状態であるかどうか
bool CApplication::m_bFade = true;
int CApplication::m_nStageSelect = 0;										// ステージ選択番号

//コンストラクタ
CApplication::CApplication()
{

}

//デストラクタ
CApplication::~CApplication()
{

}

//初期化処理
HRESULT CApplication::Init(HINSTANCE hInstance, HWND hWnd)
{
	m_hWnd = hWnd;				//ウインドウの設定処理

								//レンディングインスタンスの生成処理
	if (m_pRenderer == nullptr)
	{//ポインタはnullではなかったら
		m_pRenderer = new CRenderer;
	}

	//レンディングインスタンスの初期化処理
	if (FAILED(m_pRenderer->Init(hWnd, TRUE)))
	{//第2引数はFALSEだったら、フルスクリーンになります。
		return -1;
	}

	m_pDebug = CDebugProc::Create();			//デバッグテキストの生成処理

												//テクスチャ全部をロード処理
	CObject_2D::LoadTextures();

	//モデル全部をロード処理
	CModel::LoadAllModels();

	//全部のアニメーションの読み込み
	CAnimator::LoadAllAnimation();

	//サウンドの生成
	m_pSound = CSound::Create(hWnd);

	CLight::ReleaseAll();																						//全部のライトのリリース処理
	CDirectionalLight::Create(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(2, -5, 2));						//メインライトの生成
	CDirectionalLight::Create(D3DXCOLOR(0.75f, 0.75f, 0.75f, 0.75f), D3DXVECTOR3(-0.2f, 0.2f, 1.0f));			//ライトの生成

	m_pCamera = CCamera::Create(D3DXVECTOR3(0.0f, 0.0f, -500.0f), D3DXVECTOR3(0.0f, -200.0f, 100.0f));			//カメラの生成

	m_pMode = CTitle::Create();

	m_pFade = CFade::Create(0.01f);

	//キーボードインスタンスの生成処理 
	m_pKeyboard = new CInputKeyboard;

	//キーボードの初期化処理
	if (FAILED(m_pKeyboard->Init(hInstance, hWnd, GUID_SysKeyboard)))
	{
		return -1;
	}

	////パッドの生成
	//m_pPad = new CInputPad;

	//if (m_pPad != nullptr)
	//{//パッドの初期化処理
	//	m_pPad->Init(hInstance, hWnd, GUID_SysMouse);
	//}

	m_bPause = false;	//ポーズを未使用にする

	m_nStageSelect = 0;	// ステージ番号初期化

	CSilhouette::Create();

	return S_OK;
}

//終了処理
void CApplication::Uninit(void)
{
	//Rendererインスタンスの破棄
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit();			//終了処理
		delete m_pRenderer;				//破棄する
		m_pRenderer = nullptr;			//nullにする
	}

	// モードの破棄処理
	if (m_pMode != nullptr)
	{
		m_pMode->Uninit();				//終了処理
		delete m_pMode;					//破棄する
		m_pMode = nullptr;				//nullにする
	}

	//カメラの破棄処理
	if (m_pCamera != nullptr)
	{
		m_pCamera->Uninit();			//終了処理
		delete m_pCamera;				//破棄する
		m_pCamera = nullptr;			//nullにする
	}

	//キーボードの破棄
	if (m_pKeyboard != nullptr)
	{
		m_pKeyboard->Uninit();
		delete m_pKeyboard;
		m_pKeyboard = nullptr;
	}

	//キーボードの破棄
	if (m_pFade != nullptr)
	{
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = nullptr;
	}

	////パッドの破棄処理
	//if (m_pPad != nullptr)
	//{
	//	m_pPad->Uninit();				//終了処理
	//	delete m_pPad;					//破棄する
	//	m_pPad = nullptr;				//nullにする
	//}

	//サウンドの破棄処理
	if (m_pSound != nullptr)
	{
		m_pSound->Uninit();				//終了処理
		delete m_pSound;				//破棄する
		m_pSound = nullptr;				//nullにする
	}

	//デバッグテキストの破棄処理
	if (m_pDebug != nullptr)
	{
		m_pDebug->Uninit();
		delete m_pDebug;				//破棄する
		m_pDebug = nullptr;				//nullにする
	}

	//オブジェクト全体の終了処理
	CObject::ReleaseAll();

	//ヒットボックスの破棄処理
	CHitbox::ReleaseAll();

	CMeshfield::ClearFields();

	//ライトの破棄処理
	CLight::ReleaseAll();

	//テクスチャ全部の破棄
	CObject_2D::DestroyLoadedTextures();

	//全部のアニメーションの破棄処理
	CAnimator::DestroyLoadedAnimation();

	//モデル全部破棄の破棄処理
	CModel::DestroyAllModels();
}

//更新処理
void CApplication::Update(void)
{
	//キーボードの更新処理
	if (m_pKeyboard != nullptr)
	{
		m_pKeyboard->Update();
	}

	// ポーズ中でない場合のみ更新
	if (m_bPause == false)
	{
		//カメラの更新処理
		if (m_pCamera != nullptr)
		{
			m_pCamera->Update();
		}
	}

	//フェードの更新処理
	if (m_pFade != nullptr)
	{
		//フェードが読み込まれていない場合
		if (m_bFade)
		{
			m_pFade->Update();
		}
	}

	//Rendererの更新処理
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Update();
	}

	//現在のモードの更新処理
	if (m_pMode != nullptr)
	{
		if (!m_bPause && !m_bFade)
		{
			m_pMode->Update();
		}
	}

	////パッドの更新処理
	//if (m_pPad != nullptr)
	//{
	//	m_pPad->Update();
	//}
}

//描画処理
void CApplication::Draw(void)
{
	//Rendererの描画処理
	if (m_pRenderer != nullptr)
	{//nullではなかったら
		m_pRenderer->Draw();
	}
}

//Rendererの取得処理
CRenderer* CApplication::GetRenderer(void)
{
	return m_pRenderer;
}

//ウインドウの取得処理
HWND CApplication::GetWindow(void)
{
	return m_hWnd;
}

//サウンドの取得処理
CSound* CApplication::GetSound(void)
{
	return m_pSound;
}

//カメラの取得処理
CCamera* CApplication::GetCamera(void)
{
	return m_pCamera;
}

//フェードの取得処理
const bool CApplication::GetFade(void)
{
	return m_bFade;
}

//フェードの設定処理
void CApplication::SetFade(const bool bFade)
{
	m_bFade = bFade;
}

//フェードの設定処理
void CApplication::SetFade(CApplication::Mode mode)
{
	if (m_pFade != nullptr)
	{
		m_pFade->SetFadeOut(mode);
	}
}

// モードの取得処理
CApplication::Mode CApplication::GetMode(void)
{
	return m_mode;
}

void CApplication::SetMode(Mode mode)
{
	if (m_pMode != nullptr)
	{
		m_pMode->Uninit();
		delete m_pMode;
		m_pMode = nullptr;
	}

	CObject::ReleaseAll();
	CMeshfield::ClearFields();
	CHitbox::ReleaseAll();

	if (m_pSound != nullptr)
	{
		m_pSound->Stop();
	}

	switch (mode)
	{

	case CApplication::MODE_TITLE:

	{
		m_pMode = CTitle::Create();
	}

	break;

	case CApplication::MODE_TUTORIAL:

	{
		m_pMode = CTutorial::Create();
	}

	break;

	case CApplication::MODE_FIRST_STAGE:

	{
		m_pMode = CFirstStage::Create();

		CSilhouette::Create();
	}

	break;

	//case CApplication::MODE_RESULTS:

	//{
	//	m_pMode = CResults::Create();
	//}

	//break;

	case CApplication::MODE_RANKING:

	{
		m_pMode = CRanking::Create();
	}

	break;

	default:

		break;
	}
}

//ゲームモードの取得処理
CGame* CApplication::GetGame(void)
{
	if (m_pMode)
	{
		if (m_pMode->IsGame())
		{
			CGame* pGame = dynamic_cast<CGame*>(m_pMode);
			return pGame;
		}
	}

	return nullptr;
}



//=====================================
// ポーズ取得処理
//=====================================
bool CApplication::GetPause()
{
	return m_bPause;
}

//=====================================
// ポーズ設定処理
//=====================================
void CApplication::SetPause(const bool bPause)
{
	m_bPause = bPause;
}