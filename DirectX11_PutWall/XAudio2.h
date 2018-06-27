#pragma once
//--------------------------------------------------
//06_21_�v�ۓc_XAudio2�ɂ�鉹�y�Đ�(SE)
//�Q�l����
//https://www.timbreofprogram.info/blog/archives/463
//https://www.timbreofprogram.info/blog/archives/464
//--------------------------------------------------

#include <xaudio2.h>
#include <mmsystem.h>

#pragma comment(lib,"xaudio2.lib")
#pragma comment(lib,"winmm.lib")

#define BUFFERQUEUE_MAX 4
#define BUFFERQUEUE_ALLOC (BUFFERQUEUE_MAX+1)

class XAudio2
{
public:
				XAudio2();

				~XAudio2();

				//wave�t�@�C���ǂݍ���
				class WaveData
				{
				public:
								HMMIO g_hmmio = NULL;
								MMIOINFO g_mmioinfo;
								MMCKINFO g_riffchunkinfo;
								MMCKINFO g_datachunkinfo;
								WAVEFORMATEX g_wfx;

								unsigned char *buf;
								unsigned char *ptr;
								UINT32 buflen;
								UINT32 cnt;
								LONG readlen;

								WaveData();
								~WaveData();

								//SE�ǂݍ���
								bool Load(const TCHAR *lpFileName);
				};

				//�����ݒ�
				bool Create();

				//SE�Đ�
				void Play(WaveData waveData);

				//�������
				void Release();

private:

				IXAudio2 *g_lpXAudio = NULL;
				IXAudio2MasteringVoice *g_lpMasterVoice = NULL;
				IXAudio2SourceVoice *g_lpSourceVoice = NULL;

				XAUDIO2_BUFFER bufinfo;
				XAUDIO2_VOICE_STATE state;

				HRESULT hr;

				WaveData wave;
};