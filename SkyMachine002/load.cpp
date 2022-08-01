//=============================================================================
//
// ロード処理 [load.cpp]
// Author : SHUGO KURODA
//
//=============================================================================
#include <stdio.h>
#include "load.h"
#include "enemy_data.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_CHAR	(256)			//一行の最大読み取り文字数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
EnemyInfo g_aEnemyInfo[MAX_MOVE];	//敵情報の取得
int g_nMaxWave;

//-----------------------------------------------------------------------------
//	敵のセットロード
//-----------------------------------------------------------------------------
HRESULT LoadSpace::LoadEnemy(HWND hWnd)
{
	FILE *pFile = NULL;		//ファイルポインター宣言

	ZeroMemory(&g_aEnemyInfo, sizeof(g_aEnemyInfo));	//初期化
	int nIdxEnemy = 0;		//敵情報の配列番号の保存用

	char cScanf[MAX_CHAR];		//一行分読み取るための変数
	char cScanfHead[MAX_CHAR];	//頭の文字を読み取るための変数
	bool bReadScript = false;	//スクリプトを読み込むかどうか

	//ファイルを開く
	pFile = fopen("data/TEXT/set_enemy.txt", "r");

	if (pFile == NULL)
	{//ファイルを開けなかった場合
		MessageBox(hWnd, "敵配置テキストを開けませんでした", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	//文字列の読み取りループ処理
	while (fgets(cScanf, MAX_CHAR, pFile) != NULL)
	{
		//文字列の分析
		sscanf(cScanf, "%s", &cScanfHead);

		if (!bReadScript && strcmp(&cScanfHead[0], "SCRIPT") == 0)
		{//スクリプトの読み込みを開始
			bReadScript = true;
		}
		else if (strcmp(&cScanfHead[0], "MAX_WAVE") == 0)
		{//最大ウェーブ数
			sscanf(cScanf, "%s = %d", &cScanfHead, &g_nMaxWave);
		}
		else if (bReadScript)
		{//SCRIPT読み込みを開始したら
			if (strcmp(&cScanfHead[0], "ENEMYSET") == 0)
			{//敵の配置を開始

				//プレイヤー情報の読み取りループ処理
				while (fgets(cScanf, MAX_CHAR, pFile) != NULL)
				{
					//文字列の分析
					sscanf(cScanf, "%s", &cScanfHead);

					if (strcmp(&cScanfHead[0], "MAXMOVE") == 0)
					{//最大移動回数
						sscanf(cScanf, "%s = %d", &cScanfHead, &g_aEnemyInfo[nIdxEnemy].nMaxMove);
					}
					else if (strcmp(&cScanfHead[0], "FRAME") == 0)
					{//出現フレーム
						sscanf(cScanf, "%s = %d", &cScanfHead, &g_aEnemyInfo[nIdxEnemy].nFrame);
					}
					else if (strcmp(&cScanfHead[0], "POS") == 0)
					{//出現位置
						sscanf(cScanf, "%s = %f%f%f", &cScanfHead, &g_aEnemyInfo[nIdxEnemy].pos.x, &g_aEnemyInfo[nIdxEnemy].pos.y, &g_aEnemyInfo[nIdxEnemy].pos.z);
					}
					else if (strcmp(&cScanfHead[0], "TYPE") == 0)
					{//敵の種類
						sscanf(cScanf, "%s = %d", &cScanfHead, &g_aEnemyInfo[nIdxEnemy].nType);
					}
					else if (strcmp(&cScanfHead[0], "LIFE") == 0)
					{//敵のライフ
						sscanf(cScanf, "%s = %d", &cScanfHead, &g_aEnemyInfo[nIdxEnemy].nLife);
					}
					else if (strcmp(&cScanfHead[0], "NUM") == 0)
					{//敵の数
						sscanf(cScanf, "%s = %d", &cScanfHead, &g_aEnemyInfo[nIdxEnemy].nNum);
					}
					else if (strcmp(&cScanfHead[0], "INTERVAL") == 0)
					{//敵の数
						sscanf(cScanf, "%s = %d", &cScanfHead, &g_aEnemyInfo[nIdxEnemy].nInterval);
					}
					else if (strcmp(&cScanfHead[0], "MOVESET") == 0)
					{
						int nIdxMove = 0;
						//プレイヤーパーツ情報の読み取りループ処理
						while (fgets(cScanf, MAX_CHAR, pFile) != NULL)
						{
							//文字列の分析
							sscanf(cScanf, "%s", &cScanfHead);

							if (strcmp(&cScanfHead[0], "MOVE") == 0)
							{//移動位置の設定
								sscanf(cScanf, "%s = %f%f%d", &cScanfHead,
									&g_aEnemyInfo[nIdxEnemy].MoveInfo[nIdxMove].posEnd.x,
									&g_aEnemyInfo[nIdxEnemy].MoveInfo[nIdxMove].posEnd.y,
									&g_aEnemyInfo[nIdxEnemy].MoveInfo[nIdxMove].nFrameMove);
								//配列番号の加算
								nIdxMove++;
							}
							else if (strcmp(&cScanfHead[0], "END_MOVESET") == 0)
							{//パーツ読み込み終了
								break;
							}
						}
					}
					else if (strcmp(&cScanfHead[0], "END_ENEMYSET") == 0)
					{//プレイヤー読み込み終了
						nIdxEnemy++;
						break;
					}
				}
			}
		}
		else if (strcmp(&cScanfHead[0], "END_SCRIPT") == 0)
		{//テキストファイルを読み切った時
			break;
		}
	}

	//ファイルを閉じる
	fclose(pFile);

	return S_OK;
}

//-----------------------------------------------------------------------------
//	敵の配置情報の取得
//-----------------------------------------------------------------------------
EnemyInfo *LoadSpace::GetEnemy()
{
	return &g_aEnemyInfo[0];
}

//-----------------------------------------------------------------------------
//	最大ウェーブ数の取得
//-----------------------------------------------------------------------------
int LoadSpace::GetWave()
{
	return g_nMaxWave;
}
