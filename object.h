//=============================================================================
//
// object.h
// Author : Ricci Alex
//
//=============================================================================
#ifndef OBJECT_H
#define OBJECT_H

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
//前方宣言
//=============================================================================
class CPlayer;

//=============================================================================
// オブジェクトクラス
//=============================================================================
class CObject
{
public:

	// 頂点データ
	struct VERTEX_2D
	{
		D3DXVECTOR3 pos;			//xyz座標
		float rhw;					//rhw
		D3DCOLOR col;				//カーラー
		D3DXVECTOR2 tex;			//テクスチャ座標
	};

	struct VERTEX_3D
	{
		D3DXVECTOR3 pos;			//xyz座標
		D3DXVECTOR3 nor;			//法線
		D3DCOLOR col;				//カーラー
		D3DXVECTOR2 tex;			//テクスチャ座標
	};

	struct VERTEX_LINE
	{
		D3DXVECTOR3 pos;			//xyz座標
		D3DCOLOR col;				//カーラー
	};

	//テクスチャの種類
	enum TextType
	{
		TEXTURE_NULL = 0,

		TEXTURE_CHARACTERS,
		TEXTURE_BLOCK,
		TEXTURE_TITLE,
		TEXTURE_NUMBERS,
		TEXTURE_RANKING,

		TEXTURE_TYPE_MAX
	};

	static const int MaxObject = 5000;					//オブジェクトの最大数

	CObject();											//コンストラクタ
	CObject(int nPriority);								//コンストラクタ (1 <= priority <= 5)
	virtual~CObject();									//デストラクタ
														
	virtual HRESULT Init(void) = 0;						//初期化処理
	virtual void Uninit(void) = 0;						//終了処理
	virtual void Update(void) = 0;						//更新処理
	virtual void Draw(void) = 0;						//描画処理
														
	virtual void SetPos(const D3DXVECTOR3 pos) = 0;		//位置の設定処理
	
	void Release(void);									//インスタンス一つの終了処理
	//virtual const D3DXVECTOR2 GetSize(void) = 0;		//サイズの取得処理
	virtual const D3DXVECTOR3 GetPos(void) = 0;			//位置の取得処理
	void SetPriority(int nPriority);					//プライオリティの設定処理
														
	//静的関数
	static void ReleaseAll(void);						//全部の終了処理
	static void UpdateAll(void);						//全部の更新処理
	static void DrawAll(void);							//全部の描画処理
	static CObject** GetObj(void);						//オブジェクトの取得処理

	static int random(const int low, const int high);				//乱数を返す処理

private:								

	static const int Max_Priority = 5;					//プライオリティの最大値
	int m_nIdx;											//このインスタンスの配列のインデックス
	int m_nPriority;									//描画順
	bool m_bDeath;										//死亡フラグ

	static CObject* m_pTop[Max_Priority];				//先頭のオブジェクトへのポイン
	static CObject* m_pCurrent[Max_Priority];			//現在(一番後ろ)のオブジェクトへのポンタ
	CObject* m_pPrev;									//前のオブジェクトへのポンタ
	CObject* m_pNext;									//次のオブジェクトへのポンタ
};

#endif