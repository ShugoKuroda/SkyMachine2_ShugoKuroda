//===================================================================
// 
//	敵の生成情報[enemy_data.h]
//	Author : SHUGO KURODA
//
//===================================================================
#ifndef _ENEMY_DATA_H_
#define _ENEMY_DATA_H_

#include "renderer.h"

//===================================================================
// マクロ定義
//===================================================================
#define MAX_MOVE		(30)		//最大移動回数
#define MAX_WAVE		(30)		//最大ウェーブ数(初期値)

//*******************************************************************
//	敵のロード情報の定義
//*******************************************************************
typedef struct
{
	D3DXVECTOR3 posEnd;		//目標の位置
	int nFrameMove;			//移動時間(フレーム)
}EnemyMove;

typedef struct
{
	EnemyMove MoveInfo[MAX_MOVE];	//移動情報
	D3DXVECTOR3 pos;	//出現する位置
	int nMaxMove;		//最大移動回数
	int nFrame;			//出現するフレーム(時間)
	int nType;			//敵の種類
	int nLife;
	int nNum;			//出現する敵の数
	int nInterval;		//連続で敵を出す間隔
}EnemyInfo;

//敵の生成カウンター情報
typedef struct
{
	// 敵が連続で生成されるまでの時間
	int nCreateTime;
	// 敵が生成された数
	int nCreateNum;
	//敵を生成しないかどうか
	bool bCreate;
}EnemyCount;

typedef struct
{
	// 敵が生成されるまでの時間
	int nCreatenCount;
	//敵の生成カウンター情報
	EnemyCount CountInfo[MAX_WAVE];
	// 敵の生成情報
	EnemyInfo *pCreateInfo;
	// 最大ウェーブ数
	int nWave;
}EnemySetInfo;

#endif
