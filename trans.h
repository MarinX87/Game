//=============================================================================
//
// �G�t�F�N�g���� [effect.h]
// Author :
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"
#include "sprite.h"

// �}�N����`
// #define EFFECT_TEXTURE			_T("data/TEXTURE/bomb.png")	// �摜

#define EFFECT_NUM_EFFECTS 1 // �G�t�F�N�g�ő吔

//*****************************************************************************
// �\���̐錾
//*****************************************************************************

typedef struct // �G�t�F�N�g�\����
{
	int use;
	int isEnding;
	bool isRemoveOnFinish;

	XMFLOAT3 pos; // �|���S���̈ړ���

	int duration;
	int elapsed;
	int numFinish;

	int effectCount;
	int emitCounter;
} TRANS;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTrans(void);
void UninitTrans(void);
void UpdateTrans(void);
void DrawTrans(void);
void SetTrans(float x, float y, int duration);
