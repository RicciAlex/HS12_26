//=============================================================================
//
// nail.h
// Author : Ricci Alex
//
//=============================================================================

#ifndef _NAIL_H_
#define _NAIL_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "model.h"

//=============================================================================
//前方宣言
//=============================================================================
class CBoxHitbox;


class CNail : public CModel
{
public:
	CNail();											//コンストラクタ
	~CNail() override;									//デストラクタ
														
	HRESULT Init(void) override;						//初期化処理
	void Uninit(void) override;							//終了処理
	void Update(void) override;							//更新処理
	void Draw(void) override;							//描画処理
														
	static CNail* Create(const D3DXVECTOR3 pos);		//生成処理

private:

	CBoxHitbox* m_pHitbox;								//ヒットボックス

};





#endif