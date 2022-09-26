//=============================================================================
//
// スコア情報
// Author : SHUGO KURODA
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CNumber;

//*****************************************************************************
// 背景クラス定義
//*****************************************************************************
class CScore : public CObject
{
// 定数定義
	static const int SCORE_SIZE = 30;
	static const int MAX_SCORE = 8;

// メンバ関数
public:
	CScore();										//コンストラクタ
	~CScore() override;								//デストラクタ

	static CScore *Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size, const int& nNumSpace);		//生成
	HRESULT Init() override;						//初期化
	void Uninit() override;							//終了
	void Update() override;							//更新
	void Draw() override;							//描画
	void Add(const int& nScore);					//スコアの加算
	void Set(const int& nScore);					//スコアの加算

	int GetScore() { return m_nScore; }				// スコアの取得
	void SetColor(D3DXCOLOR col);					// 色の設定
	D3DXCOLOR GetColor();							// 色の取得

private:
	void Set();										//スコアの設定

// メンバ変数 //
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = nullptr;	//頂点バッファのポインタ
	static LPDIRECT3DTEXTURE9 m_pTexture;			//テクスチャのポインタ
	CNumber *m_apNumber[MAX_SCORE];					//スコアに使用する数字
	int m_nScore;									//スコアを格納
};

#endif // _SCORE_H_