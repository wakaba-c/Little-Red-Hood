//=============================================================================
//
// �ړI�\������ [purpose.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _PURPOSE_H_
#define _PURPOSE_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "scene2D.h"

//=============================================================================
// �񋓑̒�`
//=============================================================================
typedef enum
{
	MESSAGETYPE_MOVE = 0,
	MESSAGETYPE_HOUSEGUARD,
	MESSAGETYPE_MAX
} MESSAGETYPE;

//=============================================================================
// �N���X��`
//=============================================================================
class CPurpose : public CScene2D
{
public:
	CPurpose(PRIORITY obj);								// �R���X�g���N�^
	~CPurpose();										// �f�X�g���N�^
	HRESULT Init(void);									// ����������
	void Uninit(void);									// �J������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��

	static CPurpose *Create(void);						// �N���G�C�g����
	static HRESULT Load(void);							// ���[�h����
	void ChangePurpose(MESSAGETYPE type);				// �ړI�ݒ�

	MESSAGETYPE GetPurpose(void) { return m_type; }		// �ړI���擾

private:
	typedef enum
	{
		DRAWINGTYPE_SIGNBORAD = 0,
		DRAWINGTYPE_MESSAGE,
		DRAWINGTYPE_MAX
	} DRAWINGTYPE;

	CScene2D *m_apScene2D[DRAWINGTYPE_MAX] = {};		// �V�[���ւ̃|�C���^
	D3DXVECTOR3 m_size;									// �傫��
	float m_fLength;									//����
	MESSAGETYPE m_type;									// ���b�Z�[�W�̎�ޕύX
};
#endif