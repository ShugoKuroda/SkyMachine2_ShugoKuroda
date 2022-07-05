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
	static const int MAX_OBJECT = 20;

public:
	enum EObject
	{//オブジェクトの種類
		OBJ_PLAYER = 0,
		OBJ_ENEMY,
		OBJ_BULLET,
		OBJ_EXPLOSION,
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

	static CObject* GetObject(int nCnt) { return m_apObject[nCnt]; }
	static int GetObjectNumAll() { return m_nNumAll; }
	void SetObjectType(EObject type) { m_nType = type; }
	EObject GetObjType() { return m_nType; }

protected:
	void Release();

private:
	static CObject *m_apObject[MAX_OBJECT];
	static int m_nNumAll;
	int m_nID;
	EObject m_nType;
};

#endif // _OBJECT_H_
