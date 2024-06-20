//=============================================================================
//
// �t�B�[���h�\������ [field.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "field.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX		(1)						// �e�N�X�`���̐�

#define MAX_POLYGON		(6)						// �L���[�u�P������̖ʐ�

#define	VALUE_MOVE		(5.0f)					// �ړ���
#define	VALUE_ROTATE	(XM_PI * 0.02f)			// ��]��

#define	SIZE_WH			(100.0f)				// �n�ʂ̃T�C�Y

#define FIELD_SIZE		(3)						// 
#define MY_PI			(3.1415926)
#define MC				(1);
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexField(void);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;	// ���_���
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

#ifdef _DEBUG
static FIELD						g_Field[FIELD_SIZE];				// �|���S���f�[�^
#else 
static FIELD						g_Field;				// �|���S���f�[�^
#endif
static int							g_TexNo;				// �e�N�X�`���ԍ�

static char* g_TextureName[] = {
	//"data/TEXTURE/aaa.png",
	"data/TEXTURE/field000.jpg",
};

// ���_�z��
static VERTEX_3D g_VertexArray[4 * MAX_POLYGON] = {
	// �R�c���W							���_�̌���						�q�f�a�`					�s�d�w���W
{XMFLOAT3(SIZE_WH, -SIZE_WH,  SIZE_WH),  XMFLOAT3(0.0f, 1.0f, 0.0f),XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f)},
{XMFLOAT3(-SIZE_WH, -SIZE_WH,  SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f),XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)},
{XMFLOAT3(SIZE_WH, -SIZE_WH, -SIZE_WH),  XMFLOAT3(0.0f, 1.0f, 0.0f),XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)},
{XMFLOAT3(-SIZE_WH, -SIZE_WH, -SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f),XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)},//��

{XMFLOAT3(-SIZE_WH, SIZE_WH,  SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.7f, 0.3f, 0.3f, 1.0f), XMFLOAT2(0.0f, 0.0f)},
{XMFLOAT3(SIZE_WH, SIZE_WH,  SIZE_WH),  XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.7f, 0.3f, 0.3f, 1.0f), XMFLOAT2(1.0f, 0.0f)},
{XMFLOAT3(-SIZE_WH, SIZE_WH, -SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.7f, 0.3f, 0.3f, 1.0f), XMFLOAT2(0.0f, 1.0f)},
{XMFLOAT3(SIZE_WH, SIZE_WH, -SIZE_WH),  XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.7f, 0.3f, 0.3f, 1.0f), XMFLOAT2(1.0f, 1.0f)},//��

{XMFLOAT3(SIZE_WH,  SIZE_WH, -SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)},
{XMFLOAT3(SIZE_WH,  SIZE_WH,  SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f)},
{XMFLOAT3(SIZE_WH, -SIZE_WH, -SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)},
{XMFLOAT3(SIZE_WH, -SIZE_WH,  SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)},//�E

{XMFLOAT3(-SIZE_WH,  SIZE_WH,  SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f),XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f)},
{XMFLOAT3(-SIZE_WH,  SIZE_WH, -SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f),XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)},
{XMFLOAT3(-SIZE_WH, -SIZE_WH,  SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f),XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)},
{XMFLOAT3(-SIZE_WH, -SIZE_WH, -SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f),XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)},//��

{XMFLOAT3(-SIZE_WH,  SIZE_WH, -SIZE_WH),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 0.5f, 0.796f, 1.0f), XMFLOAT2(1.0f, 0.0f)},
{XMFLOAT3(SIZE_WH,  SIZE_WH, -SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 0.5f, 0.796f, 1.0f), XMFLOAT2(0.0f, 0.0f)},
{XMFLOAT3(-SIZE_WH, -SIZE_WH, -SIZE_WH),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 0.5f, 0.796f, 1.0f), XMFLOAT2(1.0f, 1.0f)},
{XMFLOAT3(SIZE_WH, -SIZE_WH, -SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 0.5f, 0.796f, 1.0f), XMFLOAT2(0.0f, 1.0f)},//�O

{XMFLOAT3(SIZE_WH,  SIZE_WH,  SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f), XMFLOAT2(0.0f, 0.0f)},
{XMFLOAT3(-SIZE_WH,  SIZE_WH, SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f), XMFLOAT2(1.0f, 0.0f)},
{XMFLOAT3(SIZE_WH, -SIZE_WH,  SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f), XMFLOAT2(0.0f, 1.0f)},
{XMFLOAT3(-SIZE_WH, -SIZE_WH, SIZE_WH), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f), XMFLOAT2(1.0f, 1.0f)},//��


};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitField(void)
{
	// ���_�o�b�t�@�̍쐬
	MakeVertexField();

	// �e�N�X�`������
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
		// �ʒu�E��]�E�X�P�[���̏����ݒ�
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
// �I������
//=============================================================================
void UninitField(void)
{
	// ���_�o�b�t�@�̉��
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	// �e�N�X�`���̉��
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
void UpdateField(void)
{
	//CAMERA* cam = GetCamera();

#ifdef _DEBUG
	//for (int i = 0; i < FIELD_SIZE; i++)
	//{
	//	// ��]�������Ⴄ
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
	// ��]�������Ⴄ
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


#ifdef _DEBUG	// �f�o�b�O����\������

	PrintDebugProc("Field:��������\n");

#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawField(void)
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);


	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// ���[���h�}�g���b�N�X�̏�����
#ifdef _DEBUG
	for (int i = 0; i < FIELD_SIZE; i++)
	{
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Field[i].scl.x, g_Field[i].scl.y, g_Field[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Field[i].rot.x, g_Field[i].rot.y, g_Field[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Field[i].pos.x + 250.0f * i, g_Field[i].pos.y, g_Field[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Field[i].mtxWorld, mtxWorld);
		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);		// 4���_����0�Ԗڂ̒��_�ԍ�����`��

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

			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(g_Field.scl.x, g_Field.scl.y, g_Field.scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(g_Field.rot.x, g_Field.rot.y, g_Field.rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_Field.pos.x - 100.0f, g_Field.pos.y, g_Field.pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Field.mtxWorld, mtxWorld);
			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);		// 4���_����0�Ԗڂ̒��_�ԍ�����`��

			GetDeviceContext()->Draw(4, 4);

			GetDeviceContext()->Draw(4, 8);

			GetDeviceContext()->Draw(4, 12);

			GetDeviceContext()->Draw(4, 16);

			GetDeviceContext()->Draw(4, 20);
		}

		{
			mtxWorld = XMMatrixIdentity();

			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(g_Field.scl.x, g_Field.scl.y, g_Field.scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(g_Field.rot.x, g_Field.rot.y, g_Field.rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_Field.pos.x, g_Field.pos.y, g_Field.pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Field.mtxWorld, mtxWorld);
			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);		// 4���_����0�Ԗڂ̒��_�ԍ�����`��

			GetDeviceContext()->Draw(4, 4);

			GetDeviceContext()->Draw(4, 8);

			GetDeviceContext()->Draw(4, 12);

			GetDeviceContext()->Draw(4, 16);

			GetDeviceContext()->Draw(4, 20);
		}

		{
			mtxWorld = XMMatrixIdentity();

			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(g_Field.scl.x, g_Field.scl.y, g_Field.scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(g_Field.rot.x, g_Field.rot.y, g_Field.rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_Field.pos.x + 100.0f, g_Field.pos.y, g_Field.pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Field.mtxWorld, mtxWorld);
			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);		// 4���_����0�Ԗڂ̒��_�ԍ�����`��

			GetDeviceContext()->Draw(4, 4);

			GetDeviceContext()->Draw(4, 8);

			GetDeviceContext()->Draw(4, 12);

			GetDeviceContext()->Draw(4, 16);

			GetDeviceContext()->Draw(4, 20);
		}

		{
			mtxWorld = XMMatrixIdentity();

			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(g_Field.scl.x, g_Field.scl.y, g_Field.scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(g_Field.rot.x, g_Field.rot.y, g_Field.rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_Field.pos.x + 100.0f, g_Field.pos.y+100.0f, g_Field.pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Field.mtxWorld, mtxWorld);
			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);		// 4���_����0�Ԗڂ̒��_�ԍ�����`��

			GetDeviceContext()->Draw(4, 4);

			GetDeviceContext()->Draw(4, 8);

			GetDeviceContext()->Draw(4, 12);

			GetDeviceContext()->Draw(4, 16);

			GetDeviceContext()->Draw(4, 20);
		}

		{
			mtxWorld = XMMatrixIdentity();

			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(g_Field.scl.x, g_Field.scl.y, g_Field.scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(g_Field.rot.x, g_Field.rot.y, g_Field.rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_Field.pos.x - 100.0f, g_Field.pos.y+100.0f, g_Field.pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Field.mtxWorld, mtxWorld);
			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);		// 4���_����0�Ԗڂ̒��_�ԍ�����`��

			GetDeviceContext()->Draw(4, 4);

			GetDeviceContext()->Draw(4, 8);

			GetDeviceContext()->Draw(4, 12);

			GetDeviceContext()->Draw(4, 16);

			GetDeviceContext()->Draw(4, 20);
		}

		{
			mtxWorld = XMMatrixIdentity();

			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(g_Field.scl.x, g_Field.scl.y, g_Field.scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(g_Field.rot.x, g_Field.rot.y, g_Field.rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_Field.pos.x, g_Field.pos.y + 100.0f, g_Field.pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Field.mtxWorld, mtxWorld);
			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);		// 4���_����0�Ԗڂ̒��_�ԍ�����`��

			GetDeviceContext()->Draw(4, 4);

			GetDeviceContext()->Draw(4, 8);

			GetDeviceContext()->Draw(4, 12);

			GetDeviceContext()->Draw(4, 16);

			GetDeviceContext()->Draw(4, 20);
		}

		{
			mtxWorld = XMMatrixIdentity();

			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(g_Field.scl.x, g_Field.scl.y, g_Field.scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(g_Field.rot.x, g_Field.rot.y, g_Field.rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_Field.pos.x, g_Field.pos.y + 200.0f, g_Field.pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Field.mtxWorld, mtxWorld);
			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);		// 4���_����0�Ԗڂ̒��_�ԍ�����`��

			GetDeviceContext()->Draw(4, 4);

			GetDeviceContext()->Draw(4, 8);

			GetDeviceContext()->Draw(4, 12);

			GetDeviceContext()->Draw(4, 16);

			GetDeviceContext()->Draw(4, 20);
		}

		{
			mtxWorld = XMMatrixIdentity();

			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(g_Field.scl.x, g_Field.scl.y, g_Field.scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(g_Field.rot.x, g_Field.rot.y, g_Field.rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_Field.pos.x-100.0f, g_Field.pos.y + 200.0f, g_Field.pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Field.mtxWorld, mtxWorld);
			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);		// 4���_����0�Ԗڂ̒��_�ԍ�����`��

			GetDeviceContext()->Draw(4, 4);

			GetDeviceContext()->Draw(4, 8);

			GetDeviceContext()->Draw(4, 12);

			GetDeviceContext()->Draw(4, 16);

			GetDeviceContext()->Draw(4, 20);
		}

		{
			mtxWorld = XMMatrixIdentity();

			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(g_Field.scl.x, g_Field.scl.y, g_Field.scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(g_Field.rot.x, g_Field.rot.y, g_Field.rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_Field.pos.x+100.0f, g_Field.pos.y + 200.0f, g_Field.pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Field.mtxWorld, mtxWorld);
			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);		// 4���_����0�Ԗڂ̒��_�ԍ�����`��

			GetDeviceContext()->Draw(4, 4);

			GetDeviceContext()->Draw(4, 8);

			GetDeviceContext()->Draw(4, 12);

			GetDeviceContext()->Draw(4, 16);

			GetDeviceContext()->Draw(4, 20);
		}


		{
			mtxWorld = XMMatrixIdentity();

			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(g_Field.scl.x*0.5f, g_Field.scl.y * 0.5f, g_Field.scl.z * 0.5f);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(g_Field.rot.x- MY_PI*0.5f, g_Field.rot.y, g_Field.rot.z+ MY_PI);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_Field.pos.x + 124.0f, g_Field.pos.y + 125.0f, g_Field.pos.z-50.0f);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Field.mtxWorld, mtxWorld);
			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);		// 4���_����0�Ԗڂ̒��_�ԍ�����`��

			GetDeviceContext()->Draw(4, 4);

			GetDeviceContext()->Draw(4, 8);

			GetDeviceContext()->Draw(4, 12);

			GetDeviceContext()->Draw(4, 16);

			GetDeviceContext()->Draw(4, 20);
		}

		{
			mtxWorld = XMMatrixIdentity();

			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(g_Field.scl.x * 0.5f, g_Field.scl.y * 0.5f, g_Field.scl.z * 0.5f);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(g_Field.rot.x + MY_PI * 0.5f, g_Field.rot.y, g_Field.rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_Field.pos.x - 124.0f, g_Field.pos.y + 125.0f, g_Field.pos.z - 50.0f);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Field.mtxWorld, mtxWorld);
			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);		// 4���_����0�Ԗڂ̒��_�ԍ�����`��

			GetDeviceContext()->Draw(4, 4);

			GetDeviceContext()->Draw(4, 8);

			GetDeviceContext()->Draw(4, 12);

			GetDeviceContext()->Draw(4, 16);

			GetDeviceContext()->Draw(4, 20);
		}

		{
			mtxWorld = XMMatrixIdentity();

			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(g_Field.scl.x, g_Field.scl.y, g_Field.scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(g_Field.rot.x, g_Field.rot.y, g_Field.rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_Field.pos.x + 50.0f, g_Field.pos.y + 50.0f, g_Field.pos.z-40.0f);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Field.mtxWorld, mtxWorld);
			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);		// 4���_����0�Ԗڂ̒��_�ԍ�����`��

			GetDeviceContext()->Draw(4, 4);

			GetDeviceContext()->Draw(4, 8);

			GetDeviceContext()->Draw(4, 12);

			GetDeviceContext()->Draw(4, 16);

			GetDeviceContext()->Draw(4, 20);
		}

		{
			mtxWorld = XMMatrixIdentity();

			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(g_Field.scl.x, g_Field.scl.y, g_Field.scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(g_Field.rot.x, g_Field.rot.y, g_Field.rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_Field.pos.x - 50.0f, g_Field.pos.y + 50.0f, g_Field.pos.z-40.0f);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Field.mtxWorld, mtxWorld);
			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);		// 4���_����0�Ԗڂ̒��_�ԍ�����`��

			GetDeviceContext()->Draw(4, 4);

			GetDeviceContext()->Draw(4, 8);

			GetDeviceContext()->Draw(4, 12);

			GetDeviceContext()->Draw(4, 16);

			GetDeviceContext()->Draw(4, 20);
		}

		{
			mtxWorld = XMMatrixIdentity();

			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(g_Field.scl.x * 0.5f, g_Field.scl.y * 0.5f, g_Field.scl.z * 0.5f);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(g_Field.rot.x- 0.5f * MY_PI, g_Field.rot.y, g_Field.rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_Field.pos.x - 50.0f, g_Field.pos.y + 50.0f, g_Field.pos.z - 75.0f);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Field.mtxWorld, mtxWorld);
			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);		// 4���_����0�Ԗڂ̒��_�ԍ�����`��

			GetDeviceContext()->Draw(4, 4);

			GetDeviceContext()->Draw(4, 8);

			GetDeviceContext()->Draw(4, 12);

			GetDeviceContext()->Draw(4, 16);

			GetDeviceContext()->Draw(4, 20);
		}

		{
			mtxWorld = XMMatrixIdentity();

			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(g_Field.scl.x * 0.5f, g_Field.scl.y * 0.5f, g_Field.scl.z * 0.5f);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(g_Field.rot.x-0.5f*MY_PI, g_Field.rot.y, g_Field.rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_Field.pos.x + 50.0f, g_Field.pos.y + 50.0f, g_Field.pos.z - 75.0f);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Field.mtxWorld, mtxWorld);
			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);		// 4���_����0�Ԗڂ̒��_�ԍ�����`��

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

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Field.scl.x, g_Field.scl.y, g_Field.scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Field.rot.x, g_Field.rot.y, g_Field.rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Field.pos.x - 250.0f, g_Field.pos.y, g_Field.pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Field.mtxWorld, mtxWorld);
		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);		// 4���_����0�Ԗڂ̒��_�ԍ�����`��

		GetDeviceContext()->Draw(4, 4);

		GetDeviceContext()->Draw(4, 8);

		GetDeviceContext()->Draw(4, 12);

		GetDeviceContext()->Draw(4, 16);

		GetDeviceContext()->Draw(4, 20);
	}
	{
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Field.scl.x, g_Field.scl.y, g_Field.scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Field.rot.x, g_Field.rot.y, g_Field.rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Field.pos.x + 250.0f, g_Field.pos.y, g_Field.pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Field.mtxWorld, mtxWorld);
		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);		// 4���_����0�Ԗڂ̒��_�ԍ�����`��

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
	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4 * MAX_POLYGON;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// �O���[�o���̒��_�z�񂩂璸�_�o�b�t�@������
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = g_VertexArray;

	GetDevice()->CreateBuffer(&bd, &sd, &g_VertexBuffer);

	return S_OK;
}