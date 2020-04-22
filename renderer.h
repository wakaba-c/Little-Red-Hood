//=============================================================================
//
// �����_���[���� [renderer.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
// �Q�c�|���S�����_�t�H�[�}�b�g( ���_���W[2D] / ���_�J���[ / �e�N�X�`�����W )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
// �R�c�|���S�����_�t�H�[�}�b�g( ���_���W[3D] / ���_�J���[ / �e�N�X�`�����W )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//=============================================================================
// �\���̒�`
//=============================================================================
// �Q�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	float rhw;			// ���W�ϊ��p�W��(1.0f�ŌŒ�)
	D3DCOLOR col;		// ���_�J���[
	D3DXVECTOR2 tex;	// �e�N�X�`�����W
} VERTEX_2D;

// �R�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	D3DXVECTOR3 nor;	//�@���x�N�g��
	D3DCOLOR	col;	//���_�J���[
	D3DXVECTOR2 tex;
} VERTEX_3D;

//=============================================================================
// �O���錾
//=============================================================================
#ifdef _DEBUG
class CDebugProc;
#endif

class CFade;
class CStage;

//=============================================================================
// �N���X��`
//=============================================================================
class CRenderer
{
public:
	CRenderer();													// �R���X�g���N�^
	~CRenderer();													// �f�X�g���N�^
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);		// ����������
	void Uninit(void);												// �J������
	void Update(void);												// �X�V����
	void Draw(void);												// �`�揈��
	void SetUpdate(bool bValue);									// �X�V�����̗L���؂�ւ�

	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }		// �f�o�C�X�̎擾

private:
	LPDIRECT3D9					m_pD3D = NULL;						// Direct3D�I�u�W�F�N�g�ւ̃|�C���^
	LPDIRECT3DDEVICE9			m_pD3DDevice = NULL;				// Direct3D�f�o�C�X�ւ̃|�C���^(�`��ɕK�v)
	LPDIRECT3DTEXTURE9			m_pTextureBG = NULL;				// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffBG = NULL;				// ���_�o�b�t�@�ւ̃|�C���^

	CFade *m_pFade;					// �t�F�[�h�̃|�C���^
	CStage *m_pStage;				// �X�e�[�W�̃|�C���^
	bool m_bWireFrame;				// ���C���[�t���[���̗L��
	bool m_bUpdate;					// �X�V�����̗L��

#ifdef _DEBUG
	void Debug(void);				// �f�o�b�O����
	static CDebugProc *m_pDebug;	// �f�o�b�O�̃|�C���^
#endif
};
#endif