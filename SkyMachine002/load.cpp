//=============================================================================
//
// ���[�h���� [load.cpp]
// Author : SHUGO KURODA
//
//=============================================================================
#include <stdio.h>
#include "load.h"
#include "enemy_data.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_CHAR	(256)			//��s�̍ő�ǂݎ�蕶����

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
EnemyInfo g_aEnemyInfo[MAX_WAVE];	//�G���̎擾
int g_nMaxWave;

//-----------------------------------------------------------------------------
//	�G�̃Z�b�g���[�h
//-----------------------------------------------------------------------------
HRESULT LoadSpace::LoadEnemy(HWND hWnd)
{
	FILE *pFile = NULL;		//�t�@�C���|�C���^�[�錾

	ZeroMemory(&g_aEnemyInfo, sizeof(g_aEnemyInfo));	//������
	int nIdxEnemy = 0;		//�G���̔z��ԍ��̕ۑ��p

	char cScanf[MAX_CHAR];		//��s���ǂݎ�邽�߂̕ϐ�
	char cScanfHead[MAX_CHAR];	//���̕�����ǂݎ�邽�߂̕ϐ�
	bool bReadScript = false;	//�X�N���v�g��ǂݍ��ނ��ǂ���

	//�t�@�C�����J��
	pFile = fopen("data/TEXT/set_enemy.txt", "r");

	if (pFile == NULL)
	{//�t�@�C�����J���Ȃ������ꍇ
		MessageBox(hWnd, "�G�z�u�e�L�X�g���J���܂���ł���", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	//������̓ǂݎ�胋�[�v����
	while (fgets(cScanf, MAX_CHAR, pFile) != NULL)
	{
		//������̕���
		sscanf(cScanf, "%s", &cScanfHead);

		if (!bReadScript && strcmp(&cScanfHead[0], "SCRIPT") == 0)
		{//�X�N���v�g�̓ǂݍ��݂��J�n
			bReadScript = true;
		}
		else if (strcmp(&cScanfHead[0], "MAX_WAVE") == 0)
		{//�ő�E�F�[�u��
			sscanf(cScanf, "%s = %d", &cScanfHead, &g_nMaxWave);
		}
		else if (bReadScript)
		{//SCRIPT�ǂݍ��݂��J�n������
			if (strcmp(&cScanfHead[0], "ENEMYSET") == 0)
			{//�G�̔z�u���J�n

				//�v���C���[���̓ǂݎ�胋�[�v����
				while (fgets(cScanf, MAX_CHAR, pFile) != NULL)
				{
					//������̕���
					sscanf(cScanf, "%s", &cScanfHead);

					if (strcmp(&cScanfHead[0], "MAXMOVE") == 0)
					{//�ő�ړ���
						sscanf(cScanf, "%s = %d", &cScanfHead, &g_aEnemyInfo[nIdxEnemy].nMaxMove);
					}
					else if (strcmp(&cScanfHead[0], "FRAME") == 0)
					{//�o���t���[��
						sscanf(cScanf, "%s = %d", &cScanfHead, &g_aEnemyInfo[nIdxEnemy].nFrame);
					}
					else if (strcmp(&cScanfHead[0], "POS") == 0)
					{//�o���ʒu
						sscanf(cScanf, "%s = %f%f%f", &cScanfHead, &g_aEnemyInfo[nIdxEnemy].pos.x, &g_aEnemyInfo[nIdxEnemy].pos.y, &g_aEnemyInfo[nIdxEnemy].pos.z);
					}
					else if (strcmp(&cScanfHead[0], "TYPE") == 0)
					{//�G�̎��
						sscanf(cScanf, "%s = %d", &cScanfHead, &g_aEnemyInfo[nIdxEnemy].nType);
					}
					else if (strcmp(&cScanfHead[0], "LIFE") == 0)
					{//�G�̃��C�t
						sscanf(cScanf, "%s = %d", &cScanfHead, &g_aEnemyInfo[nIdxEnemy].nLife);
					}
					else if (strcmp(&cScanfHead[0], "NUM") == 0)
					{//�G�̐�
						sscanf(cScanf, "%s = %d", &cScanfHead, &g_aEnemyInfo[nIdxEnemy].nNum);
					}
					else if (strcmp(&cScanfHead[0], "INTERVAL") == 0)
					{//�G�̐�
						sscanf(cScanf, "%s = %d", &cScanfHead, &g_aEnemyInfo[nIdxEnemy].nInterval);
					}
					else if (strcmp(&cScanfHead[0], "MOVESET") == 0)
					{
						int nIdxMove = 0;
						//�v���C���[�p�[�c���̓ǂݎ�胋�[�v����
						while (fgets(cScanf, MAX_CHAR, pFile) != NULL)
						{
							//������̕���
							sscanf(cScanf, "%s", &cScanfHead);

							if (strcmp(&cScanfHead[0], "MOVE") == 0)
							{//�ړ��ʒu�̐ݒ�
								sscanf(cScanf, "%s = %f%f%d", &cScanfHead,
									&g_aEnemyInfo[nIdxEnemy].move[nIdxMove].posEnd.x,
									&g_aEnemyInfo[nIdxEnemy].move[nIdxMove].posEnd.y,
									&g_aEnemyInfo[nIdxEnemy].move[nIdxMove].nFrameMove);
								//�z��ԍ��̉��Z
								nIdxMove++;
							}
							else if (strcmp(&cScanfHead[0], "END_MOVESET") == 0)
							{//�p�[�c�ǂݍ��ݏI��
								break;
							}
						}
					}
					else if (strcmp(&cScanfHead[0], "END_ENEMYSET") == 0)
					{//�v���C���[�ǂݍ��ݏI��
						nIdxEnemy++;
						break;
					}
				}
			}
		}
		else if (strcmp(&cScanfHead[0], "END_SCRIPT") == 0)
		{//�e�L�X�g�t�@�C����ǂݐ؂�����
			break;
		}
	}

	//�t�@�C�������
	fclose(pFile);

	return S_OK;
}

//-----------------------------------------------------------------------------
//	�G�̔z�u���̎擾
//-----------------------------------------------------------------------------
EnemyInfo *LoadSpace::GetEnemy()
{
	return &g_aEnemyInfo[0];
}

//-----------------------------------------------------------------------------
//	�ő�E�F�[�u���̎擾
//-----------------------------------------------------------------------------
int LoadSpace::GetWave()
{
	return g_nMaxWave;
}
