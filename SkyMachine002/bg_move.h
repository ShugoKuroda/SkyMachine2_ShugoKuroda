//-------------------------------------------
//
// ロゴの情報[logo.h]
// Author : SHUGO KURODA
//
//-------------------------------------------
#ifndef _BG_MOVE_H_
#define _BG_MOVE_H_

//-------------------------------------------
// インクルード
//-------------------------------------------
#include "object2D.h"

//-------------------------------------------
// ロゴクラス
//-------------------------------------------
class CBgMove: public CObject2D
{
public:
	// コンストラクタ
	CBgMove();
	// デストラクタ
	~CBgMove() override;

	// 生成
	static CBgMove* Create();
	// テクスチャの読み込み
	static HRESULT Load();
	// テクスチャの削除
	static void Unload();

	// 初期化
	HRESULT Init() override;
	// 終了
	void Uninit() override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;

private:
	// テクスチャのポインタ
	static LPDIRECT3DTEXTURE9 m_pTexture;
	// アニメーション用カウンター
	int m_nCntAnim;
	// 現在のアニメーション番号
	int m_nPatternAnim;
};

#endif		// _BG_MOVE_H_
