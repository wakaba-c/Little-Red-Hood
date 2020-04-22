//=============================================================================
//
// �Q�[������ [game.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �O���錾
//=============================================================================
class CScene3D;
class CMeshField;
class CPlayer;
class CMeshSphere;
class CEnemy;
class CSky;
class CPurpose;
class CHouse;
class CHouseUi;
class CPhase;

//=============================================================================
// �N���X��`
//=============================================================================
class CGame
{
public:
	CGame();					// �R���X�g���N�^
	~CGame();					// �f�X�g���N�^
	HRESULT Init(void);			// ����������
	void Uninit(void);			// �J������
	void Update(void);			// �X�V����
	void Draw(void);			// �`�揈��

	static void	LoadAsset(void);		// �A�Z�b�g�̓ǂݍ��ݏ���

	static CMeshField	*GetMesh(void) { return m_pMeshField; }				// ���b�V���t�B�[���h�̎擾
	static CPlayer		*GetPlayer(void) { return m_pPlayer; }				// �v���C���[�̎擾
	static CMeshSphere	*GetMeshSphere(void) { return m_pMeshSphere; }		// ���b�V���X�t�B�A�̎擾
	static CEnemy		*GetEnemy(void) { return m_pEnemy; }				// �G�̎擾
	static CSky			*GetSky(void) { return m_pSky; }					// ��̎擾
	static CPurpose		*GetPurpose(void) { return m_pPurpose; }			// �ړI�\���̎擾
	static CHouse		*GetHouse(void) { return m_pHouse; }				// �Ƃ̎擾

private:
	static CMeshField	*m_pMeshField;										// ���b�V���t�B�[���h�̃|�C���^
	static CPlayer		*m_pPlayer;											// �v���C���[�̃|�C���^
	static CMeshSphere	*m_pMeshSphere;										// ���b�V���X�t�B�A�̃|�C���^
	static CEnemy		*m_pEnemy;											// �G�̃|�C���^
	static CSky			*m_pSky;											// ��̃|�C���^
	static CPurpose		*m_pPurpose;										// �ړI�\���̃|�C���^
	static CHouse		*m_pHouse;											// �Ƃ̃|�C���^
};
#endif