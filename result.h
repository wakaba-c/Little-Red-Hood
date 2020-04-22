//=============================================================================
//
// ���U���g���� [result.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �N���X��`
//=============================================================================
class CResult
{
public:
	CResult();								// �R���X�g���N�^
	~CResult();								// �f�X�g���N�^
	HRESULT Init(void);						// ����������
	void Uninit(void);						// �J������
	void Update(void);						// �X�V����
	void Draw(void);						// �`�揈��

	static void LoadAsset(void);			// �A�Z�b�g�̓ǂݍ���

	static void SetIdxKill(int nValue);		// �|��������ݒ�
	static void SetSeconds(int nValue);		// �N���A�����b����ݒ�
	static void SetMinutes(int nValue);		// �N���A����������ݒ�

private:
	static int m_nKill;						// �|������
	static int m_nSeconds;					// �b��
	static int m_nMinutes;					// ����

};
#endif