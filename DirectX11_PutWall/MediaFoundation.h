//---------------------------------------------------------------
//06_18_久保田_MediaFoundationを使用した音楽再生
//参考資料
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

				//毎フレーム更新、音楽が鳴りやんだらループ
				void UpDate();

				//音楽再生
				void Play(AudioData &audio);

				//中断(Pause)した音楽を再生
				void PlayBack();

				//音楽の一時停止
				void Pause();

				//音楽終了
				void Stop();

private:
				//現在のBGMの状態が入る
				MFP_MEDIAPLAYER_STATE state;

				//現在再生中のAudioDataのポインタ
				IMFPMediaPlayer *pAudioData;
};