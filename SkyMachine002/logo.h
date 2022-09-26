//-------------------------------------------
//
// ロゴの情報[logo.h]
// Author : SHUGO KURODA
//
//-------------------------------------------
#ifndef _LOGO_H_
#define _LOGO_H_

//-------------------------------------------
// インクルード
//-------------------------------------------
#include "object2D.h"

//-------------------------------------------
// ロゴクラス
//-------------------------------------------
class CLogo : public CObject2D
{
public:
	enum LOGOTYPE
	{//ロゴの種類
		TYPE_NONE = 0,		// 何もなし
		TYPE_WARNING,		// ボス接近ロゴ
		TYPE_WARNING_SUB,	// ボス接近の説明ロゴ
		TYPE_REMINDER,		// 注意喚起標識のロゴ
		TYPE_CLEAR,			// ゲームクリアのロゴ
		TYPE_BONUS,			// ボーナススコアのロゴ
		TYPE_GAMEOVER,		// ゲームオーバーのロゴ
		TYPE_MAX
	};

	enum ANIMTYPE
	{//アニメーションの種類
		ANIM_NONE = 0,		// 何もしない
		ANIM_LENGTHWISE,	// 縦に広がる
		ANIM_HORIZONTALLY,	// 横に広がる
		ANIM_FLASHING,		// 点滅
		ANIM_MAX
	};

	// コンストラクタ
	CLogo();
	// デストラクタ
	~CLogo() override;

	// 生成
	static CLogo* Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size, const D3DXCOLOR& col, const float& fRot, const LOGOTYPE& type, const ANIMTYPE& AnimType, const int& nCount);
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
	static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_MAX];
	// アニメーションの種類
	ANIMTYPE m_AnimType;
	// 破棄するまでのカウンター
	int m_nCountUninit;
	// 元のサイズ記憶用
	D3DXVECTOR2 m_DefaultSize;

	LOGOTYPE m_type;
};

#endif		// _LOGO_H_
