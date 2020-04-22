//=============================================================================
//
// �J�������� [camera.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define SPLIT 0.12f;								//1�t���[��������̑����

//=============================================================================
// �N���X��`
//=============================================================================
class CCamera
{
public:
	CCamera();						// �R���X�g���N�^
	~CCamera();						// �f�X�g���N�^
	HRESULT Init(void);				// ����������
	void Uninit(void);				// �J������
	void Update(void);				// �X�V����

	void SetCamera(void);															// �J�����̐ݒ�
	void SetRotation(D3DXVECTOR3 rot);
	void SetPosition(D3DXVECTOR3 pos);												// �J�����z�u����
	void SetDistance(float fDistance);												// ���_�ƒ����_�Ƃ̋�����ݒ�
	bool VFCulling(D3DXVECTOR3 pos, int nType, float fAngle, float fNearClip, float fFarClip);	// ������J�����O����
	void SetPosCamera(D3DXVECTOR3 pos, D3DXVECTOR3 rot);							// �J�����̈ʒu�ݒ�
	void SetRotMotion(D3DXVECTOR3 target, D3DXVECTOR3 move);						// ��]�^���̐ݒ�
	HRESULT SetViewport(D3DXVECTOR2 pos, D3DXVECTOR2 size);							// �`��̈��ݒ�

	D3DXVECTOR3 GetPosition(void) { return m_originPos; }							// �J�����̏ꏊ�擾
	float GetDistance(void) { return m_fDistance; }									// ���_�ƒ����_�Ƃ̋������擾
	D3DXVECTOR3 GetRotation(void) { return m_rot; }									// �J���� �̉�]���擾
	D3DXVECTOR3 GetWorldPos(void) { return m_worldPos; }							// �}�E�X �̃��[���h���W�擾
	D3DXMATRIX GetMtxProjection(void) { return m_mtxProjection; }					// �v���W�F�N�V�����}�g���b�N�X�̎擾
	D3DXMATRIX GetMtxView(void) { return m_mtxView; }								// �r���[�}�g���b�N�X�̎擾
	D3DXVECTOR3 GetPosR(void) { return m_posR; }									// �����_�擾

private:
#ifdef _DEBUG
	LPD3DXFONT			m_pCameraFont = NULL;										// �t�H���g�ւ̃|�C���^
	void Debug(void);				// �f�o�b�O����
#endif

	D3DXVECTOR3 CalcScreenToXZ(float fSx, float fSy, int nScreen_w, int nScreen_h, D3DXMATRIX* mView, D3DXMATRIX* mPrj);	// �X�N���[���̌�_
	D3DXVECTOR3* CalcScreenToWorld(D3DXVECTOR3* pout, float fSx, float fSy, float fZ, int nScreen_w, int nScreen_h, D3DXMATRIX* mView, D3DXMATRIX* mPrj);		// ���[���h���W�ϊ�
	void CameraMove(void);			// �J�����ړ�����

	D3DXVECTOR3	m_posV;				// ���_
	D3DXVECTOR3 m_posR;				// �����_
	D3DXVECTOR3 m_vecU;				// ������x�N�g��
	D3DXVECTOR3 m_posRDest;			// �ړI �̒����_
	D3DXVECTOR3 m_posVDest;			// �ړI �̎��_
	D3DXVECTOR3 m_rot;				// ��]
	D3DXVECTOR3 m_rotDest;			// ��] �̖ڕW

	D3DXVECTOR3 m_target;			// ���Z�l�̍ő�l
	D3DXVECTOR3 m_currentRot;		// ���݂̉��Z�l
	D3DXVECTOR3 m_rotMove;			// 1�t���[��������̉�]��

	D3DXMATRIX	m_mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX	m_mtxView;			// �r���[�}�g���b�N�X
	float		m_fDistance;		// ����
	int			m_nCount;			// �J�E���^
	bool		m_bStorker;			// �v���C���[�Ǐ]���邩
	bool		m_bSmooth;			// �Ȃ߂炩�ɃJ�����ړ������邩
	bool		m_bRotMove;			// ��]�^���̍X�V

	D3DXVECTOR3 m_worldPos;			// �}�E�X �̃��[���h���W

	D3DLIGHT9 m_aLight[3];			// ���C�g �̏��
	int m_nType;					// ���C�g �̎��

	D3DXVECTOR3	m_originPos;		// �J���� �̈ʒu

	D3DXVECTOR2 m_mousePosOld;		// �O��� �}�E�X���W
};
#endif
