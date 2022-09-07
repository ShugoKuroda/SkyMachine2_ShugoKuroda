//===============================================
//
// フェード処理 (fade.cpp)
// Author : SHUGO KURODA
//
//===============================================
#include "fade.h"
#include "object.h"

//=======================================
// マクロ定義
//=======================================

//-----------------------------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------------------------
CFade::CFade() :m_type(TYPE_NONE), m_nCounter(0)
{
	//タイプの設定
	CObject::SetObjType(CObject::OBJ_FADE);
}

//-----------------------------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------------------------
CFade::~CFade()
{

}

//-----------------------------------------------------------------------------------------------
// 生成
//-----------------------------------------------------------------------------------------------
CFade *CFade::Create(FADETYPE type)
{
	CFade *pFade = new CFade;

	if (pFade != nullptr)
	{
		// フェード種類の設定
		pFade->m_type = type;

		// 初期化
		pFade->Init();
	}

	return pFade;
}

//-----------------------------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------------------------
HRESULT CFade::Init()
{
	//位置の設定
	CObject2D::SetPosition(D3DXVECTOR3(CRenderer::SCREEN_WIDTH / 2, CRenderer::SCREEN_HEIGHT / 2, 0.0f));

	//サイズの設定
	CObject2D::SetSize(D3DXVECTOR2((float)CRenderer::SCREEN_WIDTH, (float)CRenderer::SCREEN_HEIGHT));

	// 初期化
	CObject2D::Init();

	//色の設定
	if (m_type == TYPE_WHITE)
	{
		CObject2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else if (m_type == TYPE_BLACK)
	{
		CObject2D::SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	}

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------------------------
void CFade::Uninit()
{
	// 終了処理
	CObject2D::Uninit();
}

//-----------------------------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------------------------
void CFade::Update()
{
	// 変数宣言
	D3DXVECTOR3 pos = GetPosition();	// 位置を取得
	D3DXVECTOR3 size = GetSize();		// サイズを取得
	D3DXCOLOR col = GetColor();			// 色を取得

	// フェードの挙動を場合分け
	if (m_type == TYPE_WHITE)
	{//白フェードの場合
		col.a -= 0.025f;		//ポリゴンを透明にしていく

		//ポリゴンが透明になったら
		if (col.a <= 0.0f)
		{//破棄
			Uninit();
			return;
		}
	}
	else if (m_type == TYPE_BLACK)
	{//黒フェードの場合
		col.a += 0.01f;			//ポリゴンを暗くしていく

		//フェード解除カウンターが一定数以上
		if (m_nCounter >= 360)
		{
			//カウンターを止める
			m_nCounter = 360;
			//ポリゴンを透明にしていく
			col.a -= 0.02f;		

			//ポリゴンが透明になったら
			if (col.a <= 0.0f)
			{//破棄
				Uninit();
				return;
			}
		}
		// ポリゴンが半透明になったら
		else if (col.a >= 0.5f)
		{
			// α値の加算を止める
			col.a = 0.5f;
			// フェードを解除するまでのカウンター加算
			m_nCounter++;
		}
	}

	//色の設定
	SetColor(col);
}

//-----------------------------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------------------------
void CFade::Draw()
{
	CObject2D::Draw();
}
