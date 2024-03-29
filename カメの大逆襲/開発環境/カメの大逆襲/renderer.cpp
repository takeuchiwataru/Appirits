//=============================================================================
//
// レンダリング処理 [renderer.cpp]
// Author : 竹内亘
//
//=============================================================================
#include "renderer.h"
#include "scene.h"
#include "fade.h"
#include "pause.h"
#include "input.h"
#include "sound.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
CPause *CRenderer::m_pPause = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CRenderer::CRenderer()
{
	m_pD3D = NULL;			// Direct3Dオブジェクト
	m_pD3DDevice = NULL;	// Deviceオブジェクト(描画に必要)

#ifdef _DEBUG
	m_pFont = NULL;			// フォントへのポインタ
#endif
}
//=============================================================================
// デストラクタ
//=============================================================================
CRenderer::~CRenderer()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CRenderer:: Init(HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;							// バックバッファの数
	d3dpp.BackBufferWidth = SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;				// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;				// カラーモードの指定
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;						// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &m_pD3DDevice)))
			{
				// 生成失敗
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

#ifdef _DEBUG
	// デバッグ情報表示用フォントの生成
	D3DXCreateFont(m_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);
#endif

	m_bPause = false;

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CRenderer::Uninit(void)
{
#ifdef _DEBUG
	// デバッグ情報表示用フォントの破棄
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
#endif

	// デバイスの破棄
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}

	// ポーズの破棄
	if (m_pPause != NULL)
	{
		m_pPause->Uninit();

		delete m_pPause;
		m_pPause = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void CRenderer::Update(void)
{
	// キーボードの取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// ジョイパッド(XInput)の取得
	CXInputJoyPad *pXInputJoyPad = CManager::GetXInputJoyPad();

	// サウンドの取得
	CSound * pSound = CManager::GetSound();

	// フェードの取得
	CFade *pFade = CManager::GetFade();

	// モードの取得
	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_GAME)
	{// モードがゲームだったら
		if (pInputKeyboard->GetTrigger(DIK_P) == true && CFade::GetFade() == CFade::FADE_NONE ||
			pXInputJoyPad->GetTrigger(XINPUT_GAMEPAD_START,0) == true && CFade::GetFade() == CFade::FADE_NONE ||
			pXInputJoyPad->GetTrigger(XINPUT_GAMEPAD_START,1) == true && CFade::GetFade() == CFade::FADE_NONE)
		{
			m_bPause = m_bPause ? false : true;

			if (m_bPause == true)
			{// ポーズ中
				if (m_pPause == NULL)
				{// 空
					m_pPause = CPause::Create();

				}
			}
			else
			{// ポーズじゃなかったら
				m_pPause->Uninit();
				delete m_pPause;
				m_pPause = NULL;
			}
		}

		if (m_bPause == false)
		{// 
			CScene::UpdateAll();
		}
		else
		{
			if (m_pPause != NULL)
			{
				m_pPause->Update();
			}
			
			CPauseSelect::RESTART restart;
			restart = CPauseSelect::GetRstart();

			if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
				pXInputJoyPad->GetPress(XINPUT_GAMEPAD_X, 0) == true || pXInputJoyPad->GetPress(XINPUT_GAMEPAD_X, 1) == true ||
				pXInputJoyPad->GetPress(XINPUT_GAMEPAD_Y, 0) == true || pXInputJoyPad->GetPress(XINPUT_GAMEPAD_Y, 1) == true)
			{
				switch (restart)
				{
				case CPauseSelect::RESTART_CONTINUE:  // ゲーム再開
					break;

				case CPauseSelect::RESTART_RETRY:     // ゲームやり直し
					pFade->Set(CManager::MODE_GAME, pFade->FADE_OUT);
					break;

				case CPauseSelect::RESTART_QUIT:      // タイトル遷移
					pFade->Set(CManager::MODE_TITLE, pFade->FADE_OUT);
					break;
				}

				m_bPause = m_bPause ? false : true; // ポーズは解除
				m_pPause->Uninit();
				delete m_pPause;
				m_pPause = NULL;
			}
		}
	}
	else
	{// 
		CScene::UpdateAll();
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void CRenderer::Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	m_pD3DDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	
	// フェードの取得
	CFade *pFade = CManager::GetFade();

	// Direct3Dによる描画の開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		CScene::DrawAll();

		if (pFade != NULL)
		{
			pFade->Draw();
		}


#ifdef _DEBUG
		// FPS表示
		DrawFPS();
#endif
		// Direct3Dによる描画の終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
#ifdef _DEBUG
//=============================================================================
// FPS表示
//=============================================================================
void CRenderer::DrawFPS(void)
{
	int nCountFPS;

	// FPSを取得
	nCountFPS = GetFPS();

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char str[256];

	wsprintf(str, "FPS:%d\n", nCountFPS);

	// テキスト描画
	m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}
////=============================================================================
//// デバイスの取得
////=============================================================================
//LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
//{
//	return m_pD3DDevice;
//}

#endif
