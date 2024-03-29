//=============================================================================
//
// スコア処理 [score.h]
// Author : 竹内亘
//
//=============================================================================
#include "score.h"
#include "number.h"
#include "renderer.h"
#include "manager.h"

////=============================================================================
//// 静的メンバ変数
////=============================================================================
//int CScore::m_nScore = 0;
//CNumber * CScore::m_apNumber[MAX_SCORE] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CScore::CScore()
{
	m_nScore = 0;
}
//=============================================================================
// デストラクタ
//=============================================================================
CScore::~CScore()
{
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScore::Init(D3DXVECTOR3 pos)
{
	for (int nCount = 0; nCount < MAX_SCORE; nCount++)
	{
		m_apNumber[nCount] = new CNumber;

		//m_apNumber[nCount]->Create(D3DXVECTOR3(pos.x + (SCORE_INTERVAL_SIZE * nCount), pos.y, pos.z));// 謎
		m_apNumber[nCount]->Init(D3DXVECTOR3(pos.x - (SCORE_INTERVAL_SIZE * nCount), pos.y, pos.z));
	}

	m_nScore = 0;

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CScore::Uninit(void)
{
	for (int nCount = 0; nCount < MAX_SCORE; nCount++)
	{
		m_apNumber[nCount]->Uninit();
	}

	// オブジェクト(自分自身)の破棄
	Release();
}
//=============================================================================
// 更新処理
//=============================================================================
void CScore::Update(void)
{
	for (int nCount = 0; nCount < MAX_SCORE; nCount++)
	{
		m_apNumber[nCount]->Update();
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void CScore::Draw(void)
{
	for (int nCount = 0; nCount < MAX_SCORE; nCount++)
	{
		m_apNumber[nCount]->Draw();
	}
}
////=============================================================================
//// スコアの設置
////=============================================================================
//void CScore::SetScore(int nScore)
//{
//	m_nScore = nScore;
//}
//=============================================================================
// スコアの取得
//=============================================================================
int CScore::GetScore(void)
{
	return m_nScore;
}
//=============================================================================
// スコアの加算
//=============================================================================
void CScore::AddScore(int nValue)
{
	m_nScore += nValue;

	int nAnswer = 1;
	int nScore;

	for (int nCount = 0; nCount < MAX_SCORE; nCount++)
	{
		nScore = m_nScore % (nAnswer * 10) / nAnswer;
		m_apNumber[nCount]->SetNumber(nScore);
		nAnswer *= 10;
	}
}
//=============================================================================
// スコアの生成
//=============================================================================
CScore * CScore::Create(D3DXVECTOR3 pos)
{
	CScore *pScore = NULL;

	if (pScore == NULL)
	{
		pScore = new CScore;

		if (pScore != NULL)
		{
			pScore->Init(pos);
		}
	}
	return pScore;
}
