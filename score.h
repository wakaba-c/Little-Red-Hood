//=============================================================================
//
// �X�R�A���� [score.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "scene2D.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_SCORE_NUMBER 8

//=============================================================================
// �O���錾
//=============================================================================
class CNumber;

//=============================================================================
// �N���X��`
//=============================================================================
class CScore : public CScene2D
{
public:
	CScore(PRIORITY obj);						// �R���X�g���N�^
	~CScore();									// �f�X�g���N�^
	HRESULT Init(void);							// ����������
	void Uninit(void);							// �J������
	void Update(void);							// �X�V����
	void Draw(void);							// �`�揈��

	static CScore *Create(void);				// �N���G�C�g����
	static HRESULT Load(void);					// ���[�h����

	void SetNumberPos(D3DXVECTOR3 pos, float fInterval);		// �����̈ʒu�ݒ�
	void SetNumderSize(D3DXVECTOR3 size);						// �����̑傫���ݒ�
	void SetNumber(int nValue);					// ���̐ݒ�
	void AddNumber(int nValue);					// ���̉��Z

	int GetNumber(void) { return m_nNumber; }	// ���̎擾

private:
	CNumber *m_apNumber[MAX_SCORE_NUMBER];		// �����̃|�C���^
	int m_nNumber;								// ��
};
#endif