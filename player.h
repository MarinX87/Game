//=============================================================================
//
// �v���C���[���� [player.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PLAYER_MAX			(1)		// �v���C���[��Max�l��

#define	PLAYER_OFFSET_CNT	(16)	// 16���g

enum
{
	CHAR_DIR_UP,
	CHAR_DIR_RIGHT,
	CHAR_DIR_DOWN,
	CHAR_DIR_LEFT,

	CHAR_DIR_MAX
};

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

struct PLAYER
{
	XMFLOAT3	pos;			// �|���S���̍��W
	XMFLOAT3	rot;			// �|���S���̉�]��
	BOOL		use;			// true:�g���Ă���  false:���g�p
	float		w, h;			// ���ƍ���
	float		countAnim;		// �A�j���[�V�����J�E���g
	int			patternAnim;	// �A�j���[�V�����p�^�[���i���o�[
	int			texNo;			// �e�N�X�`���ԍ�
	
	int			dir;			// �����i0:�� 1:�E 2:�� 3:���j
	BOOL		moving;			// �ړ����t���O

	BOOL		dash;			// �_�b�V�����t���O
	XMFLOAT3	move;			// �ړ����x
	XMFLOAT3	offset[PLAYER_OFFSET_CNT];		// �c���|���S���̍��W

	BOOL		jump;			// �W�����v�t���O
	float		jumpY;			// �W�����v�̍���
	int			jumpCnt;		// �W�����v���̃J�E���g
	float		jumpYMax;		// 
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER* GetPlayer(void);



