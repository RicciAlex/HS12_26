//=============================================================================
//
// bg.h
// Author : Ricci Alex
//
//=============================================================================
#ifndef BG_H
#define BG_H

//=============================================================================
//インクルードファイル
//=============================================================================
#include "object.h"

//=============================================================================
//前方宣言
//=============================================================================
class CBillboard;

class CBg : public CObject
{
public:
	CBg();
	~CBg() override;

	virtual HRESULT Init(void);						//初期化処理
	virtual void Uninit(void);						//終了処理
	virtual void Update(void);						//更新処理
	virtual void Draw(void);						//描画処理

	virtual void SetPos(const D3DXVECTOR3 pos);		//位置の設定処理

	virtual const D3DXVECTOR3 GetPos(void);			//位置の取得処理

	void SetBg(const D3DXVECTOR3 relativePos, const D3DXCOLOR col);				//背景の追加処理
	void SetBg(const D3DXVECTOR3 relativePos, const D3DXCOLOR col, CObject::TextType type);				//背景の追加処理

	static CBg* Create(const D3DXVECTOR3 pos, const float fDistance);			//生成処理

private:

	static const int MaxBgLayers = 3;

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_relativePos[MaxBgLayers];

	float m_fFar;

	CBillboard* m_pBg[MaxBgLayers];

};




#endif