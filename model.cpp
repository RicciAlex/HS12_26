//=============================================================================
//
// model.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "model.h"
#include "application.h"
#include "rendering.h"
#include "directionalLight.h"
#include "inputKeyboard.h"


//=============================================================================
//							静的変数の初期化
//=============================================================================

LPD3DXMESH		CModel::m_pMeshAll[MODEL_MAX] = {};					//メッシュ情報へのポインタ
LPD3DXBUFFER	CModel::m_pBuffMatAll[MODEL_MAX] = {};				//マテリアル情報へのポインタ
DWORD			CModel::m_nNumMatAll[MODEL_MAX] = {};				//マテリアル情報の数
std::vector <LPDIRECT3DTEXTURE9>     CModel::m_vModelTexture[CModel::MODEL_MAX];		//モデルのテクスチャへのポインタのベクトル

//モデルのXファイルの相対パス
char*			CModel::m_pModelPass[MODEL_MAX] =
{																				
	{ "data\\MODELS\\AstroBot\\00_body.x" },					//MODEL_PLAYER_BODY,
	{ "data\\MODELS\\AstroBot\\01_head.x" },					//MODEL_PLAYER_HEAD,

	{ "data\\MODELS\\AstroBot\\02_armR_long.x" },				//MODEL_PLAYER_FOREARM_RIGHT,
	{ "data\\MODELS\\AstroBot\\03_handR.x" },					//MODEL_PLAYER_ARM_RIGHT,

	{ "data\\MODELS\\AstroBot\\04_armL_long.x" },				//MODEL_PLAYER_FOREARM_LEFT,
	{ "data\\MODELS\\AstroBot\\05_handL.x" },					//MODEL_PLAYER_ARM_LEFT,

	{ "data\\MODELS\\AstroBot\\06_legR.x" },					//MODEL_PLAYER_LEG_RIGHT,
	{ "data\\MODELS\\AstroBot\\07_footR.x" },					//MODEL_PLAYER_FOOT_RIGHT,

	{ "data\\MODELS\\AstroBot\\08_legL.x" },					//MODEL_PLAYER_LEG_LEFT,
	{ "data\\MODELS\\AstroBot\\09_footL.x" },					//MODEL_PLAYER_FOOT_LEFT,

	{ "data\\MODELS\\Obstacle\\8InchNails.x" },					//MODEL_NAILS,
	{ "data\\MODELS\\Obstacle\\SpikeBase.x" },					//MODEL_SPIKE_BASE,
	{ "data\\MODELS\\Obstacle\\SpikeTrap.x" },					//MODEL_SPIKE_TRAP,

	
};

//コンストラクタ
CModel::CModel()
{
	m_pMesh = nullptr;								//メッシュ情報へのポインタ
	m_pBuffMat = nullptr;							//マテリアル情報へのポインタ
	m_nNumMat = 0;									//マテリアル情報の数
	m_pos = Vec3Null;								//現在の位置
	m_LastPos = Vec3Null;							//前回の位置
	m_move = Vec3Null;								//モデルの移動量
	m_rot = Vec3Null;								//向き
	m_fScale = 0.0f;								//拡大率
	m_frameRot = Vec3Null;							//1フレームの回転角度
	D3DXMatrixIdentity(&m_mtxWorld);				//ワールドマトリックス
	m_type = (CModel::ModelType)0;					//モデルの種類
	m_vCol.clear();									//マテリアルの色
	m_bShadow = true;								//影を描画するかどうか
	m_fShadowHeight = 0.0f;							//影の高さ
	D3DXMatrixIdentity(&m_mtxRot);					//回転マトリックス
	m_rotAxis = Vec3Null;							//回転軸
	m_fRotAngle = 0.0f;								//回転角
}

CModel::CModel(const int nPriority) : CObject::CObject(nPriority)
{
	m_pMesh = nullptr;								//メッシュ情報へのポインタ
	m_pBuffMat = nullptr;							//マテリアル情報へのポインタ
	m_nNumMat = 0;									//マテリアル情報の数
	m_pos = Vec3Null;								//現在の位置
	m_LastPos = Vec3Null;							//前回の位置
	m_move = Vec3Null;								//モデルの移動量
	m_rot = Vec3Null;								//向き
	m_fScale = 0.0f;								//拡大率
	m_frameRot = Vec3Null;							//1フレームの回転角度
	D3DXMatrixIdentity(&m_mtxWorld);				//ワールドマトリックス
	m_type = (CModel::ModelType)0;					//モデルの種類
	m_vCol.clear();									//マテリアルの色
	m_bShadow = true;								//影を描画するかどうか
	m_fShadowHeight = 0.0f;							//影の高さ
	D3DXMatrixIdentity(&m_mtxRot);					//回転マトリックス
	m_rotAxis = Vec3Null;							//回転軸
	m_fRotAngle = 0.0f;								//回転角
}

//デストラクタ
CModel::~CModel()
{

}

//初期化処理
HRESULT CModel::Init(void)
{
	m_pMesh = nullptr;								//メッシュ情報へのポインタ
	m_pBuffMat = nullptr;							//マテリアル情報へのポインタ
	m_nNumMat = 0;									//マテリアル情報の数
	m_pos = Vec3Null;								//現在の位置
	m_LastPos = Vec3Null;							//前回の位置
	m_move = Vec3Null;								//モデルの移動量
	m_rot = Vec3Null;								//向き
	m_fScale = 0.0f;								//拡大率
	m_frameRot = Vec3Null;							//1フレームの回転角度
	D3DXMatrixIdentity(&m_mtxWorld);				//ワールドマトリックス
	m_vCol.clear();									//マテリアルの色
	m_bShadow = true;								//影を描画するかどうか
	m_fShadowHeight = -149.9f;						//影の高さ
	D3DXMatrixIdentity(&m_mtxRot);					//回転マトリックス
	m_rotAxis = Vec3Null;							//回転軸
	m_fRotAngle = 0.0f;								//回転角

	return S_OK;
}

//終了処理
void CModel::Uninit(void)
{
	m_vCol.clear();				//マテリアルの色をクリアする
}

//更新処理
void CModel::Update(void)
{
	m_LastPos = m_pos;			//前回の位置の更新

	if (m_frameRot != nullptr)
	{//回転速度が0ではなかったら
		m_rot += m_frameRot;			//回転角度を更新する
	}
}

//描画処理
void CModel::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();				//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans, mtxShadow;			//計算用マトリックス
	D3DMATERIAL9 matDef;							//現在のマテリアル保存用
	D3DXMATERIAL *pMat;								//マテリアルデータへのポインタ
	D3DXVECTOR4 vecLight;							//ライトの向き
	D3DXVECTOR3 pos, Normal;						//投影用の位置と法線
	D3DXPLANE planeField;							//面
	D3DXQUATERNION quat;							//回転用のクオータニオン

		//ライトの向きを設定する
		D3DXVECTOR3 dir = CDirectionalLight::GetPrincipalLightDir();
		D3DXVec3Normalize(&dir, &dir);
		vecLight = D3DXVECTOR4(-dir.x, -dir.y, -dir.z, 0.0f);

		pos = D3DXVECTOR3(0.0f, m_fShadowHeight, 0.0f);				//面の高さ
		Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);						//面の法線

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);
		D3DXMatrixIdentity(&mtxShadow);

		//スケールを反映
		if (m_fScale != 0.0f)
		{
			D3DXMATRIX mtxScale;
			
			D3DXMatrixScaling(&mtxScale, m_fScale, m_fScale, m_fScale);
			D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);
		}

		////クオータニオンの初期化
		//D3DXQuaternionIdentity(&quat);

		////クオータニオンの作成
		//D3DXQuaternionRotationAxis(&quat, &m_rotAxis, m_fRotAngle);
		//D3DXMatrixRotationQuaternion(&mtxRot, &quat);

		////向きを反映
		//D3DXMatrixMultiply(&m_mtxRot, &m_mtxRot, &mtxRot);
		//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		if (m_bShadow)
		{//影の描画のフラグがtrueだったら

		//影の描画用のマトリックスを作る
		D3DXPlaneFromPointNormal(&planeField, &pos, &Normal);
		D3DXMatrixShadow(&mtxShadow, &vecLight, &planeField);

		D3DXMatrixMultiply(&mtxShadow, &m_mtxWorld, &mtxShadow);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

		//現在のマテリアルを保持
		pDevice->GetMaterial(&matDef);

		//マテリアルデータへのポインタの取得
		pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, NULL);

			D3DXCOLOR col = pMat[nCntMat].MatD3D.Diffuse;			//マテリアルの色を保存する

			//マテリアルの色を真っ黒にする
			pMat[nCntMat].MatD3D.Diffuse.r = 0.0f;
			pMat[nCntMat].MatD3D.Diffuse.g = 0.0f;
			pMat[nCntMat].MatD3D.Diffuse.b = 0.0f;
			pMat[nCntMat].MatD3D.Diffuse.a = 1.0f;

			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(0, m_vModelTexture[m_type].data()[nCntMat]);

			//モデルパーツの描画
			m_pMesh->DrawSubset(nCntMat);

			//マテリアルの色を元に戻す
			pMat[nCntMat].MatD3D.Diffuse.r = col.r;
			pMat[nCntMat].MatD3D.Diffuse.g = col.g;
			pMat[nCntMat].MatD3D.Diffuse.b = col.b;
			pMat[nCntMat].MatD3D.Diffuse.a = col.a;
		}


		//ステンシルバッファを有効にする
		pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

		//ステンシルバッファと比較する参照値設定
		pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);

		//ステンシルバッファの値に対してのマスク設定
		pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

		//ステンシルテストの比較方法の設定
		pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

		//ステンシルテストの結果に対しての反映設定
		pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);
		pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
		pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);

		pMat = nullptr;

		//保持しいたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}


	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//現在のマテリアルを保持
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタの取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, NULL);

		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, m_vModelTexture[m_type].data()[nCntMat]);

		//モデルパーツの描画
		m_pMesh->DrawSubset(nCntMat);
	}

	//保持しいたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	//ステンシルバッファを無効にする
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	pMat = nullptr;
	
	//保持しいたマテリアルを戻す
	pDevice->SetMaterial(&matDef);


	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//現在のマテリアルを保持
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタの取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, NULL);

		//マテリアルの色が設定されていたら、そのマテリアルの色を変えたら、描画して、元に戻す
		D3DXCOLOR c = {};
		D3DXCOLOR scaledCol = {};
		bool bCol = false;

		for (int i = 0; i < (int)m_vCol.size(); i++)
		{
			if (m_vCol.data()[i].nMatNumber == nCntMat)
			{
				bCol = true;
				c = pMat[nCntMat].MatD3D.Diffuse;
				pMat[nCntMat].MatD3D.Diffuse = m_vCol.data()[i].col;
				break;
			}
		}

		if (m_fScale != 0.0f)
		{
			scaledCol = pMat[nCntMat].MatD3D.Diffuse;
			pMat[nCntMat].MatD3D.Diffuse.r *= m_fScale;
			pMat[nCntMat].MatD3D.Diffuse.g *= m_fScale;
			pMat[nCntMat].MatD3D.Diffuse.b *= m_fScale;
		}

		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, m_vModelTexture[m_type].data()[nCntMat]);

		//モデルパーツの描画
		m_pMesh->DrawSubset(nCntMat);

		if (m_fScale != 0.0f)
		{
			pMat[nCntMat].MatD3D.Diffuse.r = scaledCol.r;
			pMat[nCntMat].MatD3D.Diffuse.g = scaledCol.g;
			pMat[nCntMat].MatD3D.Diffuse.b = scaledCol.b;
		}

		if (bCol)
		{
			pMat[nCntMat].MatD3D.Diffuse = c;
		}
	}

	//保持しいたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//位置の設定処理
void CModel::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//位置の取得処理
const D3DXVECTOR3 CModel::GetPos(void)
{
	return m_pos;
}

//モデルの向き設定処理
void CModel::SetRot(const D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//モデルの向きの取得処理
const D3DXVECTOR3 CModel::GetRot(void)
{
	return m_rot;
}

void CModel::SetScale(const float scale)
{
	m_fScale = scale;
}

//モデルの設定処理
void CModel::SetModel(const ModelType type)
{
	m_pMesh = m_pMeshAll[type];
	m_pBuffMat = m_pBuffMatAll[type];
	m_nNumMat = m_nNumMatAll[type];
	m_type = type;
}

//回転速度の設定処理
void CModel::StartRotation(const D3DXVECTOR3 frameRot)
{
	m_frameRot = frameRot;
}

//回転を止まる処理
void CModel::StopRotating(void)
{
	m_frameRot = Vec3Null;
}

//カーラーの設定処理
void CModel::SetModelColor(const int nNumMat, const D3DXCOLOR col)
{
	if (nNumMat >= (int)m_vCol.size())
	{
		ModelColor mCol = {};
		mCol.nMatNumber = nNumMat;
		mCol.col = col;

		m_vCol.push_back(mCol);
	}
	else
	{
		ModelColor mCol = {};
		mCol.nMatNumber = nNumMat;
		mCol.col = col;

		m_vCol.erase(m_vCol.begin() + nNumMat);
		m_vCol.emplace(m_vCol.begin() + nNumMat, mCol);
	}

	m_vCol.shrink_to_fit();
}

//影が描画されているかどうかの設定処理
void CModel::SetShadowDraw(const bool bDraw)
{
	m_bShadow = bDraw;
}

//影の高さの設定処理
void CModel::SetShadowHeight(const float fHeight)
{
	m_fShadowHeight = fHeight;
}

const float CModel::GetShadowHeight(void)
{
	return m_fShadowHeight;
}

const float CModel::GetScale(void)
{
	return m_fScale;
}

const bool CModel::GetShadowDraw(void)
{
	return m_bShadow;
}

D3DXMATRIX * CModel::GetWorldMatrix(void)
{
	return &m_mtxWorld;
}



//=============================================================================
//								静的関数
//=============================================================================




//モデル情報の取得処理
void CModel::GetModel(ModelType type, LPD3DXMESH* pMesh, LPD3DXBUFFER* pBuffMat, DWORD* numMat)
{
	*pMesh = m_pMeshAll[type];				//メッシュへのポインタの設定
	*pBuffMat = m_pBuffMatAll[type];		//マテリアルへのポインタの設定
	*numMat = m_nNumMatAll[type];			//マテリアル数の設定
}

//モデル全部の読み込み処理
void CModel::LoadAllModels(void)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();		//デバイスの取得

	for (int nCnt = 0; nCnt < CModel::MODEL_MAX; nCnt++)
	{
		m_vModelTexture[nCnt].clear();				//テクスチャをクリアする

		//Xファイルの読み込み
		D3DXLoadMeshFromX(m_pModelPass[nCnt],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&m_pBuffMatAll[nCnt],
			NULL,
			&m_nNumMatAll[nCnt],
			&m_pMeshAll[nCnt]);

		D3DXMATERIAL *pMat = nullptr;				//マテリア情報へのポインタ

		//マテリアルデータへのポインタの取得
		pMat = (D3DXMATERIAL*)m_pBuffMatAll[nCnt]->GetBufferPointer();

		for (int a = 0; a < (int)m_nNumMatAll[nCnt]; a++)
		{//マテリアのテクスチャが設定されていたら、生成して、保存する
			LPDIRECT3DTEXTURE9 pTex = nullptr;

			D3DXCreateTextureFromFile(pDevice, pMat->pTextureFilename, &pTex);
			pMat++;
			m_vModelTexture[nCnt].push_back(pTex);
		}
	}	
}

//モデル全部の破棄処理
void CModel::DestroyAllModels(void)
{
	for (int nCnt = 0; nCnt < CModel::MODEL_MAX; nCnt++)
	{
		//メッシュの破棄
		if (m_pMeshAll[nCnt] != nullptr)
		{
			m_pMeshAll[nCnt]->Release();
			m_pMeshAll[nCnt] = nullptr;
		}

		//マテリアルの破棄
		if (m_pBuffMatAll[nCnt] != nullptr)
		{
			m_pBuffMatAll[nCnt]->Release();
			m_pBuffMatAll[nCnt] = nullptr;
		}
	}
}

//テクスチャの取得処理
void CModel::GetTextures(std::vector <LPDIRECT3DTEXTURE9>& vTex, CModel::ModelType type)
{
	vTex.clear();

	for (int nCnt = 0; nCnt < (int)m_nNumMatAll[type]; nCnt++)
	{
		vTex.push_back(m_vModelTexture[type].data()[nCnt]);
	}
}

//生成処理
CModel* CModel::Create(ModelType type, D3DXVECTOR3 pos)
{
	CModel* pModel = new CModel(3);						//モデルを生成する

	if (FAILED(pModel->Init()))
	{//初期化処理
		return nullptr;
	}

	pModel->m_pos = pos;								//位置の設定
	pModel->m_LastPos = pos;							//前回の位置の設定
	pModel->m_move = Vec3Null;							//移動量の設定
	pModel->m_rot = Vec3Null;							//向きの設定
	pModel->m_pMesh = m_pMeshAll[type];					//メッシュへのポインタの取得
	pModel->m_pBuffMat = m_pBuffMatAll[type];			//マテリアルへのポインタの取得
	pModel->m_nNumMat = m_nNumMatAll[type];				//マテリアル数の取得
	pModel->m_type = type;								//種類の設定
	pModel->m_fShadowHeight = pos.y + 0.1f;					//影の位置のY座標の設定

	return pModel;										//生成したインスタンスを返す
}

CModel* CModel::Create(ModelType type, D3DXVECTOR3 pos, const int nPriority)
{
	CModel* pModel = new CModel(nPriority);				//モデルを生成する

	if (FAILED(pModel->Init()))
	{//初期化処理
		return nullptr;
	}

	pModel->m_pos = pos;								//位置の設定
	pModel->m_LastPos = pos;							//前回の位置の設定
	pModel->m_move = Vec3Null;							//移動量の設定
	pModel->m_rot = Vec3Null;							//向きの設定
	pModel->m_pMesh = m_pMeshAll[type];					//メッシュへのポインタの取得
	pModel->m_pBuffMat = m_pBuffMatAll[type];			//マテリアルへのポインタの取得
	pModel->m_nNumMat = m_nNumMatAll[type];				//マテリアル数の取得
	pModel->m_type = type;								//種類の設定
	pModel->m_fShadowHeight = pos.y;					//影の位置のY座標の設定

	return pModel;										//生成したインスタンスを返す
}