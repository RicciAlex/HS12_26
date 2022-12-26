//=============================================================================
//
// application.h
// Author : Ricci Alex
//
//=============================================================================

#ifndef APPLICATION_H
#define APPLICATION_H

//=============================================================================
//インクルードファイル
//=============================================================================
#include "sound.h"

//前方宣言
class CRenderer;
class CInputKeyboard;
class CSound;
class CCamera;
class CDebugProc;
class CMode;
class CFade;
class CGame;
//class CMenu;
//class CMessage;

//アプリケーションのクラス
class CApplication
{
public:
	enum Mode
	{
		MODE_TITLE = 0,
		MODE_TUTORIAL,
		MODE_FIRST_STAGE,
		//MODE_RESULTS,
		MODE_RANKING,

		MODE_MAX
	};

	enum FADE
	{
		FADE_NONE = 0,	// 何もしていない状態
		FADE_IN,		// フェードイン(徐々に透明にする状態)
		FADE_OUT,		// フェードアウト(徐々に不透明にする状態)

		FADE_MAX
	};

	CApplication();										//コンストラクタ
	~CApplication();									//デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);		//初期化処理
	void Uninit(void);									//終了処理
	void Update(void);									//更新処理
	void Draw(void);									//描画処理
	static CRenderer* GetRenderer(void);				//レンディングインスタンスの取得処理
	static HWND GetWindow(void);						//ウインドウの取得処理
	static CSound* GetSound(void);						//サウンドの取得処理
	static CCamera* GetCamera(void);					//カメラの取得処理
	static const bool GetFade(void);					//フェードの取得処理
	static void SetFade(const bool bFade);				//フェードの設定処理
	static void SetFade(Mode mode);						// モードの設定処理

	static Mode GetMode(void);							// モードの取得処理
	static void SetMode(Mode mode);						//モードの設定処理
	static CGame* GetGame(void);						//ゲームモードの取得処理

	static bool GetPause(void);							// ポーズの取得処理
	static void SetPause(const bool bPause);			// ポーズの設定処理


private:



	static int m_nStageSelect;							//選択中のステージセレクト
	static bool m_bPause;								//ポーズ中であるかどうか
	static bool m_bFade;

	static HWND m_hWnd;									//クライエント画面
	static CRenderer* m_pRenderer;						//レンディングインスタンスへのポインタ
	static CInputKeyboard* m_pKeyboard;					//インプットデバイスへのポインタ
	static CSound* m_pSound;							//サウンドのインスタンスへのポインタ
	static CFade* m_pFade;								//フェードのインスタンスへのポインタ
	static CCamera* m_pCamera;							//カメラのインスタンスへのポインタ
	static CMode* m_pMode;								//モードのインスタンスへのポインタ
	static CDebugProc* m_pDebug;						//デバッグテキストのインスタンス

	static Mode m_mode;		//現在モード
	static Mode m_modeNext;	//次のモード
};

#endif // !APPLICATION_H
