//===================================================================
//
//	オプション弾の処理[bullet_option.h]
//	Author:SHUGO KURODA
//
//===================================================================
#ifndef _BULLET_OPTION_H_
#define _BULLET_OPTION_H_

#include "object2D.h"

//*******************************************************************
//	弾クラスの定義
//*******************************************************************
class CBulletOption : public CObject2D
{
public:
	// オプション(小)のサイズ
	static const int SIZE_SMALL = 30;
	// オプション(中)のサイズ
	static const int SIZE_MEDIUM = 60;
	// オプション(大)のサイズ
	static const int SIZE_LARGE = 90;
	// オプション(小)のダメージ量
	static const int DAMAGE_SMALL = 10;
	// オプション(中)のダメージ量
	static const int DAMAGE_MEDIUM = 20;
	// オプション(大)のダメージ量
	static const int DAMAGE_LARGE = 30;

public:
	enum EType
	{//オプションの種類
		TYPE_NONE = 0,
		TYPE_SMALL,
		TYPE_MEDIUM,
		TYPE_LARGE,
		TYPE_MAX
	};

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
	static CBulletOption *Create(const D3DXVECTOR3& pos, const int& nSize, const int& nDamage, const EType& type);		//インスタンス生成処理
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
	float m_fRad;			//角度
	EType m_type;			//弾の種類
	EColor m_col;			//色の種類
	int m_nDamage;			//ダメージ量
};

#endif		// _BULLET_H_
