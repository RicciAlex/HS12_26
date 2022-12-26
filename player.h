//=============================================================================
//
// player.h
// Author : Ricci Alex
//
//=============================================================================

#ifndef _PLAYER_H_
#define _PLAYER_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "object.h"

//=============================================================================
//前方宣言
//=============================================================================
class CModelPart;
class CAnimator;
class CCylinderHitbox;

class CPlayer : public CObject
{
public:

	static const D3DXVECTOR3 m_playerSize;								//プレイヤーのサイズ

	enum Parts
	{
		BODY = 0,								//体
		HEAD,									//頭
		LEFT_ARM,								//左腕
		LEFT_HAND,								//左手
		RIGHT_ARM,								//右腕
		RIGHT_HAND,								//右手
		LEFT_LEG,								//左足
		LEFT_FOOT,								//左太腿
		RIGHT_LEG,								//右足
		RIGHT_FOOT,								//右太腿
		SAYA,									//鞘
		KATANA,									//刀
		PARTS_MAX
	};

	enum STATE
	{
		STATE_NEUTRAL = 0,						//ニュートラル
		STATE_RUNNING,							//走る
		STATE_JUMP_START,
		STATE_JUMP,								//ジャンプ
		STATE_ATTACK,							//攻撃
		STATE_DAMAGE,							//ダメージ
		STATE_MAX								
	};

	enum PlayerColor
	{
		PLAYER_RED = 0,							//赤
		PLAYER_GREEN,							//緑
		PLAYER_BLUE,							//青
		PLAYER_YELLOW,							//黄
		PLAYER_MAGENTA,							//マゼンタ
		PLAYER_CYAN,							//シアン
		PLAYER_BLACK,							//黒
		PLAYER_WHITE,							//白

		PLAYER_COLOR_MAX
	};

	CPlayer();															//コンストラクタ
	~CPlayer() override;												//デストラクタ

	HRESULT Init(void) override;										//初期化処理
	void Uninit(void) override;											//終了処理
	void Update(void) override;											//更新処理
	void Draw(void) override;											//描画処理

	void SetPos(const D3DXVECTOR3 pos) override;						//位置の設定処理
	void SetRot(const D3DXVECTOR3 rot);									//向きの設定処理
	void SetLanded(void);												//着地しているかどうかの設定処理
	void SetMove(const D3DXVECTOR3 move);								//速度の設定処理

	const D3DXVECTOR3 GetPos(void) override;							//位置の取得処理
	const D3DXVECTOR3 GetLastPos(void);									//前回の位置の取得処理
	const D3DXVECTOR3 GetMove(void);									//速度の取得処理
	const bool		  GetFall(void);									//倒したかどうかの取得処理

	static CPlayer* Create(const D3DXVECTOR3 pos,int nCntPlayer);		//生成処理
	static D3DXCOLOR* GetPlayerColors(void);							//プレイヤーの色の取得処理

	void PlayerController(void);										//プレイヤーのキー処理
	void SetFriction(const float fFriction);							//摩擦係数の設定処理

private:

	static const int   m_nTimeRange = 180;								//バランスが変わるまでの最大時間
	static const float m_fMouseSafeRadius;								
	static const float m_fMouseSensibilityCoefficient;		
	static const float m_fFallLimit;
	static const int   m_nFrameBalanceRange = 5;

	void RespawnPlayer(void);											//リスポーン処理
	void HitboxEffectUpdate(void);										//当たった後の反応処理
	void ControlBalance(void);											//バランスの処理

	static D3DXCOLOR m_playerColor[PLAYER_COLOR_MAX];					//プレイヤーの色
	static const float m_MaxWalkingSpeed;								//最大の歩くスピード
	static const float m_AccelerationCoeff;								//加速係数

	D3DXVECTOR3 m_pos;													//位置
	D3DXVECTOR3 m_LastPos;												//前回の位置
	D3DXVECTOR3 m_move;													//速度
	D3DXVECTOR3 m_rot;													//向き
	D3DXVECTOR3 m_DestRot;												//目的の角度
	D3DXMATRIX  m_mtxWorld;												//ワールドマトリックス
	int			m_nInvincibilityCnt;									//無敵状態のカウンター
	int			m_nCntBalance;											//バランスカウンター
	int			m_nBalanceChangeTime;									//バランスが変わるフレーム
	float		m_fFrictionCoeff;										//摩擦係数
	float		m_fBalance;												//バランス変数
	float		m_fFrameBalance;										//毎フレーム加算されているバランスの値
	bool		m_bJump;												//ジャンプしているかどうか
	bool		m_bLanded;												//着地しているかどうか
	bool		m_bMoving;												//移動しているかどうか
	bool		m_bRot;													//回転したかどうか
	bool		m_bHit;													//当たったかどうか
	bool		m_bFall;												//落下しているかどうか

	STATE m_State;														//プレイヤーの状態

	CModelPart* m_pModel[PARTS_MAX];									//モデルへのポインタ
	CAnimator* m_pAnimator;												//アニメーターへのポインタ
	CCylinderHitbox* m_pHitbox;											//ヒットボックス
};

#endif