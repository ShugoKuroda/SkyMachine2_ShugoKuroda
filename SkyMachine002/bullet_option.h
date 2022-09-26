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
private:
	// オプションの直径サイズ
	static const int SIZE = 30;
	// オプションのダメージ量
	static const int DAMAGE = 5;

public:
	// オプションの最大生成数
	static const int MAX_NUM = 4;

	// 親プレイヤー
	enum PARENT
	{
		PLAYER_1 = 0,	//1P
		PLAYER_2,		//2P
	};

	// 強化状態
	enum LEVEL
	{
		LEVEL_NONE = 0,
		LEVEL_1,		//1段階目
		LEVEL_2,		//2段階目
		LEVEL_3,		//3段階目
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
	static CBulletOption *Create(const float& fRot, const PARENT& parent);		//インスタンス生成処理
	static HRESULT Load();		//テクスチャの読み込み
	static void Unload();		//テクスチャの削除

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Attack();
	void SetRad(float fRad) { m_fRad = fRad; }

private:
	//メンバ変数
	static LPDIRECT3DTEXTURE9 m_apTexture;		// テクスチャのポインタ
	float m_fRad;				// 角度
	EColor m_col;				// 色の種類
	PARENT m_parent;			// 所有するプレイヤー
	int m_nCounterAttack;		// 攻撃カウンター
};

#endif		// _BULLET_H_
