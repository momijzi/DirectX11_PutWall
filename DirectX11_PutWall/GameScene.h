#pragma once
class GameScene
{
public:
				GameScene();
				~GameScene() {};

				void SceneManager();

				void MainTurn();

				//�J�����̊p�x���}�E�X�ŕύX����
				//����̓J�����}�l�[�W���[�ŏ����ׂ���������͑Ë�
				void CameraAnglesChangeMouse();
				//�J�����̍��W�ړ�
				void CameraPositionMove();
				//�ړ����Ă���Ƃ����̈ړ��͂ǂ̕����ɍs���Ă��邩���m�F����
				int KeyMoveData();
				//������
				void Release();
private:
				//�Q�[���̗���
				enum GameState { TITLE, PLAY, OVER };
				GameState gameState = TITLE;

				//�v���C���̊�{�I�ȗ���
				enum Scene { TURN_FIRST, PUSH_WALL_SELECT, SET_PUSH_WALL_LENGTH,
								PUSH_WALL, PLAYER_MOVE,TURN_END, DROP_CHACK, DROP_ANIMATION};
				Scene scene = TURN_FIRST;

				//�J��������
				//3D��Ԃ�`��Ɏg�p����J����
				Camera mainCamera;
				const float moveSpeed = 0.1f;
				const float angleSpeed = 0.05f;
				//�ǂƒ��̃u���b�N����ɕ`�悷��
				Wall wall;
				//�v���C���[�̈ړ��Ȃǂ̏������s��
				PlayerManager pMana;
				//UI��`�悷��Ƃ��Ɏg�p����@���������g�Ő錾����UI�`�悷�邾��
				UIData uiData;
				//�J�����̈ړ����s���i���S�ł͂Ȃ����������炩���������̂Łj
				Move move;
				//Wall�̉����o�����s�������̎����p�Ɏg�p
				int testDirection = 0;
				//�c�艽�^�[���Œ��ނ�
				const int downLimit = 4;
				//�o�߃^�[��
				int currentTurn = 0;
				//���ޑ��x�̐ݒ�
				const int downTime = 2;
				//�����Ă���̎��Ԍo�߂�ێ�����
				float downTimeCount = 0;

				MediaFoundation bgm;
				XAudio2 se;

				MediaFoundation::AudioData bgm_title;
				MediaFoundation::AudioData bgm_main;
				MediaFoundation::AudioData bgm_end;

				XAudio2::WaveData se_ok;
				XAudio2::WaveData se_select;
				XAudio2::WaveData se_cancel;
				XAudio2::WaveData se_move;
				XAudio2::WaveData se_stop;
				XAudio2::WaveData se_cutin;

				Ray ray;
};