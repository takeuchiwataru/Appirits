//=============================================================================
//
// テクスチャ処理 [texture.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TEXTURE (14)

#define FRAME_TEXTURE000 "data/TEXTURE/timer_frame2.png"		// タイマーフレーム
#define FRAME_TEXTURE001 "data/TEXTURE/score_frame.png"			// スコアフレーム
#define IMAGE_TEXTURE000 "data/TEXTURE/remain.png"				// サルのカウント画像
#define IMAGE_TEXTURE001 "data/TEXTURE/UI000.png"				// 残り
#define IMAGE_TEXTURE002 "data/TEXTURE/tutorial000.png"			// コントローラー説明
#define IMAGE_TEXTURE003 "data/TEXTURE/UI001.png"				// 捕まえた数
#define IMAGE_TEXTURE004 "data/TEXTURE/UI002.png"				// 残り時間
#define IMAGE_TEXTURE005 "data/TEXTURE/UI003.png"				// 合計
#define PARTICLE_TEXTURE_000 "data/TEXTURE/effect000.jpg"		// エフェクトのテクスチャ
#define PARTICLE_TEXTURE_001 "data/TEXTURE/effect000.jpg"		// エフェクトのテクスチャ
#define PAUSEMAT_TEXTURE	"data/TEXTURE/pause100.png"			// ポーズ台紙のテクスチャ
#define PAUSESELECT_TEXTURE000	"data/TEXTURE/pause000.png"		// CONTINUE
#define PAUSESELECT_TEXTURE001	"data/TEXTURE/pause001.png"		// RETRY
#define PAUSESELECT_TEXTURE002	"data/TEXTURE/pause002.png"		// QUIT

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CTexture
{
public:
	CTexture();
	~CTexture();
	static HRESULT Load(void);
	static void Unload(void);
	static LPDIRECT3DTEXTURE9 GetTexture(int nIdx);

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEXTURE];		// 配列ver
	//static LPDIRECT3DTEXTURE9 m_pTexture;					// 動的確保ver

	static const char *m_apFileName[];

};

#endif