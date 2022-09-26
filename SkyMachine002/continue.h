//=============================================================================
//
// コンティニュー情報[continue.h]
// Author : SHUGO KURODA
//
//=============================================================================
#ifndef _CONTINUE_H_
#define _CONTINUE_H_

#include "object.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CNumber;
class CUi;

//*****************************************************************************
// 背景クラス定義
//*****************************************************************************
class CContinue : public CObject
{
	// 定数定義
	static const int LIFE_SIZE = 30;
	static const int MAX_CONTINUE = 2;
	static const int MAX_UI = 2;

	// メンバ関数
public:
	CContinue();									//コンストラクタ
	~CContinue() override;							//デストラクタ

	static CContinue *Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size);		//生成
	HRESULT Init() override;						//初期化
	void Uninit() override;							//終了
	void Update() override;							//更新
	void Draw() override;							//描画
	void Add(const int& nScore);					//カウントダウンの加算

	// ライフの取得
	int GetLife() { return m_nContinue; }
	// コンティニュー演出状態の設定
	static void SetContinue(bool bContinue) { m_bContinue = bContinue; }
	// コンティニュー演出状態の取得
	static bool GetContinue() { return m_bContinue; }

private:
	void Set();										//数値の設定

	// メンバ変数 //
private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャのポインタ

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//頂点バッファのポインタ
	CNumber *m_apNumber[MAX_CONTINUE];		//ライフに使用する数字
	CUi *m_pUi[MAX_UI];						//UI情報の格納用
	CObject2D *m_pFade;						//フェード用
	int m_nContinue;						//ライフを格納
	int m_nCountDown;						//コンティニューのカウントダウン
	static bool m_bContinue;				//コンティニュー中かどうか
};

#endif // _LIFE_H_
