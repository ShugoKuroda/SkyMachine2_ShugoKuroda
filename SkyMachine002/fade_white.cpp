//===============================================
//
// 白フェード処理 (whiteFade.cpp)
// Author : SHUGO KURODA
//
//===============================================
#include "fade_white.h"
#include "object.h"

//=======================================
// マクロ定義
//=======================================

//-----------------------------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------------------------
CFadeWhite::CFadeWhite()
{
	//タイプの設定
	CObject::SetObjType(CObject::OBJ_FADE_WHITE);
}

//-----------------------------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------------------------
CFadeWhite::~CFadeWhite()
{

}

//-----------------------------------------------------------------------------------------------
// 生成
//-----------------------------------------------------------------------------------------------
CFadeWhite *CFadeWhite::Create()
{
	CFadeWhite *pFadeWhite = new CFadeWhite;

	if (pFadeWhite != nullptr)
	{
		// 初期化
		pFadeWhite->Init();
	}

	return pFadeWhite;
}

//-----------------------------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------------------------
HRESULT CFadeWhite::Init()
{
	//位置の設定
	CObject2D::SetPosition(D3DXVECTOR3(CRenderer::SCREEN_WIDTH / 2, CRenderer::SCREEN_HEIGHT / 2, 0.0f));

	//サイズの設定
	CObject2D::SetSize(D3DXVECTOR2((float)CRenderer::SCREEN_WIDTH, (float)CRenderer::SCREEN_HEIGHT));

	// 初期化
	CObject2D::Init();

	//色の設定
	CObject2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------------------------
void CFadeWhite::Uninit()
{
	// 終了処理
	CObject2D::Uninit();
}

//-----------------------------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------------------------
void CFadeWhite::Update()
{
	// 変数宣言
	D3DXVECTOR3 pos = GetPosition();	// 位置を取得
	D3DXVECTOR3 size = GetSize();		// サイズを取得
	D3DXCOLOR col = GetColor();			// 色を取得

	col.a -= 0.025f;		//ポリゴンを透明にしていく

	//ポリゴンが透明になったら
	if (col.a <= 0.0f)
	{//破棄
		Uninit();
		return;
	}

	//色の設定
	SetColor(col);
}

//========================================
// 白フェードの描画処理
//========================================
void CFadeWhite::Draw()
{
	CObject2D::Draw();
}
