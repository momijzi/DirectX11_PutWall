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

//���y�f�[�^�쐬
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

//���t���[���X�V�A���y�����񂾂烋�[�v
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

//���y�Đ�
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

//���f(Pause)�������y���Đ�
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

//���y�̈ꎞ��~
void MediaFoundation::Pause()
{
				pAudioData->GetState(&state);
				if (state == MFP_MEDIAPLAYER_STATE_PLAYING)
				{
								pAudioData->Pause();
				}
}

//���y�I��
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
