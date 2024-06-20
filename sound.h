//=============================================================================
//
// �T�E���h���� [sound.h]
//
//=============================================================================
#pragma once

#include <windows.h>
#include "xaudio2.h"						// �T�E���h�����ŕK�v

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
enum 
{
	SOUND_LABEL_BGM_sample000,	// BGM0
	SOUND_LABEL_BGM_sample001,	// BGM1
	SOUND_LABEL_BGM_sample002,	// BGM2
	SOUND_LABEL_BGM_result,
	SOUND_LABEL_SE_menu1,		// MENU���쉹�P
	SOUND_LABEL_SE_confirm,	// 
	SOUND_LABEL_SE_restart,	// 
	SOUND_LABEL_SE_rim_scan,		// 
	SOUND_LABEL_SE_Deny,	// 
	SOUND_LABEL_SE_move_confirm,	// 
	SOUND_LABEL_SE_move_select,		// 
	SOUND_LABEL_SE_cancel,		// 


	SOUND_LABEL_MAX,
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(int label);
void StopSound(int label);
void StopSound(void);

