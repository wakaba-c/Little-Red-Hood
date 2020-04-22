//=============================================================================
//
// �G�t�F�N�g���� [effect.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_EFFECT 3000					// �G�t�F�N�g�̍ő吔

//=============================================================================
// �\���̒�`
//=============================================================================
typedef enum
{
	EFFECTTYPE_ROSE = 0,
	EFFECTTYPE_SMOKE,
	EFFECTTYPE_SANDSMOKE,
	EFFECTTYPE_HALO,
	EFFECTTYPE_SHOCKWAVE,
	EFFECTTYPE_STAR,
	EFFECTTYPE_SLASH,
	EFFECTTYPE_SPHERE,
	EFFECTTYPE_WING,
	EFFECTTYPE_AURA,
	EFFECTTYPE_MAX
} EFFECTTYPE;

typedef enum
{
	EASINGTYPE_NONE = 0,
	EASINGTYPE_IN,
	EASINGTYPE_OUT,
	EASINGTYPE_MAX
} EASINGTYPE;

typedef struct
{
	LPDIRECT3DTEXTURE9		pTexture;				// �e�N�X�`�����̃|�C���^
	D3DXVECTOR2				sprite;					// �摜�̕�����
	bool					bAlpha;					// �A���t�@�u�����f�B���O�̗L��
} EFFECT;

//=============================================================================
// �O���錾
//=============================================================================
class CMeshCapsule;
class CMeshSphere;
class CMeshCube;

//=============================================================================
// �N���X��`
//=============================================================================
class CEffect : public CScene
{
public:
	CEffect(PRIORITY obj);															// �R���X�g���N�^
	~CEffect();																		// �f�X�g���N�^
	HRESULT Init(void);																// ����������
	void Uninit(void);																// �I������
	void Update(void);																// �X�V����
	void Draw(void);																// �`�揈��

	static void Create();															// �N���G�C�g����
	static HRESULT Load(void);														// �f�ނ̓ǂݍ���

	void SetPosition(D3DXVECTOR3 pos);												// �ʒu�̐ݒ�
	void SetMove(D3DXVECTOR3 move);													// �ړ��ʂ̐ݒ�
	void SetUse(bool bValue);														// �g�p���ݒ�
	void SpriteAnimation(D3DXVECTOR2 patternNo, int nPatternAnim, int nHeight);		// �X�v���C�g�A�j���[�V��������
	static void SetEffect(EFFECTTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 size,		// �G�t�F�N�g�̔�������
		D3DXVECTOR3 move, D3DXVECTOR3 moveSize, EASINGTYPE easingType,
		D3DXVECTOR3 rot, D3DXCOLOR col, int nLife, bool bGravity, float fResistance,
		bool bBillboard, int nPatternAnim, int nHeight, D3DXVECTOR3 centerPos,
		float fAngle, float fDistance, float fRotationSpeed);

	static void FallingPetals(bool bGravity, D3DXVECTOR3 pos, int nLife);			// �����U��Ԃт�̃G�t�F�N�g
	static void PetalsGather(D3DXVECTOR3 pos);										// �W�܂�Ԃт�̃G�t�F�N�g
	static void PetalsSoaring(D3DXVECTOR3 pos);										// �Ԃт炪�����オ��G�t�F�N�g
	static void WingStay(D3DXVECTOR3 pos);											// �H�������G�t�F�N�g
	static void SandSmokeEffect(D3DXVECTOR3 pos);									// �����̃G�t�F�N�g
	static void SandSmokeExplosion(D3DXVECTOR3 pos);								// ���o�����G�t�F�N�g
	static void Shockwave(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife);				// �V���b�N�E�E�F�[�u
	static void	Halo(D3DXVECTOR3 pos, D3DXVECTOR3 size);							// �n���[����
	static void Purification(D3DXVECTOR3 pos);										// �򉻕��U
	static void Star(D3DXVECTOR3 pos);												// �X�^�[
	static void Slashing(D3DXVECTOR3 pos, D3DXVECTOR3 rot);							// �a��
	static void Aura(int nCount, bool bJump, D3DXVECTOR3 pos);						// �I�[��
	static void PetalCluster(D3DXVECTOR3 pos, D3DXVECTOR3 rot);						// �Ԃт�̏W����

	bool GetUse(void) { return m_bUse; }											// �g�p��Ԃ̎擾
	int GetAnimPage(void) { return m_AnimPage; }									// �y�[�W���̎擾
	D3DXVECTOR3 GetMove(void) { return m_move; }									// �ړ��ʂ̎擾
	D3DXVECTOR3 GetPosition(void) { return m_pos; }									// �ʒu�̎擾
	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; }								// ���[���h�}�g���b�N�X�̎擾

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};

private:
#ifdef _DEBUG
	void Debug(void);

	/* ===============�����蔻�����=============== */
	CMeshCapsule					*m_pCapsule;									// �J�v�Z���̃|�C���^
	CMeshSphere						*m_pSphere;										// ���̂̃|�C���^
	CMeshCube						*m_pCube;										// �l�p�`�̃|�C���^
	ImVec4							debug_size;										// �R���C�_�[�T�C�Y
	ImVec4							debug_pos;										// �R���C�_�[�|�W�V�����I�t�Z�b�g�l
#endif

	void MakeVertex(void);															// ���_���̍쐬
	void Animation(void);															// �A�j���[�V��������

	static CEffect *m_pEffect[MAX_EFFECT];											// �������ꂽ�G�t�F�N�g�|�C���^

	/* ================ �X�e�[�^�X ================ */
	static LPDIRECT3DTEXTURE9		m_pTexture[EFFECTTYPE_MAX];						// �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3						m_pos;											// �|�W�V����

	D3DXVECTOR3						m_centerPos;									// ��]���S
	float							m_fRotationSpeed;								// ��]���x
	float							m_fAngle;
	float							m_fDistance;

	D3DXVECTOR3						m_rot;											// ��]��
	D3DXVECTOR3						m_move;											// �ړ���
	D3DXVECTOR3						m_posOld;										// �O�̃|�W�V����
	D3DXVECTOR3						m_size;											// �傫��
	D3DXVECTOR3						m_moveSize;
	D3DXCOLOR						m_col;											// �F
	D3DXMATRIX						m_mtxWorld;										// ���[���h�}�g���b�N�X
	EASINGTYPE						m_easingType;									// �C�[�W���O�^�C�v
	float							m_fRadius;										// ���a
	float							m_fResistance;									// ��R
	int								m_nLife;										// ���C�t
	int								m_nMaxLife;										// ���C�t�̍ő�l
	bool							m_bGravity;										// �d��
	bool							m_bUse;											// �g�p���Ă��邩�ǂ���
	bool							m_bBillboard;									// �r���{�[�h�̎g�p

	/* ================ �p�[�e�B�N�� ================ */
	static EFFECT					m_aEffect[EFFECTTYPE_MAX];						// �G�t�F�N�g
	EFFECTTYPE						m_type;											// �p�[�e�B�N���^�C�v
	int								m_AnimPage;										// ���݂̃y�[�W��
	int								m_AnimCount;									// ���̃y�[�W�J�E���g
	int								m_nInterval;									// ���̃y�[�W�܂ł̊Ԋu

	/*===============3D�����_�����O�֘A===============*/
	LPDIRECT3DVERTEXBUFFER9			m_pVtxBuff;										// ���_�o�b�t�@�ւ̃|�C���^
	LPD3DXMESH						m_pMesh;										// ���b�V�����ւ̃|�C���^
	DWORD							m_nNumMat;										// �}�e���A�����̐�
	LPD3DXBUFFER					m_pBuffMat;										// �}�e���A�����ւ̃|�C���^
};
#endif