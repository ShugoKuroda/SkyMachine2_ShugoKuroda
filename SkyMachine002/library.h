//===================================================================
//
// 色々な処理まとめ [library.h]
// Author : SHUGO KURODA
//
//===================================================================
#ifndef _LIBRARY_H_
#define _LIBRARY_H_

#include "renderer.h"

//*******************************************************************
// 色々な処理のネームスペース
//*******************************************************************
namespace LibrarySpace
{
	bool OutScreen2D(const D3DXVECTOR3 *pPos, const D3DXVECTOR2 size);
	bool SteyInScreen2D(D3DXVECTOR3 *pPos, D3DXVECTOR2 size);
	bool BoxCollision2D(const D3DXVECTOR3 *pPos1, const D3DXVECTOR3*pPos2, const D3DXVECTOR3*pSize1, const D3DXVECTOR3*pSize2);
	bool SphereCollision2D(const D3DXVECTOR3& posIn, const D3DXVECTOR3& posOut, const float& fsizeIn, const float& fsizeOut);
	int GetRandNum(int nMax, int nMin);
	float GetRandFloat(int nMax, int nMin, int nNumDiv);
	float GetSinVec(float fRot, float fNumMul);
	float GetCosVec(float fRot, float fNumMul);
	float SphereRange(D3DXVECTOR3 posStart, D3DXVECTOR3 posEnd);
};

#endif