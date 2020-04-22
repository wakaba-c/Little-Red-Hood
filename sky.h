//=============================================================================
//
// �󏈗� [sky.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _SKY_H_
#define _SKY_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// �O���錾
//=============================================================================
class CMeshSphere;

//=============================================================================
// �N���X��`
//=============================================================================
class CSky : public CScene
{
public:
	CSky(PRIORITY obj);									// �R���X�g���N�^
	~CSky();											// �f�X�g���N�^
	HRESULT Init(void);									// ����������
	void Uninit(void);									// �J������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��

	static CSky *Create(void);							// �N���G�C�g����
	static HRESULT Load(void);							// �f�ރf�[�^�̎擾
	static void Unload(void);							// �f�ރf�[�^�̊J��

	void MakeVertex(void);								// ���_���̍쐬

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};

private:
#ifdef _DEBUG
	void Debug(void);
#endif
	static LPDIRECT3DTEXTURE9	m_pTexture;				// �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 m_size;									// �傫��
	D3DXVECTOR3 m_rot;									// ��]��
	D3DXCOLOR	m_color;								// �F
	D3DXMATRIX	m_mtxWorld;								// ���[���h�}�g���b�N�X
	bool m_bDraw;										// �`�悷�邩�ǂ���

	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;				// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuff;				// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	LPD3DXMESH		m_pMesh;							// ���b�V�����ւ̃|�C���^
	DWORD			m_nNumMat;							// �}�e���A�����̐�
	LPD3DXBUFFER	m_pBuffMat;							// �}�e���A�����ւ̃|�C���^

	CMeshSphere		*m_pMeshSphere;						// �X�t�B�A�̃|�C���^

	float m_fAngle;										// �p�x
	float m_fLength;									// ����

	float					m_fRadius;					// �|���S���̔��a
	D3DXCOLOR				m_col;						// �|���S���̐F

	int m_nNumVertexSphere;								// �����_��
	int m_nNumIndexSphere;								// �C���f�b�N�X��
	int m_nNumPolygonSphere;							// ���|���S����

	int m_nNumVertex;									// �����_��
	int m_nNumIndex;									// �C���f�b�N�X��
	int m_nNumPolygon;									// ���|���S����
};
#endif