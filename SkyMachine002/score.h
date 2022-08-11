//=============================================================================
//
// スコアヘッダ
// Author : 
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

//前方宣言
class CNumber;

//*****************************************************************************
// 背景クラス定義
//*****************************************************************************
class CScore : public CObject
{
// 定数定義 //
	static const int SCORE_SIZE = 30;
	static const int MAX_SCORE = 8;

// メンバ関数 //
public:
	CScore();										//コンストラクタ
	~CScore() override;								//デストラクタ

	static CScore *Create();						//生成
	HRESULT Init() override;						//初期化
	void Uninit() override;							//終了
	void Update() override;							//更新
	void Draw() override;							//描画
	void Add(const int& nScore);					//スコアの加算

private:
	void Set();										//スコアの設定

// メンバ変数 //
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = nullptr;	//頂点バッファのポインタ
	static LPDIRECT3DTEXTURE9 m_pTexture;			//テクスチャのポインタ
	CNumber *pNumber[MAX_SCORE];					//スコアに使用する数字
	int m_nScore;									//スコアを格納
};

#endif // !_SCORE_H_