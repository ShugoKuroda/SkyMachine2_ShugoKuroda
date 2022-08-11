//=============================================================================
//
// 数字ポリゴン[number.h]
// Author : 
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "object2D.h"

//*****************************************************************************
// 背景クラス定義
//*****************************************************************************
class CNumber : public CObject2D
{
// 定数定義 //
public:
	// U座標の最大分割数
	static const int DIVISION_U = 16;
	// V座標の最大分割数
	static const int DIVISION_V = 1;

// メンバ関数 //
public:
	CNumber();								//コンストラクタ
	~CNumber() override;					//デストラクタ

	static HRESULT Load();					//テクスチャ読み込み
	static void Unload();					//テクスチャ破棄
	HRESULT Init() override;				//初期化
	void Uninit() override;					//終了
	void Update() override;					//更新
	void Draw() override;					//描画

// メンバ変数 //
private:
	static LPDIRECT3DTEXTURE9 m_pTexture;			//テクスチャのポインタ
};

#endif // _BULLET_H_
