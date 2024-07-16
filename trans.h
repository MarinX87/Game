//=============================================================================
//
// エフェクト処理 [effect.h]
// Author :
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"
#include "sprite.h"

// マクロ定義
// #define EFFECT_TEXTURE			_T("data/TEXTURE/bomb.png")	// 画像

#define TRANS_NUM_PARTS 30	 // エフェクト内パーティクル数
#define TRANS_NUM_EFFECTS 10 // エフェクト最大数

//*****************************************************************************
// 構造体宣言
//*****************************************************************************

typedef struct // エフェクト構造体
{
	XMFLOAT3 pos;	 // ポリゴンの移動量
	XMFLOAT3 move;	 // 移動量
	int PatternAnim; // アニメーションパターンナンバー
	int CountAnim;	 // アニメーションカウント

	int liveTime;

	bool isFinish;
} TRANS_PARTICLE;

typedef struct // エフェクト構造体
{
	int use;
	int isEnding;
	bool isRemoveOnFinish;

	XMFLOAT3 pos; // ポリゴンの移動量

	int duration;
	int elapsed;
	int numFinish;

	int effectCount;
	int emitCounter;

	PARTICLE pParticle[TRANS_NUM_PARTS];

} TRANS_EFFECT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTrans(void);
void UninitTrans(void);
void UpdateTrans(void);
void DrawTrans(void);
void SetTrans(float x, float y, int duration);
