//=============================================================================
//
// model.h
// Author : Ricci Alex
//
//=============================================================================
#ifndef _MODEL_H
#define _MODEL_H

//=============================================================================
//インクルードファイル
//=============================================================================
#include "object.h"
#include <vector>

class CModel : public CObject
{
public:

	//マテリアルの色
	struct ModelColor
	{
		int nMatNumber;
		D3DXCOLOR col;
	};

	//モデルの種類
	enum ModelType
	{
		MODEL_PLAYER_HEAD,
		MODEL_PLAYER_BODY,
		MODEL_PLAYER_FOREARM,
		MODEL_PLAYER_ARM,
		MODEL_PLAYER_LEG,
		MODEL_PLAYER_FOOT,
		MODEL_SAYA,
		MODEL_KATANA,

		MODEL_NAILS,
		MODEL_SPIKE_BASE,
		MODEL_SPIKE_TRAP,

		MODEL_MAX
	};

	CModel();																			//コンストラクタ
	CModel(const int nPriority);														//コンストラクタ
	virtual ~CModel() override;															//デストラクタ
													
	virtual HRESULT Init(void) override;												//初期化処理
	virtual void Uninit(void) override;													//終了処理
	virtual void Update(void) override;													//更新処理
	virtual void Draw(void) override;													//描画処理

	void SetPos(const D3DXVECTOR3 pos) override;										//位置の設定処理
	const D3DXVECTOR3 GetPos(void) override;											//位置の取得処理

	void SetRot(const D3DXVECTOR3 ros);													//向きの設定処理
	const D3DXVECTOR3 GetRot(void);														//向きの取得処理

	void SetScale(const float scale);													//拡大率の設定処理

	void SetModel(const ModelType type);												//モデルの設定処理

	void StartRotation(const D3DXVECTOR3 frameRot);										//回転開始処理
	void StopRotating(void);															//回転停止処理
																						
	void SetModelColor(const int nNumMat, const D3DXCOLOR col);							//モデルの色の設定処理
	void SetShadowDraw(const bool bDraw);												//影の描画フラグの設定処理
	void SetShadowHeight(const float fHeight);											//影の高さの設定処理

	const float GetShadowHeight(void);													//影の高さの取得処理
	const float GetScale(void);															//拡大率の取得処理
	const bool GetShadowDraw(void);														//影を描画するかどうかの取得処理
	D3DXMATRIX* GetWorldMatrix(void);													//ワールドマトリックスの取得処理

	static void GetModel(ModelType type, LPD3DXMESH* pMesh, LPD3DXBUFFER* pBuffMat, DWORD* numMat);		//モデルのメッシュ情報の取得処理
	static void GetTextures(std::vector <LPDIRECT3DTEXTURE9>& vTex, CModel::ModelType type);			//マテリアルのテクスチャの取得処理
	static void LoadAllModels(void);													//全部のモデルのロード処理
	static void DestroyAllModels(void);													//全部のモデルの破棄処理

	static CModel* Create(ModelType type, D3DXVECTOR3 pos);								//生成処理
	static CModel* Create(ModelType type, D3DXVECTOR3 pos, const int nPriority);		//生成処理

private:

	LPD3DXMESH		m_pMesh;												//メッシュ情報へのポインタ
	LPD3DXBUFFER	m_pBuffMat;												//マテリアル情報へのポインタ
	DWORD			m_nNumMat;												//マテリアル情報の数
	D3DXVECTOR3		m_pos;													//現在の位置
	D3DXVECTOR3		m_LastPos;												//前回の位置
	D3DXVECTOR3		m_move;													//モデルの移動量
	D3DXVECTOR3		m_rot;													//向き
	D3DXMATRIX		m_mtxRot;												//回転マトリックス
	D3DXVECTOR3		m_rotAxis;												//回転軸
	float			m_fRotAngle;											//回転角
	float			m_fScale;												//拡大率
	D3DXVECTOR3		m_frameRot;												//1フレームの回転角度
	D3DXMATRIX		m_mtxWorld;												//ワールドマトリックス
	ModelType		m_type;													//モデルの種類
	std::vector <ModelColor> m_vCol;										//モデルの色
	bool			m_bShadow;												//影があるかどうか
	float			m_fShadowHeight;										//影の高さ

	static LPD3DXMESH		m_pMeshAll[MODEL_MAX];							//メッシュ情報へのポインタ
	static LPD3DXBUFFER		m_pBuffMatAll[MODEL_MAX];						//マテリアル情報へのポインタ
	static DWORD			m_nNumMatAll[MODEL_MAX];						//マテリアル情報の数
	static char*			m_pModelPass[MODEL_MAX];						//モデルの相対パス
	static std::vector <LPDIRECT3DTEXTURE9> m_vModelTexture[MODEL_MAX];		//モデルのテクスチャへのポインタのベクトル
};

#endif