//=============================================================================
//
// テクスチャ処理 [textue.cpp]
// Author : 竹内亘
//
//=============================================================================
#include "texture.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9 CTexture::m_apTexture[MAX_TEXTURE] = {};
//LPDIRECT3DTEXTURE9 CTexture::m_pTexture = NULL;

const char * CTexture::m_apFileName[] =
{// テクスチャのファイル名
	FRAME_TEXTURE000,		// タイマーフレーム		(0)
	FRAME_TEXTURE001,		// スコアフレーム		(1)
	IMAGE_TEXTURE000,		// サルのカウント画像	(2)
	IMAGE_TEXTURE001,		// 残り画像				(3)
	IMAGE_TEXTURE002,		// コントローラーの説明	(4)
	IMAGE_TEXTURE003,		// 捕まえた数画像		(5)
	IMAGE_TEXTURE004,		// 残り画像				(6)
	IMAGE_TEXTURE005,		// 合計画像				(7)
	PARTICLE_TEXTURE_000,	// エフェクト			(8)
	PARTICLE_TEXTURE_001,	// エフェクト			(9)
	PAUSEMAT_TEXTURE,		// ポーズ台紙			(10)
	PAUSESELECT_TEXTURE000,	// CONTINUE				(11)
	PAUSESELECT_TEXTURE001,	// RETRY				(12)
	PAUSESELECT_TEXTURE002,	// QUIT					(13)
};
//=============================================================================
// コンストラクタ
//=============================================================================
CTexture::CTexture()
{

}
//=============================================================================
// デストラクタ
//=============================================================================
CTexture::~CTexture()
{

}
//=============================================================================
// テクスチャの読み込み
//=============================================================================
HRESULT CTexture::Load(void)
{
	CManager manager;	// インスタンス

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();


	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	{
		//// テクスチャの生成(動的確保ver)
		//D3DXCreateTextureFromFile(pDevice, m_apFileName[nCntTex],&m_pTexture);

		// テクスチャの生成(配列ver)
		D3DXCreateTextureFromFile(pDevice, m_apFileName[nCntTex], &m_apTexture[nCntTex]);
	}

	return S_OK;
}
//=============================================================================
// テクスチャの破棄
//=============================================================================
void CTexture::Unload(void)
{
	//// テクスチャの破棄
	//if (m_pTexture != NULL)
	//{
	//	m_pTexture->Release();
	//	m_pTexture = NULL;
	//}

	//// メモリの開放
	//delete m_pTexture;

	//// NULLにする
	//m_pTexture = NULL;


	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	{
		// テクスチャの破棄
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}
//=============================================================================
// テクスチャの取得
//=============================================================================
LPDIRECT3DTEXTURE9 CTexture::GetTexture(int nIdx)
{
	return m_apTexture[nIdx];
}
////=============================================================================
//// テクスチャの取得
////=============================================================================
//LPDIRECT3DTEXTURE9 CTexture::GetTexture(int nIdx)
//{
//	return m_pTexture;
//}
