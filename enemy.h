//=============================================================================
//
// �G���� [enemy.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "scene.h"
#include "model.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_KEY 30				// �L�[�̍ő�l

//=============================================================================
// �񋓑̒�`
//=============================================================================
// �^�[�Q�b�g�^�C�v
typedef enum
{
	TARGETTYPE_NONE = -1,
	TARGETTYPE_PLAYER = 0,
	TARGETTYPE_HOUSE,
	TARGETTYPE_MAX
} TARGETTYPE;

//=============================================================================
// �O���錾
//=============================================================================
class CMeshCapsule;
class CColliderSphere;
class CColliderBox;
class CMeshCube;
class CCollider;
class CLife;

//=============================================================================
// �N���X��`
//=============================================================================
class CEnemy : public CScene
{
public:
	//���[�V�����^�C�v
	typedef enum
	{
		ANIMATIONTYPE_NONE = -1,
		ANIMATIONTYPE_NEUTRAL = 0,
		ANIMATIONTYPE_RUN,
		ANIMATIONTYPE_ATTACK,
		ANIMATIONTYPE_DAMAGE,
		ANIMATIONTYPE_DIE,
		ANIMATIONTYPE_MAX
	} ANIMATIONTYPE;

	//�L�[�v�f
	typedef struct
	{
		D3DXVECTOR3 pos;			// �ʒu
		D3DXVECTOR3 rot;			// ��]
	} KEY;

	//�L�[���
	typedef struct
	{
		KEY aKey[MAX_KEY];			// �p�[�c�̃L�[���
		int nFrame;					// �t���[����
	} KEY_INFO;

	// �A�j���[�V�������
	typedef struct
	{
		int	nLoop;					// ���[�v���邩�ǂ���
		int	nMaxKey;				// �L�[��
		KEY_INFO *apKeyInfo;		// �L�[�̍\����
	} ANIMATION;

	CEnemy(PRIORITY obj);			// �R���X�g���N�^
	~CEnemy();						// �f�X�g���N�^
	HRESULT Init(void);				// ����������
	void Uninit(void);				// �J������
	void Update(void);				// �X�V����
	void Draw(void);				// �`�揈��

	static CEnemy *Create(void);	// �N���G�C�g����
	static HRESULT Load(void);		// ���[�h����
	static void LoadEnemy(char *add);		// �G�z�u���ǂݍ��ݏ���
	void SetTarget(TARGETTYPE target);		// �^�[�Q�b�g�̐ݒ�

	void OnTriggerEnter(CCollider *col);
	void OnCollisionEnter(CCollider *col) {};

	static int GetEnemyKill(void) { return m_nKill; }					// �|�������̎擾
	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; }					// �}�g���b�N�X�̎擾
	CColliderSphere *GetSphere(void) { return m_pSphere; }				// �X�t�B�A�R���C�_�[�̎擾
	TARGETTYPE GetTarget(void) { return m_target; }						// �^�[�Q�b�g�̎擾

private:
#ifdef _DEBUG
	void Debug(void);													// �f�o�b�O����
#endif
	void LoadScript(void);												// �X�N���v�g�f�[�^�̃��[�h�֐�
	void Move(D3DXVECTOR3 &pPos);										// �ړ�����
	void Animation(void);												// �A�j���[�V�����֘A�֐�
	void AnimationSwitch(ANIMATIONTYPE Type);							// �A�j���[�V�����̐؂�ւ�����
	void Collider(void);												// �����蔻��}�l�[�W��

	static LPDIRECT3DTEXTURE9		m_pTexture;							// �e�N�X�`���ւ̃|�C���^
	D3DXMATRIX						m_mtxWorld;							// ���[���h�}�g���b�N�X

	/*===============3D�����_�����O�֘A===============*/
	LPDIRECT3DVERTEXBUFFER9			m_pVtxBuff;							// ���_�o�b�t�@�ւ̃|�C���^
	static LPD3DXMESH				m_pMesh;							// ���b�V�����ւ̃|�C���^
	static DWORD					m_nNumMat;							// �}�e���A�����̐�
	static LPD3DXBUFFER				m_pBuffMat;							// �}�e���A�����ւ̃|�C���^
	CModel							*m_pModel;							// �p�[�c���f���̃|�C���^

	/*===============�A�j���[�V�����֘A===============*/
	int								m_nNumKey;							// �L�[�̑���
	int								m_nNumParts;						// �p�[�c�̑���
	int								m_nCurrentKey;						// ���݂̃L�[No.
	int								m_nCurrentFrame;					// ���݂̃t���[��No.
	int								m_nCntMotion;						// �J�E���^�[���[�V����
	ANIMATION						m_pAnimation[ANIMATIONTYPE_MAX];	// �L�[���|�C���^
	ANIMATIONTYPE					m_AnimationType;					// ���݂̃A�j���[�V�����^�C�v
	D3DXVECTOR3						*m_apCurrentPos;					// 1�t���[��������̈ړ���
	D3DXVECTOR3						*m_apCurrentRot;					// 1�t���[��������̉�]��
	int								m_nInterval;						// �x�e����

	/*=============== �����蔻�� ===============*/
	CColliderSphere					*m_pSphere;							// �����蔻��(��)�̃|�C���^
	CColliderSphere					*m_pAttack;							// �����蔻��(�U��)�̃|�C���^
	CColliderBox					*m_pBox;							// �����蔻��(�L���[�u)�̃|�C���^

	/*=============== �X�e�[�^�X ===============*/
	static int						m_nKill;							// �|������
	D3DXVECTOR3						m_rot;								// ��]
	D3DXVECTOR3						m_move;								// �ړ���
	D3DXVECTOR3						m_dest;								// �ړ���
	D3DXVECTOR3						m_difference;						// ��
	int								m_nLife;							// �̗�
	bool							m_bAnimSwitch;						// �A�j���[�V�����̗L��
	bool							m_bHit;								// �����蔻��
	bool							m_bJump;							// �W�����v
	CLife							*m_pLife;							// �̗̓Q�[�W
	TARGETTYPE						m_target;							// �U���Ώ�
};
#endif