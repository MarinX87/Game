//=============================================================================
//
// HP処理 [hpbar.cpp]
// Author : 荒山　秀磨
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "score.h"
#include "sprite.h"
#include "player.h"
#include "hpbar.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(400)
#define TEXTURE_HEIGHT				(32) 
#define TEXTURE_MAX					(2)		// テクスチャの数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer* vertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView* texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char* texturName[] = {
	"data/TEXTURE/hp_bar.png",
	"data/TEXTURE/bar_base.png"
};

static BOOL						use;						// true:使っている  false:未使用
static float					w, h;					// 幅と高さ
static XMFLOAT3					pos;						// ポリゴンの座標
static int						texNo;					// テクスチャ番号

static int						hpbar;			    	// ヒットポイント

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitHpbar(void)
{
	ID3D11Device* pDevice = GetDevice();

	//テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			texturName[i],
			NULL,
			NULL,
			&texture[i],
			NULL);
	}


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &vertexBuffer);


	// バーの初期化
	use = TRUE;
	w = TEXTURE_WIDTH;
	h = TEXTURE_HEIGHT;
	pos = { 200.0f, 20.0f, 0.0f };
	texNo = 0;

	hpbar = 0;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitHpbar(void)
{
	if (vertexBuffer)
	{
		vertexBuffer->Release();
		vertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (texture[i])
		{
			texture[i]->Release();
			texture[i] = NULL;
		}
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateHpbar(void)
{


#ifdef _DEBUG	// デバッグ情報を表示する
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", pos.x, pos.y);

#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawHpbar(void)
{
	PLAYER * player = GetPlayer();

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &texture[1]);

	// HPの位置やテクスチャー座標を反映
	float px = pos.x;		// HPの表示位置X
	float py = pos.y;		// HPの表示位置Y
	float pw = w;			// HPの表示幅
	float ph = h;			// HPの表示高さ

	float tw = 1.0f;		// テクスチャの幅
	float th = 1.0f;		// テクスチャの高さ
	float tx = 0.0f;	    // テクスチャの左上X座標
	float ty = 0.0f;		// テクスチャの左上Y座標

	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColor(vertexBuffer, px, py, pw, ph, tx, ty, tw, th,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);

	// 頂点バッファ設定
	stride = sizeof(VERTEX_3D);
	offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &texture[0]);
	px = 12.0f + player[0].hp * 1.87f;	// HPの表示位置X
	pw = player[0].hp * 4.0f;			// HPの表示幅

	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColor(vertexBuffer, px, py, pw, ph, tx, ty, tw, th,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);
}
