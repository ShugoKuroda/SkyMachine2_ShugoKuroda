//===================================================================
//
//	ロード処理のヘッダー[load.h]
//	Author:黒田 周吾
//
//===================================================================
#ifndef _LOAD_H_			//このマクロ定義がされなかったら
#define _LOAD_H_			//2重インクルード防止のマクロ定義

#include "renderer.h"
#include "enemy_data.h"

//======================================================
//	ロード処理のネームスペース
//======================================================
namespace LoadSpace
{
	HRESULT LoadEnemy(HWND hWnd);
	EnemyInfo *GetEnemy();
	ParentEnemyInfo *GetParentEnemy();
	int GetWave();
}

#endif