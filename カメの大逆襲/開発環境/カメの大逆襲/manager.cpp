//=============================================================================
//
// マネージャー処理 [manager.cpp]
// Author : 竹内亘
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"

#include "game.h"
#include "title.h"
#include "result.h"
#include "result2.h"
#include "ranking.h"
#include "tutorial.h"
#include "fade.h"
#include "sound.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
CRenderer * CManager::m_pRenderer = NULL;
CInputKeyboard * CManager::m_pInputKeyboard = NULL;
CInputJoyPad * CManager::m_pInputJoyPad = NULL;
CXInputJoyPad * CManager::m_pXInputJoyPad = NULL;
CSound * CManager::m_pSound = NULL;
CManager::MODE CManager::m_mode = MODE_TITLE;
CTitle * CManager::m_pTitle = NULL;
CGame * CManager::m_pGame = NULL;
CResult * CManager::m_pResult = NULL;
CResult2 * CManager::m_pResult2 = NULL;
CRanking * CManager::m_pRanking = NULL;
CTutorial * CManager::m_pTutorial = NULL;
CFade * CManager::m_pFade = NULL;
int CManager::m_aScore[MAX_RANKING] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CManager::CManager()
{
	MODE m_mode = MODE_TITLE;
}
//=============================================================================
// デストラクタ
//=============================================================================
CManager::~CManager()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// レンダリングクラスの生成
	if (m_pRenderer == NULL)
	{// 空
		m_pRenderer = new CRenderer;

		m_pRenderer->Init(hWnd, bWindow);

		//CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f));		// センター
		//CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f + 150, SCREEN_HEIGHT / 2.0f, 0.0f));	// 右
		//CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f - 150, SCREEN_HEIGHT / 2.0f, 0.0f));	// 左
	}

	// キーボードクラスの生成
	if (m_pInputKeyboard == NULL)
	{// 空
		m_pInputKeyboard = new CInputKeyboard;

		m_pInputKeyboard->Init(hInstance, hWnd);
	}

	// ジョイパッドクラスの生成
	if (m_pInputJoyPad == NULL)
	{// 空
		m_pInputJoyPad = new CInputJoyPad;

		m_pInputJoyPad->Init(hInstance, hWnd);
	}

	// ジョイパッドクラス(XInput)の生成
	if (m_pXInputJoyPad == NULL)
	{
		m_pXInputJoyPad = new CXInputJoyPad;

		m_pXInputJoyPad->Init();
	}

	// 音の生成
	if (m_pSound == NULL)
	{
		m_pSound = new CSound;

		m_pSound->Init(hWnd);
	}

	// フェードの生成
	m_pFade = CFade::Create();
	m_pFade->Set(m_mode, m_pFade->FADE_IN);

	//// モードの設定
	//SetMode(m_mode);

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CManager::Uninit(void)
{
	//　レンダリングクラスの破棄
	if (m_pRenderer != NULL)
	{
		// 終了処理
		m_pRenderer->Uninit();

		// データの開放
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	//　キーボードの破棄
	if (m_pInputKeyboard != NULL)
	{
		// 終了処理
		m_pInputKeyboard->Uninit();

		// データの開放
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	//　ジョイパッドの破棄
	if (m_pInputJoyPad != NULL)
	{
		// 終了処理
		m_pInputJoyPad->Uninit();

		// データの開放
		delete m_pInputJoyPad;
		m_pInputJoyPad = NULL;
	}

	//　ジョイパッド(XInput)の破棄
	if (m_pXInputJoyPad != NULL)
	{
		// データの開放
		delete m_pXInputJoyPad;
		m_pXInputJoyPad = NULL;
	}

	// サウンドの破棄
	if (m_pSound != NULL)
	{
		// 終了処理
		m_pSound->Uninit();

		delete m_pSound;
		m_pSound = NULL;
	}

	// フェードの破棄
	if (m_pFade != NULL)
	{
		// 終了処理
		m_pFade->Uninit();

		delete m_pFade;
		m_pFade = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void CManager::Update(void)
{
	if (m_pRenderer != NULL)
	{// レンダリングクラスの更新処理
		m_pRenderer->Update();
	}
	if (m_pInputKeyboard != NULL)
	{// キーボードの更新処理
		m_pInputKeyboard->Update();
	}
	if (m_pInputJoyPad != NULL)
	{// ジョイパッドの更新処理
		m_pInputJoyPad->Update();
	}

	if (m_pXInputJoyPad != NULL)
	{// ジョイパッド(XInput)の更新処理
		m_pXInputJoyPad->Update();
	}

	// サウンドの取得
	CSound * pSound = CManager::GetSound();

	switch (m_mode)
	{
	case MODE_TITLE:
		if (m_pTitle != NULL)
		{
			m_pTitle->Update();
		}
		break;


	case MODE_GAME:
		if (m_pGame != NULL)
		{
			m_pGame->Update();
		}
		break;

	case MODE_RESULT:
		if (m_pResult != NULL)
		{
			m_pResult->Update();
		}
		break;

	case MODE_RESULT2:
		if (m_pResult2 != NULL)
		{
			m_pResult2->Update();
		}
		break;

	case MODE_RANKING:
		if (m_pRanking != NULL)
		{
			m_pRanking->Update();
		}
		break;

	case MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{
			m_pTutorial->Update();

		}
		break;

	}

	if (m_pFade != NULL)
	{
		m_pFade->Update();
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void CManager::Draw(void)
{
	if (m_pRenderer != NULL)
	{// レンダリングクラスの描画処理
		m_pRenderer->Draw();
	}

	switch (m_mode)
	{
	case MODE_TITLE:
		if (m_pTitle != NULL)
		{
			m_pTitle->Draw();
		}
		break;

	case MODE_GAME:
		if (m_pGame != NULL)
		{
			m_pGame->Draw();
		}
		break;

	case MODE_RESULT:
		if (m_pResult != NULL)
		{
			m_pResult->Draw();
		}
		break;

	case MODE_RESULT2:
		if (m_pResult2 != NULL)
		{
			m_pResult2->Draw();
		}
		break;

	case MODE_RANKING:
		if (m_pRanking != NULL)
		{
			m_pRanking->Draw();
		}
		break;

	case MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{
			m_pTutorial->Draw();
		}
		break;
	}
}
//=============================================================================
// モードの設定
//=============================================================================
void CManager::SetMode(MODE mode)
{
	// サウンドの取得
	CSound * pSound = CManager::GetSound();

	switch (m_mode)
	{
	case MODE_TITLE:
		if (m_pTitle != NULL)
		{
			m_pTitle->Uninit();

			delete m_pTitle;
			m_pTitle = NULL;
		}
		break;

	case MODE_GAME:
		if (m_pGame != NULL)
		{
			m_pGame->Uninit();

			delete m_pGame;
			m_pGame = NULL;

		}
		break;

	case MODE_RESULT:
		if (m_pResult != NULL)
		{
			m_pResult->Uninit();

			delete m_pResult;
			m_pResult = NULL;
		}
		break;

	case MODE_RESULT2:
		if (m_pResult2 != NULL)
		{
			m_pResult2->Uninit();

			delete m_pResult2;
			m_pResult2 = NULL;
		}
		break;

	case MODE_RANKING:
		if (m_pRanking != NULL)
		{
			m_pRanking->Uninit();

			delete m_pRanking;
			m_pRanking = NULL;
		}
		break;

	case MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{
			m_pTutorial->Uninit();
			
			delete m_pTutorial;
			m_pTutorial = NULL;
		}
		break;
	}

	switch (mode)
	{
	case MODE_TITLE:
		if (m_pTitle == NULL)
		{
			m_pTitle = new CTitle;

			m_pTitle->Init();

			pSound->StopSound(CSound::SOUND_LABEL_BGM_GAME);
			pSound->StopSound(CSound::SOUND_LABEL_BGM_CLEAR);
			pSound->StopSound(CSound::SOUND_LABEL_BGM_OVER);

			pSound->PlaySound(CSound::SOUND_LABEL_BGM_TITLE);
		}
		break;

	case MODE_GAME:
		if (m_pGame == NULL)
		{
			m_pGame = new CGame;

			m_pGame->Init();
			pSound->StopSound(CSound::SOUND_LABEL_SE_PS);
			pSound->StopSound(CSound::SOUND_LABEL_BGM_TUTORIAL);
			pSound->PlaySound(CSound::SOUND_LABEL_BGM_GAME);
		}
		break;

	case MODE_RESULT:
		if (m_pResult == NULL)
		{
			m_pResult = new CResult;


			m_pResult->Init();
			pSound->StopSound(CSound::SOUND_LABEL_SE_PS);
			pSound->StopSound(CSound::SOUND_LABEL_BGM_GAME);
			pSound->PlaySound(CSound::SOUND_LABEL_BGM_OVER);
		}
		break;

	case MODE_RESULT2:
		if (m_pResult2 == NULL)
		{
			m_pResult2 = new CResult2;

			m_pResult2->Init();
			pSound->StopSound(CSound::SOUND_LABEL_SE_PS);
			pSound->StopSound(CSound::SOUND_LABEL_BGM_GAME);
			pSound->PlaySound(CSound::SOUND_LABEL_BGM_CLEAR);
		}
		break;


	case MODE_RANKING:
		if (m_pRanking == NULL)
		{
			m_pRanking = new CRanking;

			m_pRanking->Init();
		}
		break;

	case MODE_TUTORIAL:
		if (m_pTutorial == NULL)
		{
			m_pTutorial = new CTutorial;

			m_pTutorial->Init();

			pSound->StopSound(CSound::SOUND_LABEL_BGM_TITLE);
			pSound->PlaySound(CSound::SOUND_LABEL_BGM_TUTORIAL);

		}
		break;
	}

	m_mode = mode;	//現在のモードを切り替える
}
//=============================================================================
// モードの取得
//=============================================================================
CManager::MODE CManager::GetMode(void)
{
	return m_mode;
}
//=============================================================================
// レンダリングの取得
//=============================================================================
CRenderer *CManager::GetRenderer(void)
{
	return m_pRenderer;
}
//=============================================================================
// キーボードの取得
//=============================================================================
CInputKeyboard *CManager::GetInputKeyboard(void)
{
	return m_pInputKeyboard;
}
//=============================================================================
// ジョイパッドの取得
//=============================================================================
CInputJoyPad *CManager::GetJoyPad(void)
{
	return m_pInputJoyPad;
}
//=============================================================================
// ジョイパッド(XInput)の取得
//=============================================================================
CXInputJoyPad *CManager::GetXInputJoyPad(void)
{
	return m_pXInputJoyPad;
}
//=============================================================================
// サウンドの取得
//=============================================================================
CSound * CManager::GetSound(void)
{
	return m_pSound;
}
//=============================================================================
// フェードの取得
//=============================================================================
CFade * CManager::GetFade(void)
{
	return m_pFade;
}
//=============================================================================
// ランキングスコアの取得
//=============================================================================
int CManager::GetRanking(int index)
{
	return m_aScore[index];
}
//=============================================================================
// ランキングスコアの取得
//=============================================================================
void CManager::GetRankingScore(int nScore)
{
	int nData = 0;

	if (m_aScore[MAX_RANKING - 1] <= nScore)
	{
		m_aScore[MAX_RANKING - 1] = nScore;
	}

	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		for (int nNumber = 0; nNumber < MAX_RANKING -1; nNumber++)
		{
			if (m_aScore[nNumber] < m_aScore[nNumber + 1])
			{
				nData = m_aScore[nNumber];
				m_aScore[nNumber] = m_aScore[nNumber + 1];
				m_aScore[nNumber + 1] = nData;
			}
		}
	}
}
