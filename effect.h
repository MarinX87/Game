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
//#define EFFECT_TEXTURE			_T("data/TEXTURE/bomb.png")	// �摜

#define EFFECT_NUM_PARTS 30		//�G�t�F�N�g���p�[�e�B�N����
#define EFFECT_NUM_EFFECTS 10	//�G�t�F�N�g�ő吔


//*****************************************************************************
// �\���̐錾
//*****************************************************************************

typedef struct	// �G�t�F�N�g�\����
{
	XMFLOAT3		pos;						// �|���S���̈ړ���
	XMFLOAT3		move;						// �ړ���
	int				PatternAnim;				// �A�j���[�V�����p�^�[���i���o�[
	int				CountAnim;					// �A�j���[�V�����J�E���g

	int				liveTime;

	bool			isFinish;
}PARTICLE;

typedef struct	// �G�t�F�N�g�\����
{
	int				use;
	int				isEnding;
	bool			isRemoveOnFinish;

	XMFLOAT3		pos;						// �|���S���̈ړ���

	int				duration;
	int				elapsed;
	int				numFinish;

	int				effectCount;
	int				emitCounter;

	PARTICLE		pParticle[EFFECT_NUM_PARTS];

}EFFECT;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(float x, float y, int duration);
