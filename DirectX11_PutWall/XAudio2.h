#pragma once
//--------------------------------------------------
//06_21_久保田_XAudio2による音楽再生(SE)
//参考資料
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

				//waveファイル読み込み
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

								//SE読み込み
								bool Load(const TCHAR *lpFileName);
				};

				//初期設定
				bool Create();

				//SE再生
				void Play(WaveData waveData);

				//解放処理
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