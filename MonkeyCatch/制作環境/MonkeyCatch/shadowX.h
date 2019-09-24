//=============================================================================
//
// 影(円柱)の処理 [shadowX.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _SHADOWX_H_
#define _SHADOWX_H_

#include "main.h"
#include "sceneX.h"

class CPlayer;
class CEnemy;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SHADOW_MODEL_NAME000 "data/MODEL/shadow000.x"
#define SHADOW_MOVE_SPEED (2.0f)

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CShadowX : public CSceneX
{// 派生させる時は : public ～を後ろにつけてやる
public:

	typedef enum
	{// オブジェクトの種類
		OBJECTTYPE_TREE = 0,
		OBJECTTYPE_MAX
	}OBJECTTYPE;

	CShadowX();
	~CShadowX();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CShadowX * Create(D3DXVECTOR3 pos);
	static HRESULT LoadX(void);
	static void UnloadX(void);

	// 静的メンバ変数
	static LPDIRECT3DTEXTURE9		*m_pTexture;	// テクスチャへのポインタ
	static LPD3DXMESH		m_pMesh;				// メッシュ情報へのポインタ
	static LPD3DXBUFFER		m_pBuffMat;				// マテリアル情報へのポインタ
	static DWORD			m_nNumMat;				// マテリアル情報の数
	static CPlayer * m_pPlayer;
	static CEnemy * m_pEnemy;

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// 頂点バッファへのポインタ
	D3DXVECTOR3				m_pos;					// 位置
	D3DXVECTOR3				m_posold;				// 位置
	D3DXVECTOR3				m_rot;					// 向き
	D3DXVECTOR3				m_move;					// 移動
	D3DXMATRIX				m_mtxWorld;				// ワールドマトリックス
	OBJECTTYPE				m_type;					// 種類
	D3DXMATRIX m_mtxRot, m_mtxTrans;			// 計算用マトリックス
	float m_fDest;								// 目的の角度
	float m_fDiff;								// 角度の差分

};
#endif