//=============================================================================
//
// �̗̓Q�[�W���� [life.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _LIFE_H_
#define _LIFE_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// �O���錾
//=============================================================================
class CScene3D;

//=============================================================================
// �N���X��`
//=============================================================================
class CLife : public CScene
{
public:
	CLife(PRIORITY obj);				// �R���X�g���N�^
	~CLife();							// �f�X�g���N�^
	HRESULT Init(void);					// ����������
	void Uninit(void);					// �J������
	void Update(void);					// �X�V����
	void Draw(void);					// �`�揈��

	static CLife *Create(void);			// �N���G�C�g����
	static HRESULT Load(void);			// ���[�h����

	void SetLifeBar(float fValue);		// �Q�[�W�̎擾

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};

private:
	void MakeVertex(void);

	typedef enum
	{
		DRAWINGTYPE_FRONT = 0,
		DRAWINGTYPE_FRAME,
		DRAWINGTYPE_BACK,
		DRAWINGTYPE_MAX
	} DRAWINGTYPE;

	D3DXVECTOR3 m_size;								// �t���[���ȊO�̑傫��
	D3DXVECTOR3 m_frameSize;						// �t���[���̑傫��
	CScene3D *m_apScene3D[DRAWINGTYPE_MAX];			// �^�C�v

	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;				// ���_�o�b�t�@�ւ̃|�C���^
	LPD3DXMESH m_pMesh;								// ���b�V�����ւ̃|�C���^
	DWORD m_nNumMat;								// �}�e���A�����̐�
	LPD3DXBUFFER m_pBuffMat;						// �}�e���A�����ւ̃|�C���^
	D3DXMATRIX m_mtxWorld;							// ���[���h�}�g���b�N�X

	float m_fAngle;									// ���a
	float m_fLength;								// ����
	D3DXVECTOR3	m_rot;								// ��]
};
#endif