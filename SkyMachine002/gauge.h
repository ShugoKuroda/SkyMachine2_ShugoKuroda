//-------------------------------------------
//
// 能力ゲージ情報[gauge.h]
// Author : SHUGO KURODA
//
//-------------------------------------------
#ifndef _GAUGE_H_
#define _GAUGE_H_

//-------------------------------------------
// インクルード
//-------------------------------------------
#include "object2D.h"

//*******************************************************************
// 前方宣言
//*******************************************************************
class CPlayer;

//-------------------------------------------
// 能力ゲージクラス
//-------------------------------------------
class CGauge : public CObject2D
{
private:
	// 最大ゲージ数
	static const int MAX_LEVEL = 4;
	// サイズX
	static const float SIZE_X;
	// サイズY
	static const float SIZE_Y;

public:
	// どのプレイヤーのゲージか
	enum PARENT
	{
		PLAYER_1 = 0,	//1P
		PLAYER_2,		//2P
	};

	//どの種類のゲージか
	enum TYPE
	{
		TYPE_NONE = 0,			// 何もなし
		TYPE_BULLET,			// メイン弾
		TYPE_BULLET_OPTION,		// オプション弾
		TYPE_BARRIER,			// バリア
		TYPE_MAX
	};

	// コンストラクタ
	CGauge();
	// デストラクタ
	~CGauge() override;

	// 生成
	static CGauge* Create(const D3DXVECTOR3& pos, const TYPE& type, const PARENT& parent);
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

	// 状態管理
	void State(CPlayer *pPlayer);

private:
	// テクスチャのポインタ
	static LPDIRECT3DTEXTURE9 m_pTexture;
	// ゲージの種類
	TYPE m_type;
	// どのプレイヤーの能力ゲージか
	PARENT m_parent;
};

#endif		// _GAUGE_H_
