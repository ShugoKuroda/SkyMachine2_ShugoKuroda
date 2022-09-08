//=============================================================================
//
// ポーズ画面情報 [pause.h]
// Author:SHUGO KURODA
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "object.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CObject;			// オブジェクトクラス

//*****************************************************************************
// ポーズ画面クラス(派生クラス)
//*****************************************************************************
class CPause : public CObject
{
public:
	enum TYPE
	{//ポーズの種類
		TYPE_FRAME = 0,		// ポーズ画面枠
		TYPE_RESUME,		// 再開
		TYPE_RESTART,		// やり直し
		TYPE_EXIT,			// タイトルに戻る
		TYPE_MAX			// ポーズの最大数
	};

	CPause();
	~CPause();

	static HRESULT Load(void);		//テクスチャの読み込み
	static void Unload(void);		//テクスチャの削除
	static CPause *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

private:
	bool m_bPause;
	int m_nNumController;

	int m_nPauseSelect;					// ポーズメニューから遷移する場所を保存

	static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];	//テクスチャのポインタ
	static CObject2D *m_apObject2D[TYPE_MAX];			//2Dポリゴンへのポインタ
};
#endif