//=============================================================================
//
// オブジェクト処理 [object.cpp]
// Author : 竹内亘
//
//=============================================================================
#include "shadowX.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"

#include "shadow.h"
#include "player.h"
#include "enemy.h"
#include "game.h"
#include "tutorial.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9 *CShadowX::m_pTexture = NULL;
LPD3DXMESH CShadowX::m_pMesh = NULL;
LPD3DXBUFFER CShadowX::m_pBuffMat = NULL;
DWORD CShadowX::m_nNumMat = NULL;
CPlayer * CShadowX::m_pPlayer = NULL;
CEnemy * CShadowX::m_pEnemy = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CShadowX::CShadowX():CSceneX(4)
{
	m_pVtxBuff = NULL;

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
//=============================================================================
// デストラクタ
//=============================================================================
CShadowX::~CShadowX()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CShadowX::Init(D3DXVECTOR3 pos)
{
	CSceneX::BindModelTex(m_pTexture, m_pMesh, m_pBuffMat, m_nNumMat);
	CSceneX::Init(pos);

	CManager manager;	// インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// オブジェクトのタイプの設定
	SetObjType(CScene::OBJTYPE_OBJECT);

	// モデル初期化
	m_pos = pos;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報を設定
	VERTEX_2D * pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 位置
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 色
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	// 頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CShadowX::Uninit(void)
{
	if (m_pTexture != NULL)
	{// (家)
		for (int nCnt = 0; nCnt < (int)m_nNumMat; nCnt++)
		{// (家具)
			if (m_pTexture[nCnt] != NULL)
			{// テクスチャ破棄
				m_pTexture[nCnt]->Release();
				m_pTexture[nCnt] = NULL;
			}
		}
	}

	// メモリの開放(解体)
	delete[] m_pTexture;
	// NULLを入れる(更地)
	m_pTexture = NULL;

	//CSceneX::Uninit();

	// オブジェクト(自分自身)の破棄
	Release();

}
//=============================================================================
// 更新処理
//=============================================================================
void CShadowX::Update(void)
{
	// モードの取得
	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_GAME)
	{// モードがゲーム
		m_pPlayer = CGame::GetPlayer();

		if (m_pPlayer != NULL)
		{
			D3DXVECTOR3 PlayerPos = m_pPlayer->GetPos();
			CSceneX::SetPosition(PlayerPos);
		}
	}

	if (mode == CManager::MODE_TUTORIAL)
	{//　モードがチュートリアル
		m_pPlayer = CTutorial::GetPlayer();

		if (m_pPlayer != NULL)
		{
			D3DXVECTOR3 PlayerPos = m_pPlayer->GetPos();
			CSceneX::SetPosition(PlayerPos);
		}
	}

	//CDebugProc::Print(1, "モデル : %.1f,%.1f,%.1f\n", m_pos.x, m_pos.y, m_pos.z);

	//// デバッグ表示
	//CDebugProc::Print(1, "ブロックのMAX : %.1f,%.1f,%.1f\n",m_VtxMax.x, m_VtxMax.y, m_VtxMax.z);
	//CDebugProc::Print(1, "ブロックのMIN : %.1f,%.1f,%.1f\n", m_VtxMin.x, m_VtxMin.y, m_VtxMin.z);
}
//=============================================================================
// 描画処理
//=============================================================================
void CShadowX::Draw(void)
{
	CManager manager;	// インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// ステンシルテストを有効
	pDevice->SetRenderState(D3DRS_STENCILENABLE,TRUE);

	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x00000000);

	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);			// ステンシルバッファに書き込む

	pDevice->SetRenderState(D3DRS_STENCILPASS,D3DSTENCILOP_ZERO);		// ステンシルテストとZテストが合格
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCR);		// ステンシルテスト合格とZテストが不合格
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_ZERO);		// ステンシルテスト不合格

//#ifdef _DEBUG
//	// キーボードの取得
//	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
//	// カリング
//	if (pInputKeyboard->GetPress(DIK_C) == true)
//	{// F1を押したらON
//		pDevice->SetRenderState(D3DRS_FILLMODE, 2);
//	}
//#endif

	// 表面のカリング
	pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CW);

	// 影のモデル描画
	CSceneX::Draw();

	pDevice->SetRenderState(D3DRS_STENCILREF, 1);						// ステンシル参照値
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);			// ステンシルバッファに書き込む

	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);		// ステンシルテストとZテストが合格
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// ステンシルテスト合格とZテストが不合格
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// ステンシルテスト不合格

	// 裏面のカリング
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// 影のモデル描画
	CSceneX::Draw();

	// 元に戻す
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0000000f);

	pDevice->SetRenderState(D3DRS_STENCILREF, 2);						// ステンシル参照値
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);			// ステンシルバッファに書き込む

	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);		// ステンシルテストとZテストが合格
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// ステンシルテスト合格とZテストが不合格
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// ステンシルテスト不合格

	// 画面を黒く塗りつぶす
	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// ステンシルテストを無効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}
//=============================================================================
// 生成
//=============================================================================
CShadowX * CShadowX::Create(D3DXVECTOR3 pos)
{
	CShadowX *pObject = NULL;

	if (pObject == NULL)
	{
		pObject = new CShadowX;

		pObject->Init(pos);
	}
	return pObject;
}
//=============================================================================
// モデルの読み込み
//=============================================================================
HRESULT CShadowX::LoadX(void)
{
	D3DXMATERIAL *pMat;							// マテリアルデータへのポインタ

	CManager manager;	// インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// Xファイルの読み込み
	D3DXLoadMeshFromX(SHADOW_MODEL_NAME000,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	m_pTexture = new LPDIRECT3DTEXTURE9[m_nNumMat];

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		m_pTexture[nCntMat] = NULL;

		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			// テクスチャの生成
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &m_pTexture[nCntMat]);
		}
	}

	return S_OK;
}
//=============================================================================
// モデルの破棄
//=============================================================================
void CShadowX::UnloadX(void)
{
	// メッシュの開放
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	// マテリアルの開放
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}
}