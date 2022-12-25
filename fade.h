//=============================================================================
//
// fade.h
// Author : Ricci Alex
//
//=============================================================================
#ifndef FADE_H
#define FADE_H

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"
#include "application.h"

//=============================================================================
//前方宣言
//=============================================================================
class CObject_2D;

class CFade
{
public:

	enum FADE
	{
		FADE_NONE = 0,			//フェードしていない
		FADE_IN,				//フェードイン
		FADE_OUT,				//フェードアウト
		FADE_MAX
	};

	CFade();										//コンストラクタ
	~CFade();										//デストラクタ

	HRESULT Init(void);								//初期化処理
	void Uninit(void);								//終了処理
	void Update(void);								//更新処理
	void Draw(void);								//描画処理

	void SetFadeIn(void);							//フェードインの設定処理
	void SetFadeOut(CApplication::Mode mode);		//フェードアウトの設定処理


	static CFade* Create(void);											//生成処理
	static CFade* Create(const float fFadeSpeed);						//生成処理
	static CFade* Create(const D3DXCOLOR fadeColor);						//生成処理
	static CFade* Create(const D3DXCOLOR fadeColor, const float fFadeSpeed);						//生成処理

private:

	static const float m_fBaseFadeSpeed;			//基本のフェードスピード

	CObject_2D* m_pFade;							//フェード用のポリゴン
	FADE m_fade;									//フェード
	CApplication::Mode m_modeNext;					//次のモード
	float m_fFadeSpeed;								//フェードスピード
	D3DXCOLOR m_fadeColor;							//フェードカラー

};

#endif