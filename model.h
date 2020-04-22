//=============================================================================
//
// ���f������ [model.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �N���X��`
//=============================================================================
class CModel
{
public:
	CModel();													// �R���X�g���N�^
	~CModel();													// �f�X�g���N�^
	HRESULT Init(void);											// ����������
	void Uninit(void);											// �J������
	void Update(void);											// �X�V����
	void Draw(D3DXMATRIX *mtxWorld);							// �`�揈��

	static CModel *Create(char sAdd[64]);						// �N���G�C�g����
	HRESULT Load(char sAdd[64]);								// ���[�h����

	void SetParent(CModel *pModel);								// �e�̐ݒ�
	void SetIndex(int nValue);									// �C���f�b�N�X�̐ݒ�
	void SetPosition(D3DXVECTOR3 pos);							// �ʒu�̐ݒ�
	void SetPosPreset(D3DXVECTOR3 pos);							// �ʒu�̐ݒ�
	void SetRotation(D3DXVECTOR3 rot);							// ��]�ʂ̐ݒ�
	void SetRotPreset(D3DXVECTOR3 rot);							// ��]�ʂ̐ݒ�
	void SetActive(bool bValue);								// �X�V�`��Ώېݒ�

	bool GetActive(void) { return m_bUse; }						// �X�V�`��Ώێ擾
	int GetIndex(void) { return m_nIndex; }						// �C���f�b�N�X�̎擾
	D3DXVECTOR3 GetPosition(void) { return m_pos; }				// �ʒu���擾����
	D3DXVECTOR3 GetPosPreset(void) { return m_originPos; }		// �v���Z�b�g�ʒu�����擾����
	D3DXVECTOR3 GetRotation(void) { return m_rot; }				// ��]�ʂ̎擾
	D3DXVECTOR3 GetRotPreset(void) { return m_originRot; }		// ��]�ʂ̃v���Z�b�g�擾
	LPD3DXMESH GetMesh(void) { return m_pMesh; }				// ���b�V�����̎擾
	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; }			// �}�g���b�N�X�̎擾

private:
	LPDIRECT3DTEXTURE9	*m_pTexture;							// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;							// ���_�o�b�t�@�ւ̃|�C���^
	LPD3DXMESH		m_pMesh;									// ���b�V�����ւ̃|�C���^
	DWORD			m_nNumMat;									// �}�e���A�����̐�
	LPD3DXBUFFER		m_pBuffMat;								// �}�e���A�����ւ̃|�C���^

	int m_nIndex;												// ������ID
	CModel *m_pParent;											// �e��ID

	D3DXVECTOR3 m_pos;											// �|�W�V����
	D3DXVECTOR3 m_posOld;										// �O�̃|�W�V����
	D3DXVECTOR3 m_originPos;									// �|�W�V�����̃v���Z�b�g
	D3DXVECTOR3 m_size;											// �傫��
	D3DXVECTOR3 m_rot;											// ��]��
	D3DXVECTOR3 m_originRot;									// ��]�ʂ̃v���Z�b�g
	D3DXCOLOR	m_color;										// �F
	D3DXMATRIX	m_mtxWorld;										// ���[���h�}�g���b�N�X
	bool		m_bUse;											// �`�悷�邩�ǂ���
};
#endif