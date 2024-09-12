//=============================================================================
//
// エネミー処理 [enemy.cpp]
// Author : 
//
//=============================================================================
#include "enemy.h"
#include "bg.h"
#include "player.h"
#include "fade.h"
#include "collision.h"
#include "bullet.h"
#include "score.h"
#include "effect.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(200/2)	// キャラサイズ
#define TEXTURE_HEIGHT				(200/2)	// 
#define TEXTURE_MAX					(3)		// テクスチャの数

#define TEXTURE_PATTERN_DIVIDE_X	(1)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define ANIM_WAIT					(4)		// アニメーションの切り替わるWait値


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報 

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/enemy00.png",
	"data/TEXTURE/bar_white.png",
};


static BOOL		g_Load = FALSE;			// 初期化を行ったかのフラグ
static ENEMY	g_Enemy[ENEMY_MAX];		// エネミー構造体

static int		g_EnemyCnt = ENEMY_MAX;

static int		g_KillCnt = 0;

static INTERPOLATION_DATA g_MoveTbl0[] = {
	//座標								回転率						拡大率					   時間
	{ XMFLOAT3(50.0f,  50.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
	{ XMFLOAT3(250.0f,  50.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 3.14f),XMFLOAT3(0.0f, 0.0f, 1.0f),	60 },
	{ XMFLOAT3(250.0f, 250.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 6.28f),XMFLOAT3(2.0f, 2.0f, 1.0f),	60 },
};


static INTERPOLATION_DATA g_MoveTbl1[] = {
	//座標									  回転率						拡大率					   時間
	{ XMFLOAT3(1700.0f,   0.0f, 0.0f),		  XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
	{ XMFLOAT3(1700.0f,  SCREEN_HEIGHT, 0.0f),XMFLOAT3(0.0f, 0.0f, 6.28f),	XMFLOAT3(2.0f, 2.0f, 1.0f),	60 },
};


static INTERPOLATION_DATA g_MoveTbl2[] = {
	//座標											回転率							拡大率					   時間
	{ XMFLOAT3(3000.0f, 100.0f, 0.0f),				XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
	{ XMFLOAT3(3000 + SCREEN_WIDTH, 100.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 6.28f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
};


static INTERPOLATION_DATA* g_MoveTblAdr[] =
{
	g_MoveTbl0,
	g_MoveTbl1,
	g_MoveTbl2,

};


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
	ID3D11Device *pDevice = GetDevice();

	//テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// エネミー構造体の初期化
	g_EnemyCnt = 0;
	g_KillCnt = 0;
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (i <= 20)
		{
			g_Enemy[i].use = TRUE;
		}
		else
		{
			g_Enemy[i].use = FALSE;
		}

		g_Enemy[i].pos = XMFLOAT3(-5000 + rand() % 10000, -5000 + rand() % 10000, 0.0f);	// 中心点から表示

		g_Enemy[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_Enemy[i].w = TEXTURE_WIDTH;
		g_Enemy[i].h = TEXTURE_HEIGHT;
		g_Enemy[i].texNo = 0;
		g_Enemy[i].type = 0;

		g_Enemy[i].countAnim = 0;
		g_Enemy[i].patternAnim = 0;

		g_Enemy[i].move = XMFLOAT3(4.0f, 0.0f, 0.0f);		// 移動量

		g_Enemy[i].time = 0.0f;			// 線形補間用のタイマーをクリア
		g_Enemy[i].tblNo = 0;			// 再生する行動データテーブルNoをセット
		g_Enemy[i].tblMax = 0;			// 再生する行動データテーブルのレコード数をセット

		g_EnemyCnt++;
	}

	// 0番だけ線形補間で動かしてみる
	g_Enemy[0].time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[0].tblNo = 0;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[0].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	// 1番だけ線形補間で動かしてみる
	g_Enemy[1].time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[1].tblNo = 1;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[1].tblMax = sizeof(g_MoveTbl1) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	// 2番だけ線形補間で動かしてみる
	g_Enemy[2].time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[2].tblNo = 2;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[2].tblMax = sizeof(g_MoveTbl2) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	g_EnemyCnt = 0;	// 生きてるエネミーの数

	for (int i = 0; i < ENEMY_MAX; i++)
	{

		PLAYER* player = GetPlayer();

		

		if (g_Enemy[i].use == FALSE)
		{

			g_Enemy[i].pos = XMFLOAT3(-10000 + rand() % 20000, -10000 + rand() % 20000, 0.0f);

			int ram = 0;

			ram = rand() % 2000;

			if (ram == 1)
			{
				g_Enemy[i].use = TRUE;
			}

		}

		// 生きてるエネミーだけ処理をする
		if (g_Enemy[i].use == TRUE)
		{

			// 地形との当たり判定用に座標のバックアップを取っておく
			XMFLOAT3 pos_old = g_Enemy[i].pos;

			// アニメーション  
			g_Enemy[i].countAnim += 1.0f;
			if (g_Enemy[i].countAnim > ANIM_WAIT)
			{
				g_Enemy[i].countAnim = 0.0f;
				// パターンの切り替え
				g_Enemy[i].patternAnim = (g_Enemy[i].patternAnim + 1) % ANIM_PATTERN_NUM;
			}			
			
			switch (g_Enemy[i].type)
			{
			case 0:
				break;
			}

			// エネミーからプレイヤーへのベクトルを計算
			XMFLOAT3 direction;
			direction.x = player[0].pos.x - g_Enemy[i].pos.x;
			direction.y = player[0].pos.y - g_Enemy[i].pos.y;
			direction.z = player[0].pos.z - g_Enemy[i].pos.z;

			// ベクトルの長さを計算
			float length = sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);

			// ベクトルを正規化
			if (length > 0.0f)
			{
				direction.x /= length;
				direction.y /= length;
				direction.z /= length;
			}

			// エネミーの移動量に方向ベクトルを掛けて移動
			g_Enemy[i].pos.x += direction.x * g_Enemy[i].move.x;
			g_Enemy[i].pos.y += direction.y * g_Enemy[i].move.x;
			g_Enemy[i].pos.z += direction.z * g_Enemy[i].move.x;


			// 移動が終わったらエネミーとの当たり判定
			{

				// エネミーの数分当たり判定を行う
				for (int j = 0; j < ENEMY_MAX; j++)
				{
					// 生きてるエネミーと当たり判定をする
					if (player[j].use == TRUE)
					{
						BOOL ans = CollisionBB(g_Enemy[i].pos, g_Enemy[i].w, g_Enemy[i].h,
							player[j].pos, player[j].w, player[j].h);
						// 当たっている？
						if (ans == TRUE)
						{
							// 当たった時の処理
						//	player[j].use = FALSE;	// デバッグで一時的に無敵にしておくか

						}
					}
				}
			}

			// 当たり判定処理
			{
				BULLET* bullet = GetBullet();

				// バレットの数分当たり判定を行う
				for (int j = 0; j < BULLET_MAX; j++)
				{
					// 生きてるバレットと当たり判定をする
					if (bullet[j].use == TRUE)
					{
						BOOL ans = CollisionBB(bullet[j].pos, bullet[j].w, bullet[j].h,
							g_Enemy[i].pos, g_Enemy[i].w, g_Enemy[i].h);
						// 当たっている？
						if (ans == TRUE)
						{
							// 当たった時の処理
							g_Enemy[i].use = FALSE;
							AddScore(100);

							// エフェクト発生
							SetEffect(g_Enemy[i].pos.x, g_Enemy[i].pos.y, 30);

							g_KillCnt++;
						}
					}
				}
			}

			g_EnemyCnt++;		// 生きてた敵の数
		}
	}


	// エネミー全滅チェック
	if (g_KillCnt >= 1)
	{
		SetFade(FADE_OUT, MODE_CLEAR);
	}

#ifdef _DEBUG	// デバッグ情報を表示する


#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	BG* bg = GetBG();

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_Enemy[i].use == TRUE)			// このエネミーが使われている？
		{									// Yes
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Enemy[i].texNo]);

			//エネミーの位置やテクスチャー座標を反映
			float px = g_Enemy[i].pos.x - bg->pos.x;	// エネミーの表示位置X
			float py = g_Enemy[i].pos.y - bg->pos.y;	// エネミーの表示位置Y
			float pw = g_Enemy[i].w;		// エネミーの表示幅
			float ph = g_Enemy[i].h;		// エネミーの表示高さ

			// アニメーション用
			//float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// テクスチャの幅
			//float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
			//float tx = (float)(g_Player[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// テクスチャの左上X座標
			//float ty = (float)(g_Player[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// テクスチャの左上Y座標

			float tw = 1.0f;	// テクスチャの幅
			float th = 1.0f;	// テクスチャの高さ
			float tx = 0.0f;	// テクスチャの左上X座標
			float ty = 0.0f;	// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				g_Enemy[i].rot.z);

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}


	// ゲージのテスト
	{
		// 下敷きのゲージ（枠的な物）
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

		//ゲージの位置やテクスチャー座標を反映
		float px = 600.0f;		// ゲージの表示位置X
		float py =  10.0f;		// ゲージの表示位置Y
		float pw = 300.0f;		// ゲージの表示幅
		float ph =  30.0f;		// ゲージの表示高さ

		float tw = 1.0f;	// テクスチャの幅
		float th = 1.0f;	// テクスチャの高さ
		float tx = 0.0f;	// テクスチャの左上X座標
		float ty = 0.0f;	// テクスチャの左上Y座標

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLTColor(g_VertexBuffer,
			px, py, pw, ph,
			tx, ty, tw, th,
			XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);


		// エネミーの数に従ってゲージの長さを表示してみる
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

		//ゲージの位置やテクスチャー座標を反映
		pw = pw * ((float)g_EnemyCnt / ENEMY_MAX);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLTColor(g_VertexBuffer,
			px, py, pw, ph,
			tx, ty, tw, th,
			XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);


	}




}


//=============================================================================
// Enemy構造体の先頭アドレスを取得
//=============================================================================
ENEMY* GetEnemy(void)
{
	return &g_Enemy[0];
}





