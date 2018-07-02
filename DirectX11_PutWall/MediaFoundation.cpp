#include "App.hpp"

MediaFoundation::MediaFoundation()
{
				App::Initialize();
				state = MFP_MEDIAPLAYER_STATE_EMPTY;
}

MediaFoundation::~MediaFoundation()
{
				if (pAudioData)
				{
								pAudioData = nullptr;
				}
}

//音楽データ作成
bool MediaFoundation::AudioData::Load(const wchar_t* const filePath)
{
				HRESULT hr = S_OK;
				hr = MFPCreateMediaPlayer(filePath, FALSE, 0, NULL, App::GetWindowHandle(), &g_pPlayer);
				if (FAILED(hr))
				{
								return false;
				}
				return true;
}

//毎フレーム更新、音楽が鳴りやんだらループ
void MediaFoundation::UpDate()
{
				if (pAudioData)
				{
								pAudioData->GetState(&state);
								if (state == MFP_MEDIAPLAYER_STATE_STOPPED)
								{
												pAudioData->Play();
								}
				}
}

//音楽再生
void MediaFoundation::Play(AudioData &audio)
{
				if (pAudioData)
				{
								pAudioData->GetState(&state);
								if (state == MFP_MEDIAPLAYER_STATE_PLAYING)
								{
												Stop();
								}
				}
				pAudioData = audio.g_pPlayer;
				SetVolume(audio.volume);
				pAudioData->Play();
}

//中断(Pause)した音楽を再生
void MediaFoundation::PlayBack()
{
				if (pAudioData)
				{
								pAudioData->GetState(&state);
								if (state == MFP_MEDIAPLAYER_STATE_PLAYING)
								{
												pAudioData->Play();
								}
				}
}

//音楽の一時停止
void MediaFoundation::Pause()
{
				pAudioData->GetState(&state);
				if (state == MFP_MEDIAPLAYER_STATE_PLAYING)
				{
								pAudioData->Pause();
				}
}

//音楽終了
void MediaFoundation::Stop()
{
				if (pAudioData)
				{
								pAudioData->Stop();
								pAudioData = nullptr;
				}
}

void MediaFoundation::SetVolume(float volume)
{
				if (pAudioData)
				{
								pAudioData->SetVolume(volume);
				}
}
