//=============================================================================
//
// game.h
// Author : Ricci Alex
//
//=============================================================================
#ifndef _GAME_H
#define _GAME_H

//=============================================================================
//インクルードファイル
//=============================================================================
#include "mode.h"

//=============================================================================
//前方宣言
//=============================================================================
class CPlayer;
class CScore;


class CGame : public CMode
{
public:

	//ステージ
	enum STAGE
	{
		FIRST_STAGE = 0,

		STAGE_MAX
	};

	CGame();						//コンストラクタ
	~CGame() override;				//デストラクタ

	HRESULT Init(void);				//初期化処理
	void Uninit(void);				//終了処理
	void Update(void);				//更新処理

	void SetPlayer(CPlayer* pPlayer);		//プレイヤーの設定処理
	CPlayer* GetPlayer(void);				//プレイヤーの取得処理
	static CScore* GetScore() { return m_pScore; }

private:

	CPlayer* m_pPlayer;
	static CScore* m_pScore;

};



#endif