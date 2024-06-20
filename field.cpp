//=============================================================================
//
// フィールド表示処理 [field.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "field.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX		(1)						// テクスチャの数

#define MAX_POLYGON		(6)						// キューブ１個あたりの面数

#define	VALUE_MOVE		(5.0f)					// 移動量
#define	VALUE_ROTATE	(XM_PI * 0.02f)			// 回転量

#define	SIZE_WH			(100.0f)				// 地面のサイズ

#define FIELD_SIZE		(3)						// 
#define MY_PI			(3.1415926)
#define MC				(1);
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexField(void);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;	// 頂点情報
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

#ifdef _DEBUG
static FIELD						g_Field[FIELD_SIZE];				// ポリゴンデータ
#else 
static FIELD						g_Field;				// ポリゴンデータ
#endif
static int							g_TexNo;				// テクスチャ番号

static char* g_TextureName[] = {
	//"data/TEXTURE/aaa.png",
	"data/TEXTURE/field000.jpg",
};

// 頂点配列
static VERTEX_3D g_VertexArray[4 * MAX_POLYGON] = {
	// ３Ｄ座標							頂点の向き						ＲＧＢＡ					ＴＥＸ座標
{XMFLOAT3(SIZE_WH, -SIZE_WH,  SIZE_WH),  XMFLOAT3(0.0f, 1.0f, 0.0f),XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f)},
{XMFLOAT3(-SIZE_WH, -SIZE_WH,  SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f),XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)},
{XMFLOAT3(SIZE_WH, -SIZE_WH, -SIZE_WH),  XMFLOAT3(0.0f, 1.0f, 0.0f),XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)},
{XMFLOAT3(-SIZE_WH, -SIZE_WH, -SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f),XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)},//下

{XMFLOAT3(-SIZE_WH, SIZE_WH,  SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.7f, 0.3f, 0.3f, 1.0f), XMFLOAT2(0.0f, 0.0f)},
{XMFLOAT3(SIZE_WH, SIZE_WH,  SIZE_WH),  XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.7f, 0.3f, 0.3f, 1.0f), XMFLOAT2(1.0f, 0.0f)},
{XMFLOAT3(-SIZE_WH, SIZE_WH, -SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.7f, 0.3f, 0.3f, 1.0f), XMFLOAT2(0.0f, 1.0f)},
{XMFLOAT3(SIZE_WH, SIZE_WH, -SIZE_WH),  XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.7f, 0.3f, 0.3f, 1.0f), XMFLOAT2(1.0f, 1.0f)},//上

{XMFLOAT3(SIZE_WH,  SIZE_WH, -SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)},
{XMFLOAT3(SIZE_WH,  SIZE_WH,  SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f)},
{XMFLOAT3(SIZE_WH, -SIZE_WH, -SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)},
{XMFLOAT3(SIZE_WH, -SIZE_WH,  SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)},//右

{XMFLOAT3(-SIZE_WH,  SIZE_WH,  SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f),XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f)},
{XMFLOAT3(-SIZE_WH,  SIZE_WH, -SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f),XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)},
{XMFLOAT3(-SIZE_WH, -SIZE_WH,  SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f),XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)},
{XMFLOAT3(-SIZE_WH, -SIZE_WH, -SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f),XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)},//左

{XMFLOAT3(-SIZE_WH,  SIZE_WH, -SIZE_WH),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 0.5f, 0.796f, 1.0f), XMFLOAT2(1.0f, 0.0f)},
{XMFLOAT3(SIZE_WH,  SIZE_WH, -SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 0.5f, 0.796f, 1.0f), XMFLOAT2(0.0f, 0.0f)},
{XMFLOAT3(-SIZE_WH, -SIZE_WH, -SIZE_WH),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 0.5f, 0.796f, 1.0f), XMFLOAT2(1.0f, 1.0f)},
{XMFLOAT3(SIZE_WH, -SIZE_WH, -SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 0.5f, 0.796f, 1.0f), XMFLOAT2(0.0f, 1.0f)},//前

{XMFLOAT3(SIZE_WH,  SIZE_WH,  SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f), XMFLOAT2(0.0f, 0.0f)},
{XMFLOAT3(-SIZE_WH,  SIZE_WH, SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f), XMFLOAT2(1.0f, 0.0f)},
{XMFLOAT3(SIZE_WH, -SIZE_WH,  SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f), XMFLOAT2(0.0f, 1.0f)},
{XMFLOAT3(-SIZE_WH, -SIZE_WH, SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f), XMFLOAT2(1.0f, 1.0f)},//後


};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitField(void)
{
	// 頂点バッファの作成
	MakeVertexField();

	// テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}
#ifdef _DEBUG
	for (int i = 0; i < FIELD_SIZE; i++)
	{
		// 位置・回転・スケールの初期設定
		g_Field[i].pos = XMFLOAT3(-200.0f, 0.0f, 0.0f);
		g_Field[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Field[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
	}
#else
	float scl = 0.5f;
	g_Field.pos = XMFLOAT3(0.0f, -100.0f, 0.0f);
	g_Field.rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_Field.scl = XMFLOAT3(scl, scl, scl);
#endif

	g_TexNo = 0;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitField(void)
{
	// 頂点バッファの解放
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	// テクスチャの解放
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateField(void)
{
	//CAMERA* cam = GetCamera();

#ifdef _DEBUG
	//for (int i = 0; i < FIELD_SIZE; i++)
	//{
	//	// 回転させちゃう
	//	if (GetKeyboardPress(DIK_LEFT))
	//	{
	//		g_Field[i].rot.y -= VALUE_ROTATE;
	//	}
	//	if (GetKeyboardPress(DIK_RIGHT))
	//	{
	//		g_Field[i].rot.y += VALUE_ROTATE;
	//	}
	//	if (GetKeyboardPress(DIK_UP))
	//	{
	//		g_Field[i].rot.x -= VALUE_ROTATE;
	//	}
	//	if (GetKeyboardPress(DIK_DOWN))
	//	{
	//		g_Field[i].rot.x += VALUE_ROTATE;
	//	}

	//	if (GetKeyboardPress(DIK_R))
	//	{
	//		g_Field[i].rot.y = g_Field[i].rot.x = 0.0f;
	//	}
	//}
#else 
	// 回転させちゃう
	if (GetKeyboardPress(DIK_LEFT))
	{
		g_Field.rot.y -= VALUE_ROTATE;
	}
	if (GetKeyboardPress(DIK_RIGHT))
	{
		g_Field.rot.y += VALUE_ROTATE;
	}
	if (GetKeyboardPress(DIK_UP))
	{
		g_Field.rot.x -= VALUE_ROTATE;
	}
	if (GetKeyboardPress(DIK_DOWN))
	{
		g_Field.rot.x += VALUE_ROTATE;
	}

	if (GetKeyboardPress(DIK_R))
	{
		g_Field.rot.y = g_Field.rot.x = 0.0f;
	}
#endif 


#ifdef _DEBUG	// デバッグ情報を表示する

	PrintDebugProc("Field:↑→↓←\n");

#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawField(void)
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);


	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// ワールドマトリックスの初期化
#ifdef _DEBUG
	for (int i = 0; i < FIELD_SIZE; i++)
	{
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Field[i].scl.x, g_Field[i].scl.y, g_Field[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Field[i].rot.x, g_Field[i].rot.y, g_Field[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Field[i].pos.x + 250.0f * i, g_Field[i].pos.y, g_Field[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Field[i].mtxWorld, mtxWorld);
		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);		// 4頂点分を0番目の頂点番号から描画

		GetDeviceContext()->Draw(4, 4);

		GetDeviceContext()->Draw(4, 8);

		GetDeviceContext()->Draw(4, 12);

		GetDeviceContext()->Draw(4, 16);

		GetDeviceContext()->Draw(4, 20);
	}
#elif MC
	{
		{
			mtxWorld = XMMatrixIdentity();

			// スケールを反映
			mtxScl = XMMatrixScaling(g_Field.scl.x, g_Field.scl.y, g_Field.scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(g_Field.rot.x, g_Field.rot.y, g_Field.rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_Field.pos.x - 100.0f, g_Field.pos.y, g_Field.pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Field.mtxWorld, mtxWorld);
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);		// 4頂点分を0番目の頂点番号から描画

			GetDeviceContext()->Draw(4, 4);

			GetDeviceContext()->Draw(4, 8);

			GetDeviceContext()->Draw(4, 12);

			GetDeviceContext()->Draw(4, 16);

			GetDeviceContext()->Draw(4, 20);
		}

		{
			mtxWorld = XMMatrixIdentity();

			// スケールを反映
			mtxScl = XMMatrixScaling(g_Field.scl.x, g_Field.scl.y, g_Field.scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(g_Field.rot.x, g_Field.rot.y, g_Field.rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_Field.pos.x, g_Field.pos.y, g_Field.pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Field.mtxWorld, mtxWorld);
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);		// 4頂点分を0番目の頂点番号から描画

			GetDeviceContext()->Draw(4, 4);

			GetDeviceContext()->Draw(4, 8);

			GetDeviceContext()->Draw(4, 12);

			GetDeviceContext()->Draw(4, 16);

			GetDeviceContext()->Draw(4, 20);
		}

		{
			mtxWorld = XMMatrixIdentity();

			// スケールを反映
			mtxScl = XMMatrixScaling(g_Field.scl.x, g_Field.scl.y, g_Field.scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(g_Field.rot.x, g_Field.rot.y, g_Field.rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_Field.pos.x + 100.0f, g_Field.pos.y, g_Field.pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Field.mtxWorld, mtxWorld);
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);		// 4頂点分を0番目の頂点番号から描画

			GetDeviceContext()->Draw(4, 4);

			GetDeviceContext()->Draw(4, 8);

			GetDeviceContext()->Draw(4, 12);

			GetDeviceContext()->Draw(4, 16);

			GetDeviceContext()->Draw(4, 20);
		}

		{
			mtxWorld = XMMatrixIdentity();

			// スケールを反映
			mtxScl = XMMatrixScaling(g_Field.scl.x, g_Field.scl.y, g_Field.scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(g_Field.rot.x, g_Field.rot.y, g_Field.rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_Field.pos.x + 100.0f, g_Field.pos.y+100.0f, g_Field.pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Field.mtxWorld, mtxWorld);
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);		// 4頂点分を0番目の頂点番号から描画

			GetDeviceContext()->Draw(4, 4);

			GetDeviceContext()->Draw(4, 8);

			GetDeviceContext()->Draw(4, 12);

			GetDeviceContext()->Draw(4, 16);

			GetDeviceContext()->Draw(4, 20);
		}

		{
			mtxWorld = XMMatrixIdentity();

			// スケールを反映
			mtxScl = XMMatrixScaling(g_Field.scl.x, g_Field.scl.y, g_Field.scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(g_Field.rot.x, g_Field.rot.y, g_Field.rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_Field.pos.x - 100.0f, g_Field.pos.y+100.0f, g_Field.pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Field.mtxWorld, mtxWorld);
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);		// 4頂点分を0番目の頂点番号から描画

			GetDeviceContext()->Draw(4, 4);

			GetDeviceContext()->Draw(4, 8);

			GetDeviceContext()->Draw(4, 12);

			GetDeviceContext()->Draw(4, 16);

			GetDeviceContext()->Draw(4, 20);
		}

		{
			mtxWorld = XMMatrixIdentity();

			// スケールを反映
			mtxScl = XMMatrixScaling(g_Field.scl.x, g_Field.scl.y, g_Field.scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(g_Field.rot.x, g_Field.rot.y, g_Field.rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_Field.pos.x, g_Field.pos.y + 100.0f, g_Field.pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Field.mtxWorld, mtxWorld);
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);		// 4頂点分を0番目の頂点番号から描画

			GetDeviceContext()->Draw(4, 4);

			GetDeviceContext()->Draw(4, 8);

			GetDeviceContext()->Draw(4, 12);

			GetDeviceContext()->Draw(4, 16);

			GetDeviceContext()->Draw(4, 20);
		}

		{
			mtxWorld = XMMatrixIdentity();

			// スケールを反映
			mtxScl = XMMatrixScaling(g_Field.scl.x, g_Field.scl.y, g_Field.scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(g_Field.rot.x, g_Field.rot.y, g_Field.rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_Field.pos.x, g_Field.pos.y + 200.0f, g_Field.pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Field.mtxWorld, mtxWorld);
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);		// 4頂点分を0番目の頂点番号から描画

			GetDeviceContext()->Draw(4, 4);

			GetDeviceContext()->Draw(4, 8);

			GetDeviceContext()->Draw(4, 12);

			GetDeviceContext()->Draw(4, 16);

			GetDeviceContext()->Draw(4, 20);
		}

		{
			mtxWorld = XMMatrixIdentity();

			// スケールを反映
			mtxScl = XMMatrixScaling(g_Field.scl.x, g_Field.scl.y, g_Field.scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(g_Field.rot.x, g_Field.rot.y, g_Field.rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_Field.pos.x-100.0f, g_Field.pos.y + 200.0f, g_Field.pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Field.mtxWorld, mtxWorld);
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);		// 4頂点分を0番目の頂点番号から描画

			GetDeviceContext()->Draw(4, 4);

			GetDeviceContext()->Draw(4, 8);

			GetDeviceContext()->Draw(4, 12);

			GetDeviceContext()->Draw(4, 16);

			GetDeviceContext()->Draw(4, 20);
		}

		{
			mtxWorld = XMMatrixIdentity();

			// スケールを反映
			mtxScl = XMMatrixScaling(g_Field.scl.x, g_Field.scl.y, g_Field.scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(g_Field.rot.x, g_Field.rot.y, g_Field.rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_Field.pos.x+100.0f, g_Field.pos.y + 200.0f, g_Field.pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Field.mtxWorld, mtxWorld);
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);		// 4頂点分を0番目の頂点番号から描画

			GetDeviceContext()->Draw(4, 4);

			GetDeviceContext()->Draw(4, 8);

			GetDeviceContext()->Draw(4, 12);

			GetDeviceContext()->Draw(4, 16);

			GetDeviceContext()->Draw(4, 20);
		}


		{
			mtxWorld = XMMatrixIdentity();

			// スケールを反映
			mtxScl = XMMatrixScaling(g_Field.scl.x*0.5f, g_Field.scl.y * 0.5f, g_Field.scl.z * 0.5f);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(g_Field.rot.x- MY_PI*0.5f, g_Field.rot.y, g_Field.rot.z+ MY_PI);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_Field.pos.x + 124.0f, g_Field.pos.y + 125.0f, g_Field.pos.z-50.0f);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Field.mtxWorld, mtxWorld);
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);		// 4頂点分を0番目の頂点番号から描画

			GetDeviceContext()->Draw(4, 4);

			GetDeviceContext()->Draw(4, 8);

			GetDeviceContext()->Draw(4, 12);

			GetDeviceContext()->Draw(4, 16);

			GetDeviceContext()->Draw(4, 20);
		}

		{
			mtxWorld = XMMatrixIdentity();

			// スケールを反映
			mtxScl = XMMatrixScaling(g_Field.scl.x * 0.5f, g_Field.scl.y * 0.5f, g_Field.scl.z * 0.5f);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(g_Field.rot.x + MY_PI * 0.5f, g_Field.rot.y, g_Field.rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_Field.pos.x - 124.0f, g_Field.pos.y + 125.0f, g_Field.pos.z - 50.0f);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Field.mtxWorld, mtxWorld);
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);		// 4頂点分を0番目の頂点番号から描画

			GetDeviceContext()->Draw(4, 4);

			GetDeviceContext()->Draw(4, 8);

			GetDeviceContext()->Draw(4, 12);

			GetDeviceContext()->Draw(4, 16);

			GetDeviceContext()->Draw(4, 20);
		}

		{
			mtxWorld = XMMatrixIdentity();

			// スケールを反映
			mtxScl = XMMatrixScaling(g_Field.scl.x, g_Field.scl.y, g_Field.scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(g_Field.rot.x, g_Field.rot.y, g_Field.rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_Field.pos.x + 50.0f, g_Field.pos.y + 50.0f, g_Field.pos.z-40.0f);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Field.mtxWorld, mtxWorld);
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);		// 4頂点分を0番目の頂点番号から描画

			GetDeviceContext()->Draw(4, 4);

			GetDeviceContext()->Draw(4, 8);

			GetDeviceContext()->Draw(4, 12);

			GetDeviceContext()->Draw(4, 16);

			GetDeviceContext()->Draw(4, 20);
		}

		{
			mtxWorld = XMMatrixIdentity();

			// スケールを反映
			mtxScl = XMMatrixScaling(g_Field.scl.x, g_Field.scl.y, g_Field.scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(g_Field.rot.x, g_Field.rot.y, g_Field.rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_Field.pos.x - 50.0f, g_Field.pos.y + 50.0f, g_Field.pos.z-40.0f);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Field.mtxWorld, mtxWorld);
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);		// 4頂点分を0番目の頂点番号から描画

			GetDeviceContext()->Draw(4, 4);

			GetDeviceContext()->Draw(4, 8);

			GetDeviceContext()->Draw(4, 12);

			GetDeviceContext()->Draw(4, 16);

			GetDeviceContext()->Draw(4, 20);
		}

		{
			mtxWorld = XMMatrixIdentity();

			// スケールを反映
			mtxScl = XMMatrixScaling(g_Field.scl.x * 0.5f, g_Field.scl.y * 0.5f, g_Field.scl.z * 0.5f);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(g_Field.rot.x- 0.5f * MY_PI, g_Field.rot.y, g_Field.rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_Field.pos.x - 50.0f, g_Field.pos.y + 50.0f, g_Field.pos.z - 75.0f);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Field.mtxWorld, mtxWorld);
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);		// 4頂点分を0番目の頂点番号から描画

			GetDeviceContext()->Draw(4, 4);

			GetDeviceContext()->Draw(4, 8);

			GetDeviceContext()->Draw(4, 12);

			GetDeviceContext()->Draw(4, 16);

			GetDeviceContext()->Draw(4, 20);
		}

		{
			mtxWorld = XMMatrixIdentity();

			// スケールを反映
			mtxScl = XMMatrixScaling(g_Field.scl.x * 0.5f, g_Field.scl.y * 0.5f, g_Field.scl.z * 0.5f);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(g_Field.rot.x-0.5f*MY_PI, g_Field.rot.y, g_Field.rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_Field.pos.x + 50.0f, g_Field.pos.y + 50.0f, g_Field.pos.z - 75.0f);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Field.mtxWorld, mtxWorld);
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);		// 4頂点分を0番目の頂点番号から描画

			GetDeviceContext()->Draw(4, 4);

			GetDeviceContext()->Draw(4, 8);

			GetDeviceContext()->Draw(4, 12);

			GetDeviceContext()->Draw(4, 16);

			GetDeviceContext()->Draw(4, 20);
		}
	}
#else
	{
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Field.scl.x, g_Field.scl.y, g_Field.scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Field.rot.x, g_Field.rot.y, g_Field.rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Field.pos.x - 250.0f, g_Field.pos.y, g_Field.pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Field.mtxWorld, mtxWorld);
		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);		// 4頂点分を0番目の頂点番号から描画

		GetDeviceContext()->Draw(4, 4);

		GetDeviceContext()->Draw(4, 8);

		GetDeviceContext()->Draw(4, 12);

		GetDeviceContext()->Draw(4, 16);

		GetDeviceContext()->Draw(4, 20);
	}
	{
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Field.scl.x, g_Field.scl.y, g_Field.scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Field.rot.x, g_Field.rot.y, g_Field.rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Field.pos.x + 250.0f, g_Field.pos.y, g_Field.pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Field.mtxWorld, mtxWorld);
		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);		// 4頂点分を0番目の頂点番号から描画

		GetDeviceContext()->Draw(4, 4);

		GetDeviceContext()->Draw(4, 8);

		GetDeviceContext()->Draw(4, 12);

		GetDeviceContext()->Draw(4, 16);

		GetDeviceContext()->Draw(4, 20);
	}
#endif
}



HRESULT MakeVertexField(void)
{
	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4 * MAX_POLYGON;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// グローバルの頂点配列から頂点バッファをつくる
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = g_VertexArray;

	GetDevice()->CreateBuffer(&bd, &sd, &g_VertexBuffer);

	return S_OK;
}