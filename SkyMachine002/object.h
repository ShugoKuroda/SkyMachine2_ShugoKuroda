//=============================================================================
//
// オブジェクト処理[object.h]
// Author : SHUGO KURODA
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <d3dx9.h>		// 描画処理に必要

//*****************************************************************************
// クラス定義(基本クラス)
//*****************************************************************************
class CObject
{
public:		// 定数
	// オブジェクトの最大数
	static const int MAX_OBJECT = 500;

public:
	enum EObject
	{//オブジェクトの種類(下から描画を優先)
		OBJ_BG = 0,
		OBJ_BG_MOVE,
		OBJ_CLOUD,
		OBJ_TITLE_LOGO,
		OBJ_TITLE,
		OBJ_FADE,
		OBJ_BUBBLE,
		OBJ_PLAYER,
		OBJ_EFFECT,
		OBJ_ENEMYBOSS,
		OBJ_ENEMY,
		OBJ_BULLET,
		OBJ_EXPLOSION,
		OBJ_SPRAY,
		OBJ_WAVE1,
		OBJ_WAVE2,
		OBJ_WAVE3,
		OBJ_SCORE,
		OBJ_LOGO,
		OBJ_PAUSE,
		OBJ_PAUSE_MENU,
		OBJ_MAX
	};

	CObject();
	virtual ~CObject();

	virtual HRESULT Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	static void ReleaseAll();
	static void UpdateAll();
	static void DrawAll();
	static void ShakeAll();

	static CObject* GetObject(int nCnt) { return m_apObject[nCnt]; }
	void SetObjType(EObject type) { m_nType = type; }
	static void SetShake(int nShakeNum);
	static bool GetShake() { return m_bShake; }
	EObject GetObjType() { return m_nType; }

protected:
	void Release();
	void SetShake(const D3DXVECTOR3& pos);

private:
	static CObject *m_apObject[MAX_OBJECT];
	static int m_nNumAll;
	static bool m_bShake;
	static int m_nShakeInterval;
	static D3DXVECTOR3 m_aShakePos[MAX_OBJECT];
	int m_nID;
	EObject m_nType;
};

#endif // _OBJECT_H_
