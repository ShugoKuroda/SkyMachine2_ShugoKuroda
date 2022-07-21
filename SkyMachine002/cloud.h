//===================================================================
//
//	弾の処理[bullet.h]
//	Author:SHUGO KURODA
//
//===================================================================
#ifndef _CLOUD_H_
#define _CLOUD_H_

#include "object2D.h"

//*******************************************************************
//	弾クラスの定義
//*******************************************************************
class CCloud : public CObject2D
{
private:	//定数定義
	//最大テクスチャ数
	static const int MAX_TEX = 2;
	//基準サイズ(X)
	static const int DEFAULT_WIDTH = 200;
	//最高サイズ(X)
	static const int MAX_WIDTH = 400;
	//基準サイズ(Y)
	static const int DEFAULT_HEIGHT = 100;
	//最高サイズ(Y)
	static const int MAX_HEIGHT = 200;
	//基準移動量
	static const int DEFAULT_MOVE = 1;
	//最高移動量
	static const int MAX_MOVEING = 2;

public:
	CCloud();
	~CCloud() override;

	//メンバ関数
	static CCloud *Create();	//インスタンス生成処理
	static HRESULT Load();		//テクスチャの読み込み
	static void Unload();		//テクスチャの削除
	static void AddPos(float fMul) { m_fCloudPos += 0.5f * fMul; }		//位置の加算

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

private:
	//メンバ変数
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEX];	//テクスチャのポインタ
	static float m_fCloudPos;		// 雲の生成位置
	D3DXVECTOR3 m_move;				// 移動量
};

#endif		// _BULLET_H_
