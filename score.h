//=============================================================================
//
// スコア処理 [score.h]
// Author : KADO TAKUMA
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"
#include "object.h"

//=============================================================================
// 前方定義
//=============================================================================
class CNumber;

//=============================================================================
// マクロ定義
//=============================================================================
#define SCORE_DIGIT	(5)		//使用する桁数

//=============================================================================
// クラスの定義
//=============================================================================
class CScore : public CObject
{
public:
	//-------------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//-------------------------------------------------------------------------
	CScore();
	~CScore() override;

	//-------------------------------------------------------------------------
	// メンバー関数
	//-------------------------------------------------------------------------
	HRESULT Init() override;																				// 初期化処理
	void Uninit() override;																					// 終了処理						
	void Update() override;																					// 更新処理
	void Draw() override;																					// 描画処理
	static CScore *Create(bool bCalcDigit, int nScore, int nDigit, D3DXVECTOR3 pos, D3DXVECTOR2 size);		// 生成処理

	void SetScore(int nScore);										// スコアの設定処理
	void AddScore(int nValue);										// スコアの加算処理
	void SubScore(int nValue);										// スコアの減算処理
	void AddSaveScore(int nValue);									// 保存スコアの加算処理
	int GetScore(void) { return m_nScore; };						// スコアのゲッター
	void CalcDigit(int nScore);										// 桁数を計算する処理

	//void VtxUpdate() override {};									// 頂点座標更新処理
	void SetPos(const D3DXVECTOR3 pos) override;					// 座標設定処理
	void SetSize(D3DXVECTOR2 size)  { m_size = size; };		// サイズ設定処理
	//void SetMove(D3DXVECTOR3 move) override { D3DXVECTOR3 i = move; };	// 移動量設定処理
	//void SetCol(D3DXCOLOR col) override { D3DXCOLOR i = col; };			// 色設定処理
	//void SetRot(D3DXVECTOR3 rot) override { D3DXVECTOR3 i = rot; };	// 向き設定処理
	void SetDigit(int nDigit) { m_nDigit = nDigit; }				// 桁数設定処理
	void SetCalcDigit(bool bCalcDigit) { m_bCalcDigit = bCalcDigit; }

	const D3DXVECTOR3 GetPos(void) override { return m_pos; }				// 座標取得処理
	D3DXVECTOR2 GetSize(void)  { return m_size; }			// サイズ取得処理
	//D3DXVECTOR3 GetMove(void) override { return D3DXVECTOR3(); }	// 移動量取得処理
	//D3DXVECTOR3 GetRot(void) override { return D3DXVECTOR3(); }		// 向き取得処理
	//D3DXCOLOR GetCol(void) override { return D3DXCOLOR(); }			// 色取得処理

private:
	//-------------------------------------------------------------------------
	// メンバー変数
	//-------------------------------------------------------------------------
	CNumber *m_apNumber[SCORE_DIGIT];			// 表示する桁数のポインタ
	D3DXVECTOR3 m_pos;							// 座標
	D3DXVECTOR2 m_size;							// サイズ
	int  m_nScore;								// 情報格納変数
	int  m_nDigit;								// 使用する桁数
	bool m_bCalcDigit;							// 桁数を変更するか否か
	int m_nSaveScore;
};

#endif
