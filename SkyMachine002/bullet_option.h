//===================================================================
//
//	オプション弾の処理[bullet_option.h]
//	Author:SHUGO KURODA
//
//===================================================================
#ifndef _BULLET_OPTION_H_
#define _BULLET_OPTION_H_

#include "object2D.h"
#include "player.h"

//*******************************************************************
//	弾クラスの定義
//*******************************************************************
class CBulletOption : public CObject2D
{
public:
	// オプションの直径サイズ
	static const int SIZE = 30;
	// オプションのダメージ量
	static const int DAMAGE = 10;

public:
	enum EColor
	{//色の変化状態
		FADE_NONE = 0,
		FADE_RED,
		FADE_GREEN,
		FADE_BLUE,
	};

	CBulletOption();
	~CBulletOption() override;

	//メンバ関数
	static CBulletOption *Create(const D3DXVECTOR3& pos, const CPlayer::PLAYER& parent);		//インスタンス生成処理
	static HRESULT Load();		//テクスチャの読み込み
	static void Unload();		//テクスチャの削除

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	bool Collision(D3DXVECTOR3 posStart);

private:
	//メンバ変数
	static LPDIRECT3DTEXTURE9 m_apTexture;		//テクスチャのポインタ
	float m_fRad;				//角度
	EColor m_col;				//色の種類
	int m_nDamage;				//ダメージ量
	CPlayer::PLAYER m_parent;	//所有するプレイヤー
};

#endif		// _BULLET_H_
