//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �\���̒�`
//=============================================================================
typedef struct
{
	char *pFilename;			// �t�@�C����
	int nCntLoop;				// ���[�v�J�E���g
} SOUNDPARAM;

//=============================================================================
// �T�E���h�t�@�C��
//=============================================================================
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// �R�̒�
	SOUND_LABEL_SE_GRASS_1,		// ���̓��މ�1
	SOUND_LABEL_SE_GRASS_2,		// ���̓��މ�2
	SOUND_LABEL_SE_GRASS_3,		// ���̓��މ�3
	SOUND_LABEL_SE_GRASS_4,		// ���̓��މ�4
	SOUND_LABEL_SE_SAND_1,		// ���̓��މ�1
	SOUND_LABEL_SE_SAND_2,		// ���̓��މ�2
	SOUND_LABEL_SE_SAND_3,		// ���̓��މ�3
	SOUND_LABEL_SE_SAND_4,		// ���̓��މ�4
	SOUND_LABEL_SE_SLASHING,	// ���̎a����
	SOUND_LABEL_SE_BOMB,		// ������
	SOUND_LABEL_SE_JUMP,		// �W�����v��
	SOUND_LABEL_SE_LANDING,		// ���n��
	SOUND_LABEL_SE_PUNCH,		// �_���[�W��
	SOUND_LABEL_SE_CHARGE,		// �`���[�W��
	SOUND_LABEL_SE_GET,			// �o��
	SOUND_LABEL_MAX
} SOUND_LABEL;

//=============================================================================
// �N���X��`
//=============================================================================
class CSoundCallback : public IXAudio2VoiceCallback {
public:
	CSoundCallback() { }
	~CSoundCallback() { }
	void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 BytesRequired) { }
	void STDMETHODCALLTYPE OnVoiceProcessingPassEnd(void) { }
	void STDMETHODCALLTYPE OnStreamEnd(void) { SetEvent(m_hEvent); }
	void STDMETHODCALLTYPE OnBufferStart(void *pBufferContext) { SetEvent(m_hEvent); }
	void STDMETHODCALLTYPE OnBufferEnd(void *pBufferContext) { }
	void STDMETHODCALLTYPE OnLoopEnd(void *pBufferContext) { }
	void STDMETHODCALLTYPE OnVoiceError(void *pBufferContext, HRESULT Error) { }
private:
	HANDLE m_hEvent = NULL;
};

class CSound
{
public:
	CSound();
	~CSound();
	HRESULT Init(HWND hWnd);
	void Uninit(void);
	void Update(void);

	void SetVolume(SOUND_LABEL label, float fVolume);

	HRESULT PlaySound(SOUND_LABEL label);
	void StopSound(SOUND_LABEL label);
	void StopSound(void);

private:
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2 = NULL;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;			// �}�X�^�[�{�C�X
	IXAudio2SubmixVoice *m_pSubmixVoice = NULL;					// �T�u�~�b�N�X�{�C�X
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX] = {};	// �\�[�X�{�C�X

	BYTE *m_apDataAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^�T�C�Y

	// �e���f�ނ̃p�����[�^
	SOUNDPARAM m_aParam[SOUND_LABEL_MAX] =
	{
		{ "data/sound/bgm/karuizawa-birds-2_01.wav", -1 },					// �^�C�g��
		{ "data/sound/se/grass/dash-lawn1_01.wav", 0 },						// ���̓��މ�1
		{ "data/sound/se/grass/dash-lawn1_02.wav", 0 },						// ���̓��މ�2
		{ "data/sound/se/grass/dash-lawn1_03.wav", 0 },						// ���̓��މ�3
		{ "data/sound/se/grass/dash-lawn1_04.wav", 0 },						// ���̓��މ�4
		{ "data/sound/se/sand/dash-soil1_01.wav", 0 },						// ���̓��މ�1
		{ "data/sound/se/sand/dash-soil1_02.wav", 0 },						// ���̓��މ�2
		{ "data/sound/se/sand/dash-soil1_03.wav", 0 },						// ���̓��މ�3
		{ "data/sound/se/sand/dash-soil1_04.wav", 0 },						// ���̓��މ�4
		{ "data/sound/se/sword-gesture.wav", 0 },							// ���̎a����
		{ "data/sound/se/bomb.wav", 0 },									// ������
		{ "data/sound/se/jump.wav", 0 },									// �W�����v��
		{ "data/sound/se/landing.wav", 0 },									// ���n��
		{ "data/sound/se/punch.wav", 0 },									// �_���[�W��
		{ "data/sound/se/magic-circle.wav", 0 },							// �`���[�W��
		{ "data/sound/se/explosion000.wav", 0 },							// ���艹
	};
};
#endif