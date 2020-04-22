//=============================================================================
//
// �N���A�^�C������ [clearTime.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _CLEARTIME_H_
#define _CLEARTIME_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "scene2D.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_CLEARTIME_NUMBER 4

//=============================================================================
// �O���錾
//=============================================================================
class CNumber;

//=============================================================================
// �N���X��`
//=============================================================================
class CClearTime : public CScene2D
{
public:
	CClearTime(PRIORITY obj);		// �R���X�g���N�^
	~CClearTime();					// �f�X�g���N�^
	HRESULT Init(void);				// ����������
	void Uninit(void);				// �J������
	void Update(void);				// �X�V����
	void Draw(void);				// �`�揈��

	static CClearTime *Create(void);		// �N���G�C�g����
	static HRESULT Load(void);				// ���[�h����

	void SetNumberPos(D3DXVECTOR3 pos, float fInterval);	// �����̈ʒu�ݒ�
	void SetNumderSize(D3DXVECTOR3 size);					// �����̃T�C�Y�ݒ�
	void SetNumber(int nValue);								// ���̍X�V
	void AddNumber(int nValue);								// ���̉��Z

	int GetNumber(void) { return m_nNumber; }				// �����̎擾

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;					// �e�N�X�`��

	CNumber *m_apNumber[MAX_CLEARTIME_NUMBER];				// �����̃|�C���^
	CScene2D *m_pScene2D;									// �J�e�S���摜�̃|�C���^
	int m_nNumber;											// ��
};
#endif