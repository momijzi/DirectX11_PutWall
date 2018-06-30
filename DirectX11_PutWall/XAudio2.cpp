#include "App.hpp"

XAudio2::XAudio2()
{
				if (Create() == false)
				{
								MessageBoxA(App::GetWindowHandle(), "XAudio2èâä˙âªé∏îs", "Error", S_OK);
				}
}

XAudio2::~XAudio2()
{
				Release();
}

XAudio2::WaveData::WaveData()
{
				buf = NULL;
				cnt = 0;
}

XAudio2::WaveData::~WaveData()
{
				if (exist)
				{
								Release();
				}
}

//SEì«Ç›çûÇ›
bool XAudio2::WaveData::Load(const TCHAR *lpFileName)
{
				MMCKINFO mmckinfo;
				PCMWAVEFORMAT pcmwf;
				MMRESULT mmret;

				memset(&g_mmioinfo, 0x00, sizeof(g_mmioinfo));
				g_hmmio = mmioOpen(const_cast<TCHAR *>(lpFileName), &g_mmioinfo, MMIO_READ);
				if (g_hmmio == NULL)return false;
				memset(&g_riffchunkinfo, 0x00, sizeof(g_riffchunkinfo));
				g_riffchunkinfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
				mmret = mmioDescend(g_hmmio, &g_riffchunkinfo, NULL, MMIO_FINDRIFF);
				if (mmret != MMSYSERR_NOERROR)return false;
				memset(&mmckinfo, 0x00, sizeof(mmckinfo));
				mmckinfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
				mmret = mmioDescend(g_hmmio, &mmckinfo, &g_riffchunkinfo, MMIO_FINDCHUNK);
				if (mmret != MMSYSERR_NOERROR)return false;

				if (mmckinfo.cksize >= sizeof(WAVEFORMATEX))
				{
								mmioRead(g_hmmio, (HPSTR)&g_wfx, sizeof(g_wfx));
				}
				else
				{
								mmioRead(g_hmmio, (HPSTR)&pcmwf, sizeof(pcmwf));
								memset(&g_wfx, 0x00, sizeof(g_wfx));
								memcpy(&g_wfx, &pcmwf, sizeof(pcmwf));
								g_wfx.cbSize = 0;
				}
				mmioAscend(g_hmmio, &mmckinfo, 0);
				memset(&g_datachunkinfo, 0x00, sizeof(g_datachunkinfo));
				g_datachunkinfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
				mmret = mmioDescend(g_hmmio, &g_datachunkinfo, &g_riffchunkinfo, MMIO_FINDCHUNK);
				if (mmret != MMSYSERR_NOERROR)return false;
				buflen = g_wfx.nAvgBytesPerSec / 4;
				buf = new unsigned char[buflen*BUFFERQUEUE_ALLOC];
				ptr = buf;
				cnt = (cnt + 1) % BUFFERQUEUE_ALLOC;
				readlen = mmioRead(g_hmmio, (HPSTR)ptr, buflen);
				if (readlen <= 0)
				{
								return false;
				}
				return true;
}

void XAudio2::WaveData::Release()
{
				if (buf != NULL) { delete[] buf; }
				if (g_hmmio != NULL) { mmioClose(g_hmmio, 0); }
}

//èâä˙ê›íË
bool XAudio2::Create()
{
				CoInitializeEx(NULL, COINIT_MULTITHREADED);

				hr = XAudio2Create(&g_lpXAudio, 0, XAUDIO2_DEFAULT_PROCESSOR);
				if (FAILED(hr))
				{
								return false;
				}
				hr = g_lpXAudio->CreateMasteringVoice(&g_lpMasterVoice, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, 0, NULL);
				if (FAILED(hr))
				{
								return false;
				}
				return true;
}

//SEçƒê∂
void XAudio2::Play(const WaveData& waveData)
{
				wave = waveData;
				wave.exist = false;

				hr = g_lpXAudio->CreateSourceVoice(&g_lpSourceVoice, &wave.g_wfx, XAUDIO2_VOICE_NOPITCH, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL);
				if (FAILED(hr))
				{
								return;
				}

				memset(&bufinfo, 0x00, sizeof(bufinfo));
				bufinfo.Flags = ((UINT32)wave.readlen >= wave.buflen) ? 0 : XAUDIO2_END_OF_STREAM;
				bufinfo.AudioBytes = wave.readlen;
				bufinfo.pAudioData = wave.ptr;
				bufinfo.PlayBegin = 0;
				bufinfo.PlayLength = wave.readlen / wave.g_wfx.nBlockAlign;
				hr = g_lpSourceVoice->SubmitSourceBuffer(&bufinfo, NULL);
				if (FAILED(hr))
				{
								return;
				}

				g_lpSourceVoice->GetState(&state);
				if (state.BuffersQueued == 0)
				{
								g_lpSourceVoice->Stop();
								g_lpSourceVoice->FlushSourceBuffers();
								g_lpSourceVoice->SubmitSourceBuffer(&bufinfo);
								g_lpSourceVoice->GetState(&state);

								wave.buflen = wave.g_wfx.nAvgBytesPerSec / 4;
								wave.ptr = wave.buf;
								wave.cnt = (wave.cnt + 1) % BUFFERQUEUE_ALLOC;
								wave.readlen = mmioRead(wave.g_hmmio, (HPSTR)wave.ptr, wave.buflen);
				}

				hr = g_lpSourceVoice->Start(0, XAUDIO2_COMMIT_NOW);

				while (state.BuffersQueued < BUFFERQUEUE_MAX&&wave.g_hmmio != NULL)
				{
								wave.ptr = wave.buf + wave.buflen*wave.cnt;
								wave.readlen = mmioRead(wave.g_hmmio, (HPSTR)wave.ptr, wave.buflen);
								if (wave.readlen <= 0)
								{
												break;
								}
				}
}

//âï˙èàóù
void XAudio2::Release()
{
				if (g_lpSourceVoice != NULL) { g_lpSourceVoice->DestroyVoice(); }
				if (g_lpMasterVoice != NULL) { g_lpMasterVoice->DestroyVoice(); }
				if (g_lpXAudio != NULL) { g_lpXAudio->Release(); }
}