//=============================================================================
//
// �G�̐��\������ [enemyUi.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _ENEMYUI_H_
#define _ENEMYUI_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "scene2D.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_ENEMY_NUMBER 2				// ����

//=============================================================================
// �O���錾
//=============================================================================
class CNumber;

//=============================================================================
// �N���X��`
//=============================================================================
class CEnemyUi : public CScene2D
{
public:
	CEnemyUi(PRIORITY obj);										// �R���X�g���N�^
	~CEnemyUi();												// �f�X�g���N�^
	HRESULT Init(void);											// ����������
	void Uninit(void);											// �J������
	void Update(void);											// �X�V����
	void Draw(void);											// �`�揈��

	static CEnemyUi *Create(void);								// �N���G�C�g����
	static HRESULT Load(void);									// ���[�h����

	void SetNumberPos(D3DXVECTOR3 pos, float fInterval);		// ���̈ʒu��ݒ�
	void SetNumderSize(D3DXVECTOR3 size);						// ���̃T�C�Y��ݒ�
	void SetNumber(int nValue);									// ����ݒ�
	void AddNumber(int nValue);									// �������Z

	int GetNumber(void) { return m_nNumber; }					// ���̎擾
private:
	static LPDIRECT3DTEXTURE9 m_pTexture;						// �e�N�X�`�����̃|�C���^
	CNumber *m_apNumber[MAX_ENEMY_NUMBER];						// �����̃|�C���^
	int m_nNumber;												// ��
};
#endif