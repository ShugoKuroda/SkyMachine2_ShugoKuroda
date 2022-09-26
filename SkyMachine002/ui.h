//-------------------------------------------
//
// UI情報[ui.h]
// Author : SHUGO KURODA
//
//-------------------------------------------
#ifndef _UI_H_
#define _UI_H_

//-------------------------------------------
// インクルード
//-------------------------------------------
#include "object2D.h"

//-------------------------------------------
// ロゴクラス
//-------------------------------------------
class CUi : public CObject2D
{
public:
	// どのプレイヤーのUIか
	enum PARENT
	{
		PLAYER_1 = 0,			// 1P
		PLAYER_2,				// 2P
		PLAYER_1_NOT_ENTRY,		// 1P(エントリーしていない場合)
		PLAYER_2_NOT_ENTRY,		// 2P
		PLAYER_NONE,			// どちらでもない
	};

	enum TYPE
	{//種類
		TYPE_NONE = 0,			// 何もなし
		TYPE_AREA_A,			// エリアA
		TYPE_PLAYER1,			// プレイヤー1
		TYPE_PLAYER2,			// プレイヤー2
		TYPE_PRESS_ANY_BUTTON,	// エントリー待ち1
		TYPE_TO_START,			// エントリー待ち2
		TYPE_LIFE1,				// プレイヤー1の残機
		TYPE_LIFE2,				// プレイヤー2の残機
		TYPE_BULLET,			// メイン弾
		TYPE_BULLET_OPTION,		// オプション弾
		TYPE_BARRIER,			// バリア
		TYPE_OK,				// エントリー状態
		TYPE_CONTINUE,			// コンティニュー
		TYPE_MAX
	};

	enum ANIMTYPE
	{//アニメーションの種類
		ANIM_NONE = 0,		// 何もしない
		ANIM_FLASHING,		// 点滅
	};

	// コンストラクタ
	CUi();
	// デストラクタ
	~CUi() override;

	// 生成
	static CUi* Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size, const TYPE& type, const ANIMTYPE& AnimType, const PARENT& parent);
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
	// どのプレイヤーのUIか
	PARENT m_parent;
};

#endif		// _UI_H_
