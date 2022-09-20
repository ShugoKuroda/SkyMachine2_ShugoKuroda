//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA, SHUGO KURODA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include <xaudio2.h>					// サウンド再生に必要

//-----------------------------------------------------------------------------
// サウンドクラス(基本クラス)
//-----------------------------------------------------------------------------
class CSound
{
public:
	CSound();
	~CSound();

	// サウンドファイル
	typedef enum
	{
		// BGM音
		SOUND_LABEL_TITLE = 0,			// タイトルBGM
		SOUND_LABEL_GAME,				// ゲームBGM
		SOUND_LABEL_RESULT,				// リザルトBGM
		SOUND_LABEL_BOSS,				// ボス戦BGM
		// ゲームSE音
		SOUND_LABEL_SE_ENTRY,			// 自機登場
		SOUND_LABEL_SE_SHOT,			// 弾発射
		SOUND_LABEL_SE_HIT,				// 弾ヒット
		SOUND_LABEL_SE_EXPLOSION,		// 爆発
		SOUND_LABEL_SE_EXPLOSION_BOSS,	// ボス爆破
		SOUND_LABEL_SE_DIE_PLAYER,		// プレイヤーやられ
		SOUND_LABEL_SE_DIE_BOSS,		// ボス撃破
		SOUND_LABEL_SE_ITEM,			// アイテム取得
		SOUND_LABEL_SE_CLEAR,			// ゲームクリア
		// UI関連SE音
		SOUND_LABEL_SE_MENU_IN,			// メニューイン
		SOUND_LABEL_SE_MENU_OUT,		// メニューアウト
		SOUND_LABEL_SE_MENU_SELECT,		// 選択
		SOUND_LABEL_SE_MENU_OK,			// 決定
		SOUND_LABEL_SE_CANCEL,			// キャンセル
		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	HRESULT Init(HWND hWnd);
	void Uninit();
	static HRESULT Play(SOUND_LABEL label);
	static void Stop(SOUND_LABEL label);
	static void Stop();

private:
	// パラメータ構造体定義
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	} SOUNDPARAM;

	static IXAudio2 *m_pXAudio2;									// XAudio2オブジェクトへのインターフェイス
	static IXAudio2MasteringVoice *m_pMasteringVoice;				// マスターボイス
	static IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	static BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータ
	static DWORD m_aSizeAudio[SOUND_LABEL_MAX];						// オーディオデータサイズ

	static SOUNDPARAM m_aParam[SOUND_LABEL_MAX];
};

#endif
