//=============================================================================
//
// �G�t�F�N�g���� [effect.cpp]
// Author :
//
//=============================================================================
#include "trans.h"
#include "bg.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define EFFECT_TEXTURE_SIZE_X (100) // �e�N�X�`���T�C�Y
#define EFFECT_TEXTURE_SIZE_Y (100) // ����

#define EFFECT_TEXTURE_PATTERN_DIVIDE_X (7)															// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define EFFECT_TEXTURE_PATTERN_DIVIDE_Y (1)															// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define EFFECT_ANIM_PATTERN_NUM (EFFECT_TEXTURE_PATTERN_DIVIDE_X * EFFECT_TEXTURE_PATTERN_DIVIDE_Y) // �A�j���[�V�����p�^�[����
#define EFFECT_TIME_ANIMATION (1)																	// �A�j���[�V�����̐؂�ւ��J�E���g

#define EMISSION_FULL 0 // �p�[�e�B�N���S�����t���O
#define EMISSION_RATE 5 // �p�[�e�B�N���̐����Ԋu(duration/EMISSION_RATE�̐����G�t�F�N�g���o��)

#define EMISSION_WIDTH 50  // �p�[�e�B�N�������͈́i�����j
#define EMISSION_HEIGHT 50 // �p�[�e�B�N�������͈́i�����j

#define TEXTURE_MAX (1) // �e�N�X�`���̐�

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer *g_VertexBuffer = NULL;						  // ���_���
static ID3D11ShaderResourceView *g_Texture[TEXTURE_MAX] = {NULL}; // �e�N�X�`�����

static char *g_TexturName[] = {
	"data/TEXTURE/effect01.png",
};

static BOOL g_Load = FALSE;				   // ���������s�������̃t���O
static TRANS effectWk[EFFECT_NUM_EFFECTS]; // �G�t�F�N�g�\����

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTrans(void)
{
	ID3D11Device *pDevice = GetDevice();

	// �e�N�X�`������
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

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	// ����������
	for (int i = 0; i < EFFECT_NUM_EFFECTS; i++)
	{
		effectWk[i].use = false;
		effectWk[i].elapsed = 0;
	}

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTrans(void)
{
	if (g_Load == FALSE)
		return;

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
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTrans(void)
{
	for (int i = 0; i < EFFECT_NUM_EFFECTS; i++)
	{
		if (effectWk[i].use)
		{
			if (effectWk[i].isEnding)
			{
				effectWk[i].use = FALSE;
				continue;
			}

			// �G�~�b�g���L���ł���΃G�t�F�N�g�쐬�������s��
			if (effectWk[i].isRemoveOnFinish == FALSE)
			{

				// �G�t�F�N�g�쐬���[�g�̑�������
				if (effectWk[i].effectCount < EFFECT_NUM_PARTS)
					effectWk[i].emitCounter++;

				// �o�b�t�@�ɋ󂫂�����A�ǉ��^�C�~���O�����Ă���ΐV���ȃG�t�F�N�g��ǉ�����
				while ((effectWk[i].effectCount < EFFECT_NUM_PARTS) && (effectWk[i].emitCounter > EMISSION_RATE))
				{
					// �S�̒ǉ��t���O��ON�ł���΋󂫗̈�S�ĂɐV���ȃG�t�F�N�g��ǉ�����
					if (EMISSION_FULL)
						effectWk[i].effectCount = EFFECT_NUM_PARTS;
					else
						effectWk[i].effectCount++;

					// �G�t�F�N�g�쐬���[�g�̏�����
					effectWk[i].emitCounter = 0;
				}

				effectWk[i].elapsed++;

				// ���Ԓ���
				if ((effectWk[i].duration != -1) && (effectWk[i].duration < effectWk[i].elapsed))
				{
					effectWk[i].isRemoveOnFinish = TRUE;
				}
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTrans(void)
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	BG *bg = GetBG();

	for (int i = 0; i < EFFECT_NUM_EFFECTS; i++)
	{
		if (effectWk[i].use == TRUE) // ���̃G�t�F�N�g���g���Ă���H
		{							 // Yes
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

			for (int n = 0; n < effectWk[i].effectCount; n++)
			{
				if (effectWk[i].pParticle[n].isFinish == 0)
				{
					// �o���b�g�̈ʒu��e�N�X�`���[���W�𔽉f
					float px = effectWk[i].pParticle[n].pos.x - bg->pos.x; // �G�t�F�N�g�̕\���ʒuX
					float py = effectWk[i].pParticle[n].pos.y - bg->pos.y; // �G�t�F�N�g�̕\���ʒuY
					float pw = EFFECT_TEXTURE_SIZE_X;					   // �G�t�F�N�g�̕\����
					float ph = EFFECT_TEXTURE_SIZE_Y;					   // �G�t�F�N�g�̕\������

					px -= EFFECT_TEXTURE_SIZE_X / 4;
					py -= EFFECT_TEXTURE_SIZE_Y / 4;

					float tw = 1.0f / EFFECT_TEXTURE_PATTERN_DIVIDE_X;												 // �e�N�X�`���̕�
					float th = 1.0f / EFFECT_TEXTURE_PATTERN_DIVIDE_Y;												 // �e�N�X�`���̍���
					float tx = (float)(effectWk[i].pParticle[n].PatternAnim % EFFECT_TEXTURE_PATTERN_DIVIDE_X) * tw; // �e�N�X�`���̍���X���W
					float ty = (float)(effectWk[i].pParticle[n].PatternAnim / EFFECT_TEXTURE_PATTERN_DIVIDE_X) * th; // �e�N�X�`���̍���Y���W

					// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
					SetSpriteColorRotation(g_VertexBuffer,
										   px, py, pw, ph,
										   tx, ty, tw, th,
										   XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
										   0.0f);

					// �|���S���`��
					GetDeviceContext()->Draw(4, 0);
				}
			}
		}
	}
}

//=============================================================================
// �G�t�F�N�g�̃Z�b�g
//
// int duration		�G�t�F�N�g�������̐�������
//=============================================================================
void SetTrans(float x, float y, int duration)
{
	// �������g�p�̃G�t�F�N�g��������������s���Ȃ�( =����ȏ�\���ł��Ȃ����Ď� )
	for (int i = 0; i < EFFECT_NUM_EFFECTS; i++)
	{
		if (effectWk[i].use == false) // ���g�p��Ԃ̃G�t�F�N�g��������
		{
			effectWk[i].use = true;
			effectWk[i].isEnding = false;
			effectWk[i].isRemoveOnFinish = FALSE;

			effectWk[i].duration = duration;
			effectWk[i].pos.x = x; // ���W���Z�b�g
			effectWk[i].pos.y = y; // ���W���Z�b�g

			effectWk[i].effectCount = 0;
			effectWk[i].elapsed = 0;
			effectWk[i].emitCounter = EMISSION_RATE;
			effectWk[i].numFinish = 0;

			return; // 1�Z�b�g�����̂ŏI������
		}
	}
}
