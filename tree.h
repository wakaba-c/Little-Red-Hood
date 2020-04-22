//=============================================================================
//
// �؏��� [tree.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _TREE_H_
#define _TREE_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// �񋓑̒�`
//=============================================================================
typedef enum
{
	TREETYPE_NORMAL = 0,
	TREETYPE_MAX
} TREETYPE;

//=============================================================================
// �N���X��`
//=============================================================================
class CTree : public CScene
{
public:
	CTree(PRIORITY obj);
	~CTree();																		// �f�X�g���N�^
	HRESULT Init(void);																// ����������
	void Uninit(void);																// �I������
	void Update(void);																// �X�V����
	void Draw(void);																// �`�揈��

	static CTree *Create(TREETYPE type);											// ��������
	static HRESULT Load(void);														// �f�ނ̓ǂݍ���
	static void Unload(void);														// �f�ނ̊J��

	static void LoadModel(char *add);												// �؂̔z�u���ǂݍ��ݏ���
	void SetType(TREETYPE type);													// ��ނ̐ݒ�֐�

	TREETYPE GetType(void) { return m_type; }										// ��ނ̎擾�֐�
	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; }								// ���[���h�}�g���b�N�X�̎擾

	void OnCollisionEnter(CCollider *pCol) {};
	void OnTriggerEnter(CCollider *pCol) {};

private:
#ifdef _DEBUG
	void Debug(void);																// �f�o�b�O����
#endif
	void MakeVertex(void);															// ���_���̍쐬����

	static LPDIRECT3DTEXTURE9		m_pTexture;										// �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3						m_pos;											// �|�W�V����
	D3DXVECTOR3						m_rot;											// ��]��
	D3DXVECTOR3						m_posOld;										// �O�̃|�W�V����
	D3DXVECTOR3						m_size;											// �傫��
	D3DXCOLOR						m_col;											// �F
	D3DXMATRIX						m_mtxWorld;										// ���[���h�}�g���b�N�X
	TREETYPE						m_type;											// ���
	LPDIRECT3DVERTEXBUFFER9			m_pVtxBuff;										// ���_�o�b�t�@�ւ̃|�C���^
};
#endif