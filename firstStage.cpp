//=============================================================================
//
// firstStage.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "firstStage.h"
#include "player.h"
#include "meshfield.h"
#include "inputKeyboard.h"
#include "application.h"
#include "bg.h"
#include "BoxHitbox.h"
#include "object3D.h"

#include "nail.h"
#include "spikeTrap.h"

//コンストラクタ
CFirstStage::CFirstStage()
{
	nCntEnd = 0;
}

//デストラクタ
CFirstStage::~CFirstStage()
{

}

//初期化処理
HRESULT CFirstStage::Init(void)
{
	//基本クラスの初期化処理
	if (FAILED(CGame::Init()))
	{
		return -1;
	}

	CPlayer* pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, -200.0f, -300.0f), 0);

	if (!pPlayer)
	{
		return -1;
	}
	nCntEnd = 0;

	SetPlayer(pPlayer);
	CMeshfield* pField = nullptr;
	pField = CMeshfield::Create(D3DXVECTOR3(-250.0f, -200.0f, 500.0f), Vec3Null, D3DXVECTOR2(50.0f, 50.0f), 11, 20);
	pField->SetTexture(CObject::TEXTURE_BLOCK);
	pField->SetTextureTiling(0.25f);

	pField = CMeshfield::Create(D3DXVECTOR3(-250.0f, -200.0f, 1500.0f), Vec3Null, "data\\MeshField\\SlopePitFall_Stage_01.txt", 3);
	pField->SetTexture(CObject::TEXTURE_BLOCK);
	pField->SetTextureTiling(0.25f);

	pField = CMeshfield::Create(D3DXVECTOR3(-250.0f, -200.0f, 2500.0f), Vec3Null, "data\\MeshField\\PitFall_Stage_01.txt", 3);
	pField->SetTexture(CObject::TEXTURE_BLOCK);
	pField->SetTextureTiling(0.25f);

	CBoxHitbox::Create(D3DXVECTOR3(0.0f, -410.0f, 1500.0f - 500.0f), Vec3Null, D3DXVECTOR3(150.0f, 270.0f, 150.0f), CHitbox::TYPE_FALL, nullptr, -1, CHitbox::EFFECT_FALL);

	CSpikeTrap::Create(D3DXVECTOR3(-175.0f, -199.9f, 1500.0f - 900.0f));
	CSpikeTrap::Create(D3DXVECTOR3(-100.0f, -199.9f, 1500.0f - 900.0f));
	CSpikeTrap::Create(D3DXVECTOR3(175.0f, -199.9f, 1500.0f - 900.0f));
	CSpikeTrap::Create(D3DXVECTOR3(100.0f, -199.9f, 1500.0f - 900.0f));

	CBoxHitbox::Create(D3DXVECTOR3(75.0f, -410.0f,  2500.0f - 375.0f), Vec3Null, D3DXVECTOR3(125.0f, 200.0f, 125.0f), CHitbox::TYPE_FALL, nullptr, -1, CHitbox::EFFECT_FALL);
	CBoxHitbox::Create(D3DXVECTOR3(-75.0f, -410.0f, 2500.0f - 800.0f), Vec3Null, D3DXVECTOR3(125.0f, 200.0f, 125.0f), CHitbox::TYPE_FALL, nullptr, -1, CHitbox::EFFECT_FALL);

	CNail::Create(D3DXVECTOR3(-200.0f, -199.9f, 2500.0f - 950.0f));
	CSpikeTrap::Create(D3DXVECTOR3(200.0f, -199.9f, 2500.0f - 550.0f), 2.0f, 60);

	CSpikeTrap::Create(D3DXVECTOR3(-200.0f, -199.9f, 2500.0f - 50.0f), 2.0f, 30);
	CSpikeTrap::Create(D3DXVECTOR3(-125.0f, -199.9f, 2500.0f - 50.0f), 2.0f, 30);
	CSpikeTrap::Create(D3DXVECTOR3(-50.0f, -199.9f,  2500.0f - 50.0f), 2.0f, 30);

	CBg* pBg = CBg::Create(D3DXVECTOR3(0.0f, 0.0f, 800.0f), 1700.0f);

	if (pBg)
	{
		pBg->SetBg(Vec3Null, ColorCyan);
	}

	CNail* pNail = nullptr;

	/*for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		pNail = CNail::Create(D3DXVECTOR3( (75.0f * nCnt), -199.0f, 400.0f));
		pNail = CNail::Create(D3DXVECTOR3(-(75.0f * nCnt), -199.0f, 400.0f));
	}*/


	return S_OK;
}

//終了処理
void CFirstStage::Uninit(void)
{
	//基本クラスの終了処理
	CGame::Uninit();
}

//更新処理
void CFirstStage::Update(void)
{
	//基本クラスの更新処理
	CGame::Update();

	if (GetPlayer()->GetFall())
	{
		nCntEnd++;

		if (nCntEnd >= 29)
		{
			CApplication::SetFade(CApplication::MODE_RESULTS);
		}
	}

#ifdef _DEBUG

	if (CInputKeyboard::GetKeyboardTrigger(DIK_O))
	{
		CApplication::SetFade(CApplication::MODE_RESULTS);
	}

#endif // DEBUG

}




//=============================================================================
//
//								静的関数
//
//=============================================================================


//生成処理
CFirstStage* CFirstStage::Create(void)
{
	CFirstStage* pStage = new CFirstStage;		//インスタンスを生成する

	if (FAILED(pStage->Init()))
	{//初期化処理
		return nullptr;
	}

	return pStage;								//生成したインスタンスを返す
}