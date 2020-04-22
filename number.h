//=============================================================================
//
// �������� [number.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include  "scene2D.h"

//=============================================================================
// �N���X��`
//=============================================================================
class CNumber : public CScene2D
{
public:
	CNumber(PRIORITY obj);					// �R���X�g���N�^
	~CNumber();								// �f�X�g���N�^
	HRESULT Init(void);						// ����������
	void Uninit(void);						// �J������
	void Update(void);						// �X�V����
	void Draw(void);						// �`�揈��

	static CNumber *Create(void);			// �N���G�C�g����
	static HRESULT Load(void);				// ���[�h����

	void SetNumber(int nNumber);			// �����̐ݒ�
	void MakeVertex(void);					// ���_���̍쐬

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	int m_nNumOld;							// �O�܂ł̐���
};
#endif