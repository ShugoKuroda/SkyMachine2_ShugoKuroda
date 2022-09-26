//=============================================================================
//
// スコア情報
// Author : SHUGO KURODA
//
//=============================================================================
#ifndef _LIFE_H_
#define _LIFE_H_

#include "object.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CNumber;

//*****************************************************************************
// 背景クラス定義
//*****************************************************************************
class CLife : public CObject
{
	// 定数定義
	static const int LIFE_SIZE = 30;
	static const int MAX_LIFE = 1;

	// メンバ関数
public:
	CLife();										//コンストラクタ
	~CLife() override;								//デストラクタ

	static CLife *Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size);		//生成
	HRESULT Init() override;						//初期化
	void Uninit() override;							//終了
	void Update() override;							//更新
	void Draw() override;							//描画
	void Add(const int& nScore);					//ライフの加算

	// ライフの取得
	int GetLife() { return m_nLife; }

private:
	void Set();										//ライフの設定

	// メンバ変数 //
private:
	static LPDIRECT3DTEXTURE9 m_pTexture;			//テクスチャのポインタ

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				//頂点バッファのポインタ
	CNumber *m_apNumber[MAX_LIFE];					//ライフに使用する数字
	int m_nLife;									//ライフを格納
};

#endif // _LIFE_H_
