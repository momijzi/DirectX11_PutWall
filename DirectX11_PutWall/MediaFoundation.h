//---------------------------------------------------------------
//06_18_�v�ۓc_MediaFoundation���g�p�������y�Đ�
//�Q�l����
//http://dioltista.blogspot.com/2017/05/c-directx11-mediafoundationmp3.html
//---------------------------------------------------------------
#include <mfplay.h>
#pragma comment(lib,"mfplay.lib")

class MediaFoundation
{
public:
				MediaFoundation();

				~MediaFoundation();

				struct AudioData
				{
								bool Load(const wchar_t* const filePath);
								IMFPMediaPlayer *g_pPlayer = nullptr;
				};

				//���t���[���X�V�A���y�����񂾂烋�[�v
				void UpDate();

				//���y�Đ�
				void Play(AudioData &audio);

				//���f(Pause)�������y���Đ�
				void PlayBack();

				//���y�̈ꎞ��~
				void Pause();

				//���y�I��
				void Stop();

private:
				//���݂�BGM�̏�Ԃ�����
				MFP_MEDIAPLAYER_STATE state;

				//���ݍĐ�����AudioData�̃|�C���^
				IMFPMediaPlayer *pAudioData;
};