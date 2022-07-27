//-----------------------------------------------------------------------------------------------
//
// 弾の処理[bullet.cpp]
// Author : SHUGO kURODA
//
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------------------------
#include "cloud.h"
#include "manager.h"
#include "renderer.h"

#include "library.h"

//-----------------------------------------------------------------------------------------------
// using宣言
//-----------------------------------------------------------------------------------------------
using namespace LibrarySpace;

//-----------------------------------------------------------------------------------------------
// 静的メンバ変数
//-----------------------------------------------------------------------------------------------
// テクスチャのポインタ
LPDIRECT3DTEXTURE9 CCloud::m_apTexture[MAX_TEX] = { nullptr };
float CCloud::m_fCreatePos = 0.0f;

//-----------------------------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------------------------
CCloud::CCloud() :
	m_move(0.0f, 0.0f, 0.0f)
{
	//オブジェクトタイプの設定
	SetObjectType(EObject::OBJ_CLOUD);
}

//-----------------------------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------------------------
CCloud::~CCloud()
{
}

//-----------------------------------------------------------------------------------------------
// 生成
//-----------------------------------------------------------------------------------------------
CCloud * CCloud::Create()
{
	// ポインタクラスを宣言
	CCloud* pCloud = new CCloud;

	if (pCloud != nullptr)
	{
		// 位置、サイズ、移動量を乱数で設定
		D3DXVECTOR3 pos;	//位置
		D3DXVECTOR2 size = SetRandSize();	//サイズの設定

		if (CRenderer::SCREEN_HEIGHT <= (int)m_fCreatePos + 300)
		{
			m_fCreatePos = (float)CRenderer::SCREEN_HEIGHT - 300;
		}

		// 位置を(画面の右端+雲のサイズ)に設定
		pos.x = (float)CRenderer::SCREEN_WIDTH + (size.x / 2);
		pos.y = (float)GetRandNum((CRenderer::SCREEN_HEIGHT - 300) - (int)m_fCreatePos, 0);
		pos.z = 0.0f;

		//位置設定
		pCloud->SetPosition(pos);
		//サイズ設定
		pCloud->SetSize(size);
		// 初期化
		pCloud->Init();

		// テクスチャの設定(ランダムでテクスチャを設定)
		pCloud->BindTexture(m_apTexture[GetRandNum(1, 0)]);
	}

	return pCloud;
}

//-----------------------------------------------------------------------------------------------
// 生成(引数あり)
//-----------------------------------------------------------------------------------------------
CCloud* CCloud::Create(D3DXVECTOR3 pos)
{
	// ポインタクラスを宣言
	CCloud* pCloud = new CCloud;

	if (pCloud != nullptr)
	{
		//位置設定
		pCloud->SetPosition(pos);
		//サイズ設定
		pCloud->SetSize(SetRandSize());
		// 初期化
		pCloud->Init();

		// テクスチャの設定(ランダムでテクスチャを設定)
		pCloud->BindTexture(m_apTexture[GetRandNum(1, 0)]);
	}

	return pCloud;
}

//-----------------------------------------------------------------------------------------------
// サイズをランダムに決める
//-----------------------------------------------------------------------------------------------
D3DXVECTOR2 CCloud::SetRandSize()
{
	D3DXVECTOR2 size;	//サイズ

	// サイズの設定
	size.x = (float)GetRandNum(DEFAULT_WIDTH, MAX_WIDTH);
	size.y = (float)GetRandNum(DEFAULT_HEIGHT, MAX_HEIGHT);

	return size;
}

//-----------------------------------------------------------------------------------------------
// テクスチャの読み込み
//-----------------------------------------------------------------------------------------------
HRESULT CCloud::Load()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/cloud000.png",
		&m_apTexture[0]);
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/cloud001.png",
		&m_apTexture[1]);

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// テクスチャの削除
//-----------------------------------------------------------------------------------------------
void CCloud::Unload()
{
	for (int nCnt = 0; nCnt < MAX_TEX; nCnt++)
	{
		// テクスチャの破棄
		if (m_apTexture[nCnt] != nullptr)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = nullptr;
		}
	}
}

//-----------------------------------------------------------------------------------------------
// 雲の移動処理
//-----------------------------------------------------------------------------------------------
void CCloud::MoveCloud(float fMove)
{
	//雲の移動処理
	D3DXVECTOR3 posCloud;	//位置格納用
	for (int nCntObject = 0; nCntObject < CObject::MAX_OBJECT; nCntObject++)
	{
		//オブジェクト情報の取得
		CObject *pObject = CObject::GetObject(nCntObject);
		if (pObject != nullptr)
		{
			//オブジェクトタイプの取得
			CObject::EObject objType = pObject->GetObjType();
			//オブジェクトタイプが雲だったら
			if (objType == CObject::OBJ_CLOUD)
			{
				//雲を上に移動する
				CObject2D *pCloud = (CObject2D*)pObject;
				posCloud = pCloud->GetPosition();
				posCloud.y += fMove;
				pCloud->SetPosition(posCloud);
				pCloud->SetVertex();
			}
		}
	}
}

//-----------------------------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------------------------
HRESULT CCloud::Init()
{
	// 移動量を乱数で設定(Xのみ)
	m_move.x = -(float)GetRandNum(MAX_MOVEING, DEFAULT_MOVE);
	// 初期化
	CObject2D::Init();

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------------------------
void CCloud::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------------------------
void CCloud::Update()
{
	// 位置の取得
	D3DXVECTOR3 pos = CObject2D::GetPosition();
	D3DXVECTOR2 size = CObject2D::GetSize();

	// 移動量の更新
	pos += m_move;

	if (OutScreen2D(&pos, size))
	{// ライフが0
		// 雲の破棄
		Uninit();
	}
	else
	{
		// 位置の更新
		CObject2D::SetPosition(pos);

		//頂点座標の設定
		CObject2D::SetVertex();
	}
}

//-----------------------------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------------------------
void CCloud::Draw()
{
	CObject2D::Draw();
}
