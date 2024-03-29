//=============================================================================
//
// ランキングランク処理 [rankingrank.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _RANKINGRANK_H_
#define _RANKINGRANK_H_

#include "main.h"
#include "logo.h"

//*****************************************************************************
// ランキングランククラスの定義
//*****************************************************************************
class CRankingRank : public CLogo
{
public:
	CRankingRank();
	~CRankingRank();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CRankingRank * Create(D3DXVECTOR3 pos);
	static HRESULT Load(void);
	static void Unload(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	// 共有テクスチャのポインタ
};
#endif
